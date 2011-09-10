/**
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
 * @legalese
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * @endlegalese
 *
 * @todo Perhaps implement a means of upgrading old plug-in definition files?
 */

#include <QDir>
#include <QtDBus>
#include <QtDebug>
#include <QProcess>
#include "core.hpp"
#include "ipc.hpp"
#include "config.hpp"
#include "plugins.hpp"

using namespace std;

namespace Wintermute {
    namespace Plugins {
        PluginList Factory::s_allPlgns;
        Factory* Factory::s_factory = NULL;

        Factory::Factory() : QObject(Core::instance ()) {}

        void Factory::Startup () {
            qDebug() << "(core) [Factory] Starting up...";
            foreach ( const QString l_plgnName, Factory::instance ()->allPlugins ()){
                qDebug() << "(core) [Factory] Obtaining plugin" << l_plgnName << "..";
                Factory::loadPlugin (l_plgnName);
            }

            emit Factory::instance ()->initialized ();
            qDebug() << "(core) [Factory] Started.";
        }

        /// @todo Find a way to detect if a plug-in instance's rose an error.
        PluginBase* Factory::loadPlugin ( const QString &p_plgnName ) {
	    QApplication::addLibraryPath(WINTER_PLUGIN_PATH);
            const QString l_plgnSpecPath = QString(WINTER_PLUGINSPEC_PATH) + "/" + p_plgnName + ".spec";
            const QString l_plgPth = QString(WINTER_PLUGIN_PATH) + "/lib" + p_plgnName + ".so";

            if (IPC::System::module () == "plugin"){
                if (!QFile::exists (l_plgnSpecPath)){
                    qWarning() << "(core) [Factory] Failed to load " << l_plgnSpecPath;
                    return NULL;
                }

                QSettings* l_plgnSpec = new QSettings(l_plgnSpecPath,QSettings::IniFormat);

                if (!l_plgnSpec->value ("Plugin/Enabled",true).toBool ()){
                    qWarning() << "(core) [Factory] Plugin" << p_plgnName << "disabled.";
                    return NULL;
                }

                QPluginLoader* l_plgnLdr = new QPluginLoader ( l_plgPth );
                PluginBase* l_plgnBase = NULL;
                l_plgnLdr->setLoadHints ( QLibrary::ExportExternalSymbolsHint | QLibrary::ResolveAllSymbolsHint );

                if ( l_plgnLdr->load () ) {
                    l_plgnBase = dynamic_cast<PluginBase*> ( l_plgnLdr->instance () );
                    l_plgnBase->m_plgnLdr = l_plgnLdr;
                    l_plgnBase->m_settings = l_plgnSpec;

                    if ( !l_plgnBase->isSupported () ) {
                        qWarning() << "(core) [Factory] The plugin" << l_plgnBase->name () << "is incompatiable with this version of Wintermute.";
                        return NULL;
                    } else
                        qDebug() << "(core) [Factory] Plugin" << l_plgnBase->name () << "v." << l_plgnBase->version() << "loaded.";

                    qDebug() << "(core) [Factory] {plug-in:" << l_plgnBase->uuid () << "}: Dependency checking skipped.";

                    l_plgnBase->doInitialize ();
                    QObject::connect ( Core::instance (),SIGNAL(stopped()),l_plgnBase,SLOT ( doDeinitialize() ) );
                    Factory::s_allPlgns.insert ( l_plgnBase->uuid (),l_plgnBase );

                    return l_plgnBase;
                } else {
                    qWarning() << "(core) [Factory] Error loading plugin" << l_plgnLdr->fileName () << ":" << l_plgnLdr->errorString ();
                    return NULL;
                }

                emit Factory::instance ()->pluginLoaded (l_plgnBase->uuid ());

            } else {
                PluginInstance* l_inst = new PluginInstance(p_plgnName,new QSettings(l_plgnSpecPath,QSettings::IniFormat));
                Factory::instance ()->m_plgnPool.insert(p_plgnName,l_inst);
            }

            return NULL;
        }

        const QStringList Factory::loadedPlugins () {
            QStringList l_st;
            foreach (PluginInstance* l_inst, Factory::instance ()->m_plgnPool)
                l_st << l_inst->name();

            return l_st;
        }

