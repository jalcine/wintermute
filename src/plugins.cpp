/**
 * @file plugins.cpp
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
 *
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
        PluginList Factory::s_plugins;
        Factory* Factory::s_factory = NULL;

        Factory::Factory() : QObject(Core::instance ()) {}

        /// @todo Allow more in-depth listing of plugins (disabled, specific arguments, etc).
        /// @todo Move out the QSettings of the application into Core.
        void Factory::Startup () {
            if (!Core::arguments()->value("daemon").toBool()){
                qDebug() << "(core) [Factory] Starting up...";
                QSettings* l_settings = new QSettings("Synthetic Intellect Institute","Wintermute");
                const QVariant l_plgnLstVrnt = l_settings->value("Plugins/AutoStart");

                if (l_plgnLstVrnt.isValid()){
                    QStringList l_plgnLst = l_plgnLstVrnt.toStringList();
                    if (!l_plgnLst.isEmpty()){
                        foreach ( const QString l_plgnUuid, l_plgnLst){
                            if (l_plgnUuid.at(0) == QString("*").at(0))
                                qDebug() << "(core) [Factory] Plugin" << l_plgnUuid << "disabled for start-up.";
                            else {
                                qDebug() << "(core) [Factory] Obtaining plugin" << l_plgnUuid << "..";
                                Factory::loadPlugin (l_plgnUuid);
                            }
                        }
                    }
                } else {
                    qWarning() << "(core) [Factory] No plug-ins determined for loading in configuration file!"      << endl
                               << "Please check" << l_settings->fileName() << "for the option 'Plugins/AutoStart'"  << endl
                               << "and ensure that plug-ins are defined for initial loading of Wintermute." << endl << endl
                               << "*** Reset to default plug-in list.";

                }

                emit Factory::instance ()->initialized ();
                qDebug() << "(core) [Factory] Started.";
            } else {
                qDebug() << "(core] [Factory] Executing in daemon mode. Plug-ins are loaded at will.";
                emit Factory::instance ()->initialized ();
            }
        }

        AbstractPlugin* Factory::loadPlugin ( const QString &p_plgnUuid, const bool& p_forceLoad ) {
            QApplication::addLibraryPath(WINTER_PLUGIN_PATH);

            if (IPC::System::module () == "plugin" && !p_forceLoad ){
                AbstractPlugin* l_plgnBase = NULL;
                const GenericPlugin* l_gnrcPlgn = new GenericPlugin(p_plgnUuid);

                if (!l_gnrcPlgn->m_settings->value ("Version/Enabled",true).toBool ()){
                    qWarning() << "(plugin) [Factory] Plugin" << p_plgnUuid << "disabled.";
                    Core::endProgram(1,true);
                    return NULL;
                }

                l_gnrcPlgn->loadPackages ();
                l_gnrcPlgn->loadPlugins ();
                l_gnrcPlgn->loadLibrary ();

                if ( l_gnrcPlgn->m_plgnLdr->isLoaded ()) {
                    l_plgnBase = dynamic_cast<AbstractPlugin*> ( l_gnrcPlgn->m_plgnLdr->instance () );
                    l_plgnBase->m_plgnLdr = l_gnrcPlgn->m_plgnLdr;
                    l_plgnBase->m_settings = Factory::pluginSettings (p_plgnUuid);

                    if ( !l_plgnBase->isSupported () ) {
                        qWarning() << "(plugin) [Factory] The plugin" << l_plgnBase->name () << "is incompatiable with this version of Wintermute.";
                        Core::endProgram(2,true);
                        return NULL;
                    } else
                        qDebug() << "(plugin) [Factory] Plugin" << l_plgnBase->name () << "v." << l_plgnBase->version() << "is compatiable with this version of Wintermute.";

                    Factory::s_plugins.insert ( l_plgnBase->uuid (),l_plgnBase );
                    l_plgnBase->doInitialize ();

                    QObject::connect ( Core::instance (), SIGNAL(stopped()) , l_plgnBase, SLOT ( doDeinitialize() ) );
                } else {
                    qWarning() << "(plugin) [Factory] Error loading plugin" << l_gnrcPlgn->m_plgnLdr->fileName () << ";" << l_gnrcPlgn->m_plgnLdr->errorString ();
                    emit Factory::instance ()->pluginCrashed (p_plgnUuid);
                    Core::endProgram(3,true);
                    return NULL;
                }

                emit Factory::instance ()->pluginLoaded (l_plgnBase->uuid ());
                qDebug() << "(plugin) [Factory] Plugin" << l_plgnBase->name () << "loaded.";
                return l_plgnBase;
            } else {
                Instance* l_inst = new Instance(p_plgnUuid,Factory::pluginSettings (p_plgnUuid));
                Factory::instance ()->m_plgnPool.insert(p_plgnUuid,l_inst);
            }

            return NULL;
        }

        const QStringList Factory::loadedPlugins () {
            QStringList l_st;
            foreach (Instance* l_inst, Factory::instance ()->m_plgnPool)
                l_st << l_inst->uuid();

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

        void Factory::unloadPlugin ( const QString& p_plgnUuid ) {
            doPluginUnload (p_plgnUuid);

            //qDebug() << Factory::instance ()->m_plgnPool.count ();
            if (Factory::instance ()->m_plgnPool.count () == 0){
                qWarning() << "(core) [Factory] No plug-ins running; exitting..";
                Core::endProgram (0);
            }
        }

        void Factory::loadStandardPlugin () {
            const QString l_plgnUuid = Core::arguments ()->value ("plugin").toString ();

            qDebug() << "(core) [Factory] Generating plug-in" << Factory::attribute (l_plgnUuid,"Description/Name").toString ()
                     << "; uuid:" << l_plgnUuid << "...";

            InstanceAdaptor* l_adpt = new InstanceAdaptor(loadPlugin(l_plgnUuid));
            IPC::System::registerObject ("/Plugin",l_adpt);
            IPC::System::instance()->m_adapt = l_adpt;
        }

        void Factory::unloadStandardPlugin () {
            const QString l_plgnUuid = Core::arguments ()->value ("plugin").toString ();
            qDebug() << "(core) [Factory] Removing plug-in" << l_plgnUuid << "...";
            unloadPlugin(l_plgnUuid);
        }

        void Factory::doPluginCrash (const QString &p_plgnUuid) {
            qDebug() << "(core) [Factory] Plug-in" << p_plgnUuid << "crashed.";
            doPluginUnload (p_plgnUuid);
            emit Factory::instance ()->pluginCrashed (p_plgnUuid);
        }

        void Factory::doPluginLoad (const QString &p_plgName) {
            qDebug() << "(core) [Factory] Plug-in" << p_plgName << "fully loaded.";
            emit Factory::instance ()->pluginLoaded (p_plgName);
        }

        void Factory::doPluginUnload (const QString &p_plgnUuid) {
            if (Factory::loadedPlugins ().contains (p_plgnUuid)){
                Instance* l_inst = Factory::instance ()->m_plgnPool.take (p_plgnUuid);
                if (l_inst){
                    l_inst->stop ();
                    emit Factory::instance ()->pluginUnloaded (p_plgnUuid);
                    qDebug() << "(core) [Factory] Plug-in" << p_plgnUuid << "unloaded.";
                }
           }

            qDebug() << "(core) [Factory] Plug-in" << Factory::attribute (p_plgnUuid,"Description/Name").toString () << "unloaded.";
        }

        /// @todo Unload every loaded plugin and free all resources using signals (we have the process, what's the point? just kill them.)
        void Factory::Shutdown () {
            qDebug() << "(core) [Factory] Unloading plugins..";

            foreach ( Instance* l_inst, s_factory->m_plgnPool )
                unloadPlugin (l_inst->uuid ());

            qDebug() << "(core) [Factory] Plugins unloaded.";
        }

        QSettings* Factory::pluginSettings(const QString &p_plgnUuid){
            const QString l_plgnSpecPath = QString(WINTER_PLUGINSPEC_PATH) + "/" + p_plgnUuid + ".spec";

            if (!QFile::exists (l_plgnSpecPath)){
                qWarning() << "(core) [Factory] Failed to load" << l_plgnSpecPath << "for reading!";
                return NULL;
            }

            return new QSettings(l_plgnSpecPath,QSettings::IniFormat,Factory::instance ());
        }

        QVariant Factory::attribute(const QString &p_plgnUuid, const QString &p_attrPth){
            QSettings* l_plgnSpec = pluginSettings (p_plgnUuid);
            if (l_plgnSpec)
                return l_plgnSpec->value (p_attrPth);
            else
                return QVariant();
        }

        void Factory::setAttribute(const QString &p_plgnUuid, const QString &p_attrPth, const QVariant &p_attrVal) {
            QSettings* l_plgnSpec = pluginSettings (p_plgnUuid);
            if (l_plgnSpec)
                l_plgnSpec->setValue (p_attrPth,p_attrVal);
        }

        const QString AbstractPlugin::author () const { return m_settings->value ("Description/Author").toString (); }

        const QString AbstractPlugin::name () const { return m_settings->value ("Description/Name").toString (); }

        const QString AbstractPlugin::vendorName () const { return m_settings->value ("Description/Vendor").toString (); }

        const QString AbstractPlugin::uuid () const { return m_settings->value ("Version/UUID").toString (); }

        const QString AbstractPlugin::description () const { return m_settings->value ("Description/Blurb").toString (); }

        const QString AbstractPlugin::webPage () const { return m_settings->value ("Description/WebPage").toString (); }

        const double AbstractPlugin::version () const { return m_settings->value ("Version/Plugin").toDouble (); }

        const double AbstractPlugin::compatVersion () const { return m_settings->value ("Version/Compat",WINTERMUTE_VERSION).toDouble (); }

        const bool AbstractPlugin::isSupported () const { return WINTERMUTE_VERSION >= compatVersion (); }

        const QStringList AbstractPlugin::plugins () const {
            QStringList l_dep = m_settings->value ("Depends/Plugins").toStringList ();
            l_dep.removeDuplicates ();
            l_dep.removeAll ("None");
            return l_dep;
        }

        void AbstractPlugin::loadPlugins() const {
            const QStringList l_plgnLst = this->plugins ();
            qDebug () << "(core) [AbstractPlugin] Loading plug-ins for" << name () << ";" << l_plgnLst.length () << "plugin(s) to be loaded." << endl
                      << l_plgnLst;

            foreach (const QString l_plgn, l_plgnLst){
                const QString l_plgnUuid = l_plgn.split (" ").at (0);
                qDebug() << "(core) [AbstractPlugin] Loading dependency" << l_plgnUuid << "...";
                Factory::GenericPlugin* l_gnrc = new Factory::GenericPlugin(l_plgnUuid);
                l_gnrc->loadLibrary();
            }
        }

        const bool AbstractPlugin::hasPlugins () const {
            const QStringList l_deps = this->plugins ();
            foreach (const QString l_dep, l_deps){
                const QString l_depName = l_dep.split (" ").at (0);
                const QString l_depComparison = l_dep.split (" ").at (1);
                const QString l_depVersion = l_dep.split (" ").at (2);

                if (Factory::allPlugins ().contains (l_depName)){
                    if (!Factory::loadedPlugins ().contains (l_depName))
                        qDebug() << "(core) [AbstractPlugin] Dependency" << l_depName << "of" << this->name () << "isn't loaded.";

                    const AbstractPlugin* l_plgn = Factory::s_plugins.value (l_depName);
                    if (l_depComparison == "=="){
                        if (!(l_depVersion.toDouble () == l_plgn->version ())){
                            qDebug() << "(core) [AbstractPlugin] " << this->name () << "requires" << l_depName << "to have a version of" << l_depVersion;
                            return false;
                        }
                    } else if (l_depComparison == ">"){
                        if (!(l_depVersion.toDouble () > l_plgn->version ())){
                            qDebug() << "(core) [AbstractPlugin] " << this->name () << "requires" << l_depName << "to have a version greater than" << l_depVersion;
                            return false;
                        }
                    } else if (l_depComparison == "<"){
                        if (!(l_depVersion.toDouble () < l_plgn->version ())){
                            qDebug() << "(core) [AbstractPlugin] " << this->name () << "requires" << l_depName << "to have a version less than" << l_depVersion;
                            return false;
                        }
                    } else if (l_depComparison == ">="){
                        if (!(l_depVersion.toDouble () >= l_plgn->version ())){
                            qDebug() << "(core) [AbstractPlugin] " << this->name () << "requires" << l_depName << "to have a version of at least" << l_depVersion;
                            return false;
                        }
                    } else if (l_depComparison == "=="){
                        if (!(l_depVersion.toDouble () > l_plgn->version ())){
                                qDebug() << "(core) [AbstractPlugin] " << this->name () << "requires" << l_depName << "to have a version of at most" << l_depVersion;
                                return false;
                            }
                    } else {
                        qDebug() << "(core) [AbstractPlugin] <" << this->name () << "> : Invalid version string (" << l_depComparison << ").";
                    }
                }
            }

            return true;
        }

        const QStringList AbstractPlugin::packages () const {
            QStringList l_dep = m_settings->value ("Depends/Packages").toStringList ();
            l_dep.removeDuplicates ();
            l_dep.removeAll ("None");
            return l_dep;
        }

        /// @note This method requires code from QPackageKit.
        /// @todo Add code using QPackageKit's API to detect if packages are installed on a system and if not, install them.
        void AbstractPlugin::loadPackages() const {
            const QStringList l_deps = this->packages ();
            qDebug () << "(core) [AbstractPlugin] Loading packages for" << name () << ";" << l_deps.length () << "package(s) to be loaded.";
            foreach (const QString l_dep, l_deps){
                const QString l_depName = l_dep.split (" ").at (0);
            }
        }

        /// @note This method requires code from QPackageKit.
        /// @todo Add code using QPackageKit's API to detect if packages are installed on a system.
        const bool AbstractPlugin::hasPackages() const {
            const QStringList l_deps = this->packages ();

            foreach (const QString l_dep, l_deps){
                const QString l_depName = l_dep.split (" ").at (0);
                const QString l_depComparison = l_dep.split (" ").at (1);
                const QString l_depVersion = l_dep.split (" ").at (2);
            }

            return true;
        }

        void AbstractPlugin::doDeinitialize () const {
            qDebug() << "(core) [Factory] {plug-in:" << this->uuid () << "} Deinitializing..";
            emit deinitializing ();
            deinitialize ();
        }

        void AbstractPlugin::doInitialize() const {
            qDebug() << "(core) [Factory] {plug-in:" << this->uuid () << "} Initializing..";
            emit initializing ();
            initialize ();
        }

        const QVariant AbstractPlugin::attribute(const QString& p_attrPath) const {
            QVariant l_vrt = configuration()->value(p_attrPath);
            return l_vrt;
        }

        void AbstractPlugin::setAttribute(const QString& p_attrPath, const QVariant& p_attrVal) {
            configuration()->setValue(p_attrPath, p_attrVal);
        }

        /// @todo Allow resetting of specific attributes.
        void AbstractPlugin::resetAttributes() {
            configuration()->clear();
        }

        /// @todo Obtain a unique configuration file for the plug-in, first at the user level then work the way up.
        QSettings* AbstractPlugin::configuration() const {
            return m_settings;
        }

        /// @todo Return whether or not this method was successful to calling method.
        void AbstractPlugin::loadLibrary () const {
            const QString l_plgnLibrary = m_settings->value("Version/Library").toString();
            const QString l_plgPth = QString(WINTER_PLUGIN_PATH) + "/lib" + l_plgnLibrary + ".so";
            m_plgnLdr = new QPluginLoader ( l_plgPth , Factory::instance () );
            m_plgnLdr->setLoadHints ( QLibrary::ResolveAllSymbolsHint | QLibrary::ExportExternalSymbolsHint | QLibrary::LoadArchiveMemberHint );
            m_plgnLdr->load ();
        }

        AbstractPlugin::~AbstractPlugin (){
            m_plgnLdr->unload ();
            m_plgnLdr->deleteLater ();
            m_settings->deleteLater ();
        }

        Instance::Instance() : m_uuid(), m_prcss(NULL), m_settings(NULL), QObject(Factory::instance()) { }

        Instance::Instance(const Instance &p_plgnInst) : m_uuid(p_plgnInst.m_uuid),
            m_prcss(p_plgnInst.m_prcss), m_settings(p_plgnInst.m_settings), QObject(p_plgnInst.parent()) { }

        Instance::Instance(const QString& p_plgnUuid, QSettings* p_settings) : m_uuid(p_plgnUuid),
                m_settings(p_settings), m_prcss(NULL), QObject(Factory::instance ()) {
            connect(this,SIGNAL(destroyed(QObject*)),this,SLOT(stop()));
            connect(this,SIGNAL(crashed(QString)),Factory::instance (),SLOT(doPluginCrash(QString)));
            connect(this,SIGNAL(started(QString)),Factory::instance (),SLOT(doPluginLoad(QString)));
            connect(this,SIGNAL(stopped(QString)),Factory::instance (),SLOT(doPluginUnload(QString)));
            start();
        }

        Instance::~Instance() {
            this->stop ();
        }

        void Instance::stop (const QDBusMessage p_msg){
            m_prcss->terminate ();
            m_prcss->close ();
        }

        /// @todo Find a way to pass all of the command-line arguments from the core process to sub processes.
        /// @todo Allow instances to have their own arguments passed in.
        void Instance::start (const QDBusMessage p_msg){
            if (!m_prcss){
                QStringList l_plgnArgs;
                l_plgnArgs << "--ipc"    << "plugin"
                           << "--plugin" << m_uuid;

                m_prcss = new QProcess(Factory::instance ());
                connect(m_prcss,SIGNAL(started()),this,SLOT(catchStart()));
                connect(m_prcss,SIGNAL(error(QProcess::ProcessError)),this,SLOT(catchError(QProcess::ProcessError)));
                connect(m_prcss,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(catchExit(int,QProcess::ExitStatus)));

                m_prcss->setProcessChannelMode (QProcess::ForwardedChannels);
                m_prcss->start (QApplication::applicationFilePath (),l_plgnArgs);
                p_msg.createReply(true);
            } else
                qDebug() << "(core) [PluginInstance] Plug-in" << name() << "has already started in pid" << m_prcss->pid ();
        }

        const bool Instance::isActive () {
            return (m_prcss != NULL && m_prcss->state () == QProcess::Running) && !m_uuid.isEmpty ();
        }

        const QSettings* Instance::settings () { return m_settings; }

        const QString Instance::uuid () { return m_uuid; }

        const QString Instance::name() { return m_settings->value("Description/Name").toString(); }

        void Instance::catchExit(int p_exitCode, const QProcess::ExitStatus& p_exitStatus){
            switch (p_exitStatus){
                case QProcess::NormalExit:
                    if (p_exitCode == 0){
                        qDebug() << "(core) [PluginInstance] Plug-in" << name() << "has exitted normally with code" << p_exitCode << ".";
                        emit stopped();
                    } else
                        catchExit(p_exitCode,QProcess::CrashExit);
                break;

                case QProcess::CrashExit:
                    if (p_exitCode != 0){
                        qDebug() << "(core) [PluginInstance] Plug-in" << name() << "has exitted abnormally with code" << p_exitCode << ".";
                        emit crashed();
                    } else catchExit(p_exitCode,QProcess::NormalExit);
                break;

                default:
                    qDebug() << p_exitCode << p_exitStatus;
                break;
            }
        }

        void Instance::catchStart (){
            qDebug() << "(core) [PluginInstance] Plug-in" << name() << "running.";
            emit started();
        }

        /// @todo Implement more efficient error-handling here (i.e.: what signal was sent, how it crashed, etc.).
        void Instance::catchError (const QProcess::ProcessError &p_err){
            switch (p_err){
                case QProcess::Crashed:
                    if (m_prcss->exitCode () != 0)
                        qDebug() << "(core) [PluginInstance] Plug-in" << name() << "has crashed.";
                break;

                case QProcess::FailedToStart:
                    qDebug() << "(core) [PluginInstance] Plug-in" << name() << "has failed to start.";
                break;

                default:
                    qDebug() << m_prcss->errorString ();
                break;
            }
        }
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;