        const QStringList Factory::allPlugins () {
            QDir *d = new QDir ( WINTER_PLUGINSPEC_PATH );
            d->setFilter ( QDir::Files | QDir::Readable | QDir::NoSymLinks );
            d->setNameFilters (QString("*.spec").split (" "));
            d->setSorting ( QDir::Name );
            return d->entryList().replaceInStrings (".spec","");
        }

        Factory* Factory::instance () {
            if (s_factory == NULL)
                s_factory = new Factory;

            return s_factory;
        }

        /// @todo Should we merge the private slot with this public slot?
        void Factory::unloadPlugin ( const QString& p_plgnName ) {
            doPluginUnload (p_plgnName);
        }

        void Factory::loadStandardPlugin () {
            const QString l_plgnName = Core::arguments ()->value ("plugin").toString ();
            qDebug() << "(core) [Factory] Generating plug-in" << l_plgnName << "...";
            PluginInstanceAdaptor* l_adpt = new PluginInstanceAdaptor(loadPlugin(l_plgnName));
            IPC::System::registerObject ("/Plugin",l_adpt);
        }

        void Factory::unloadStandardPlugin () {
            const QString l_plgnName = Core::arguments ()->value ("plugin").toString ();
            qDebug() << "(core) [Factory] Removing plug-in" << l_plgnName << "...";
            unloadPlugin(l_plgnName);
        }

        void Factory::doPluginCrash (const QString &p_plgnName) {
            doPluginUnload (p_plgnName);
            emit Factory::instance ()->pluginCrashed (p_plgnName);
        }

        void Factory::doPluginLoad (const QString &p_plgName) {
            qDebug() << "(core) [Factory] Plug-in" << p_plgName << "fully loaded.";
            emit Factory::instance ()->pluginLoaded (p_plgName);
        }

        void Factory::doPluginUnload (const QString &p_plgnName) {
            if (Factory::loadedPlugins ().contains (p_plgnName)){
                PluginInstance* l_inst = Factory::instance ()->m_plgnPool.take (p_plgnName);
                l_inst->stop ();
                emit Factory::instance ()->pluginUnloaded (p_plgnName);
                qDebug() << "(core) [Factory] Plug-in" << p_plgnName << "unloaded.";
            }
        }

        /// @todo Unload every loaded plugin and free all resources using signals (we have the process, what's the point? just kill them.)
        void Factory::Shutdown () {
            qDebug() << "(core) [Factory] Unloading plugins..";

            foreach ( PluginInstance* l_inst, s_factory->m_plgnPool )
                unloadPlugin (l_inst->name ());

            qDebug() << "(core) [Factory] Plugins unloaded.";
        }

        const QString PluginBase::author () const { return m_settings->value ("Description/Author").toString (); }

        const QString PluginBase::name () const { return m_settings->value ("Description/Name").toString (); }

        const QString PluginBase::vendorName () const { return m_settings->value ("Description/Vendor").toString (); }

        const QString PluginBase::uuid () const { return m_settings->value ("Version/UUID").toString (); }

        const QString PluginBase::description () const { return m_settings->value ("Description/Blurb").toString (); }

        const QString PluginBase::webPage () const { return m_settings->value ("Description/WebPage").toString (); }

        const QStringList PluginBase::dependencies () const {
            QStringList l_dep = m_settings->value ("Dependencies/Plugins").toStringList ();
            l_dep.append (m_settings->value ("Dependencies/Packages").toStringList ());
            return l_dep;
        }

        const double PluginBase::version () const { return m_settings->value ("Version/Plugin").toDouble (); }

        const double PluginBase::compatVersion () const { return m_settings->value ("Version/Compat").toDouble (); }

        const bool PluginBase::isSupported () const { return WINTERMUTE_VERSION >= compatVersion (); }

        const QString PluginBase::path () const { return m_plgnLdr->fileName (); }

        void PluginBase::doDeinitialize () const {
            qDebug() << "(core) [Factory] {plug-in:" << this->uuid () << "} Deinitializing..";
            emit deinitializing ();
            deinitialize ();
        }

        void PluginBase::doInitialize() const {
            qDebug() << "(core) [Factory] {plug-in:" << this->uuid () << "} Initializing..";
            emit initializing ();
            initialize ();
        }

        PluginInstance::PluginInstance() : m_plgnName(), m_prcss(NULL), m_settings(NULL), QObject(NULL) { }

        PluginInstance::PluginInstance(const PluginInstance &p_plgnInst) : m_plgnName(p_plgnInst.m_plgnName),
            m_prcss(p_plgnInst.m_prcss), m_settings(p_plgnInst.m_settings), QObject(p_plgnInst.parent()) { }

        PluginInstance::PluginInstance(const QString& p_plgnName, QSettings* p_settings) : m_plgnName(p_plgnName),
                m_settings(p_settings), m_prcss(NULL), QObject(Factory::instance ()) {
            connect(this,SIGNAL(crashed(QString)),Factory::instance (),SLOT(doPluginCrash(QString)));
            connect(this,SIGNAL(destroyed(QObject*)),this,SLOT(stop()));
            connect(this,SIGNAL(loaded(QString)),Factory::instance (),SLOT(doPluginLoad(QString)));
            connect(this,SIGNAL(unloaded(QString)),Factory::instance (),SLOT(doPluginUnload(QString)));
            start();
        }

        void PluginInstance::stop (const QDBusMessage p_msg){
            m_prcss->terminate ();
            m_prcss->close ();
        }

        /// @todo Find a way to pass all of the command-line arguments from the core process to sub processes.
        void PluginInstance::start (const QDBusMessage p_msg){
            if (!m_prcss){
                const QStringList l_plgnArgs = QString("--ipc plugin --plugin " + m_plgnName
                    + " --gui " + Core::arguments ()->value ("gui").toString ()).split(" ");

                m_prcss = new QProcess(Factory::instance ());
                connect(m_prcss,SIGNAL(started()),this,SLOT(catchStart()));
                connect(m_prcss,SIGNAL(error(QProcess::ProcessError)),this,SLOT(catchError(QProcess::ProcessError)));
                connect(m_prcss,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(catchExit(int,QProcess::ExitStatus)));

                m_prcss->setProcessChannelMode (QProcess::ForwardedChannels);
                m_prcss->start (QApplication::applicationFilePath (),l_plgnArgs);
            } else
                qDebug() << "(core) [PluginInstance] Plug-in" << m_plgnName << "has already started in pid" << m_prcss->pid ();
        }

        const bool PluginInstance::isActive () {
            return (m_prcss != NULL && m_prcss->state () == QProcess::Running) && !m_plgnName.isEmpty ();
        }

        const QSettings* PluginInstance::settings () { return m_settings; }

        const QString PluginInstance::name () { return m_plgnName; }

        void PluginInstance::catchExit(int p_exitCode, const QProcess::ExitStatus& p_exitStatus){
            switch (p_exitStatus){
                case QProcess::NormalExit:
                    qDebug() << "(core) [PluginInstance] Plug-in" << m_plgnName << "has exitted normally with code" << p_exitCode << ".";
                    emit unloaded (m_plgnName);
                break;

                case QProcess::CrashExit:
                    if (p_exitCode != 0){
                        qDebug() << "(core) [PluginInstance] Plug-in" << m_plgnName << "has exitted abnormally with code" << p_exitCode << ".";
                        emit crashed(m_plgnName);
                    } else catchExit(p_exitCode,QProcess::NormalExit);
                break;

                default:
                    qDebug() << p_exitCode << p_exitStatus;
                break;
            }
        }

        void PluginInstance::catchStart (){
            qDebug() << "(core) [PluginInstance] Plug-in" << m_plgnName << "running.";
            emit loaded(m_plgnName);
        }

        void PluginInstance::catchError (const QProcess::ProcessError &p_err){
            switch (p_err){
                case QProcess::Crashed:
                    if (m_prcss->exitCode () != 0)
                        qDebug() << "(core) [PluginInstance] Plug-in" << m_plgnName << "has crashed.";
                break;

                case QProcess::FailedToStart:
                    qDebug() << "(core) [PluginInstance] Plug-in" << m_plgnName << "has failed to start.";
                break;

                default:
                    qDebug() << m_prcss->errorString ();
                break;
            }
        }
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;
