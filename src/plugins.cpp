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

#include "ipc.hpp"
#include "core.hpp"
#include "config.hpp"
#include "plugins.hpp"
#include "backend.hpp"
#include <QDir>
#include <QtDBus>
#include <QtDebug>
#include <QProcess>
#include <QApplication>

using namespace std;

namespace Wintermute {
    namespace Plugins {
        PluginList Factory::s_plgnLst;
        Factory* Factory::s_fctry = 0;
        AbstractPlugin* Factory::s_rtPlgn = 0;

        Factory::Factory() : QObject(Core::instance ()) { }

        void Factory::Startup () {
            if (!Core::arguments()->value("daemon").toBool()) {
                qDebug() << "(core) [Factory] Starting up...";
                QSettings* l_settings = new QSettings("Synthetic Intellect Institute","Wintermute");
                const QVariant l_plgnLstVrnt = l_settings->value("Plugins/AutoStart");

                if (l_plgnLstVrnt.isValid()) {
                    QStringList l_plgnLst = l_plgnLstVrnt.toStringList();
                    if (!l_plgnLst.isEmpty()) {
                        foreach ( QString l_plgnUuid, l_plgnLst) {
                            if (l_plgnUuid.at(0) == QString("*").at(0)) {
                                l_plgnUuid.chop(1);
                                qDebug() << "(core) [Factory] Plugin" << Factory::attribute(l_plgnUuid,"Description/Name").toString() << "disabled for start-up.";
                            }
                            else {
                                qDebug() << "(core) [Factory] Obtaining"
                                << (( Factory::attribute(l_plgnUuid,"Plugin/Type").toString() == "API") ? "framework" : "plugin")
                                << Factory::attribute(l_plgnUuid,"Description/Name").toString() << "..";
                                Factory::loadPlugin (l_plgnUuid);
                            }
                        }
                    }
                } else {
                    qWarning() << "(core) [Factory] No plug-ins determined for loading in configuration file!"      << endl
                    << "Please check" << l_settings->fileName() << "for the option 'Plugins/AutoStart'"  << endl
                    << "and ensure that plug-ins are defined for initial loading of Wintermute." << endl << endl
                    << "*** Reset to default plug-in list.";
                    l_settings->setValue("Plugins/AutoStart", (QStringList() << "a9b6b020-f4f2-11e0-be50-0800200c9a66"
                                         << "81da3bd6-bed5-4c74-aae6-44f48cd5330c"
                                         << "6d2a54ae-043d-11e1-b46a-93253d2b7d89"
                                         << "5f0741d0-026b-11e1-8f4e-5999c633b9c0").join(", "));
                }

                emit Factory::instance ()->started ();
                qDebug() << "(core) [Factory] Started.";
            } else {
                qDebug() << "(core] [Factory] Executing in daemon mode. Plug-ins are loaded at will.";
                emit Factory::instance ()->started ();
            }
        }

        const bool Factory::loadBackendPlugin(const QString& p_plgnUuid){
            const QString l_apiUuid = Factory::attribute(p_plgnUuid,"Plugin/API").toString();

            if (Factory::attribute(p_plgnUuid,"Plugin/Type").toString() == "Backend" && Factory::currentPlugin() && Factory::currentPlugin()->uuid() != l_apiUuid) {
                const QDBusMessage l_callRunningList = QDBusMessage::createMethodCall (WNTR_DBUS_FACTORY_NAME,WNTR_DBUS_FACTORY_OBJNAME,WNTR_DBUS_FACTORY_OBJPATH,"loadedPlugins");
                const QDBusMessage l_replyRunningList = QDBusConnection::sessionBus ().call(l_callRunningList,QDBus::BlockWithGui);

                if (l_replyRunningList.arguments().at(0).toStringList().contains(l_apiUuid)) {
                    QDBusMessage l_callLoadBackend = QDBusMessage::createMethodCall (QString(WNTR_DBUS_PLUGIN_NAME) + "." + l_apiUuid,WNTR_DBUS_PLUGIN_OBJNAME,WNTR_DBUS_PLUGIN_OBJPATH,"loadBackend");
                    l_callLoadBackend << p_plgnUuid;
                    const QDBusMessage l_replyLoadBackend = QDBusConnection::sessionBus ().call(l_callLoadBackend,QDBus::BlockWithGui);

                    if (!l_replyLoadBackend.arguments().at(0).toBool())
                        qDebug() << "(plugin) [Factory] Invoking load of backend" << Factory::attribute(p_plgnUuid,"Description/Name").toString() << "to API" << Factory::attribute(l_apiUuid,"Description/Name").toString() << "failed.";
                    else
                        qDebug() << "(plugin) [Factory] Invoking load of backend" << Factory::attribute(p_plgnUuid,"Description/Name").toString() << "to API" << Factory::attribute(l_apiUuid,"Description/Name").toString() << "succeeded.";

                } else
                    qDebug() << "(plugin) [Factory] API" << Factory::attribute(l_apiUuid,"Description/Name").toString() << "isn't running for backend" << Factory::attribute(p_plgnUuid,"Description/Name").toString();

                Core::exit();
                return true;
            }

            return false;
        }

        AbstractPlugin* Factory::loadPlugin ( const QString &p_plgnUuid, const bool& p_forceLoad ) {
            if (IPC::System::module () == "plugin" || p_forceLoad ) {
                AbstractPlugin* l_plgnBase = 0;

                if (!Factory::attribute(p_plgnUuid,"Plugin/Enabled").toBool ()) {
                    qWarning() << "(plugin) [Factory] Plugin" << p_plgnUuid << "disabled.";
                    Core::exit(1,true);
                    return 0;
                }

                // Checks if this shouldn't be loaded in this module but rather in it's specified API module.
                if (loadBackendPlugin(p_plgnUuid))
                    return 0;

                const ShellPlugin* l_gnrcPlgn = new ShellPlugin(p_plgnUuid);

                if (!l_gnrcPlgn->loadRequiredComponents())
                    return 0;

                if ( l_gnrcPlgn->loadLibrary() ) {
                    l_plgnBase = dynamic_cast<AbstractPlugin*> ( l_gnrcPlgn->m_loader->instance () );
                    l_plgnBase->m_loader = l_gnrcPlgn->m_loader;
                    l_plgnBase->loadSettings(p_plgnUuid);

                    /// @note Shouldn't the first plug-in be the one loaded here?
                    if (IPC::System::module() == "plugin" && !p_forceLoad)
                        s_rtPlgn = l_plgnBase;

                    l_plgnBase->doStart();
                } else {
                    qDebug()   << "(plugin) [Factory] Error loading plugin" << l_gnrcPlgn->name() << "(" << l_gnrcPlgn->m_loader->fileName() << ")";
                    qWarning() << "(plugin) [Factory] Error message:" << l_gnrcPlgn->m_loader->errorString ();

                    emit Factory::instance ()->pluginCrashed (p_plgnUuid);
                    Core::exit(3,true);
                    return 0;
                }

                emit Factory::instance ()->pluginLoaded (l_plgnBase->uuid ());
                qDebug() << "(plugin) [Factory] Plugin" << l_plgnBase->name () << "loaded.";
                return l_plgnBase;
            } else
                Factory::instance ()->m_plgnPool.insert(p_plgnUuid,(new Instance(p_plgnUuid,Factory::pluginSettings (p_plgnUuid))));

            return 0;
        }

        const QStringList Factory::loadedPlugins () {
            QStringList l_st;
            foreach (Instance* l_inst, Factory::instance ()->m_plgnPool)
            l_st << l_inst->uuid();

            return l_st;
        }

        const QStringList Factory::allPlugins () {
            QDir *d = new QDir ( WNTR_PLUGINSPEC_PATH );
            d->setFilter ( QDir::Files | QDir::Readable | QDir::NoSymLinks );
            d->setNameFilters (QString("*.spec").split (" "));
            d->setSorting ( QDir::Name );
            return d->entryList().replaceInStrings (".spec","");
        }

        Factory* Factory::instance () {
            if (s_fctry == 0)
                s_fctry = new Factory;

            return s_fctry;
        }

        void Factory::unloadPlugin ( const QString& p_plgnUuid ) {
            doPluginUnload (p_plgnUuid);

            if (Factory::instance ()->m_plgnPool.count () == 0) {
                qWarning() << "(core) [Factory] No plug-ins running; exitting..";
                Core::exit (0);
            }
        }

        void Factory::loadStandardPlugin () {
            const QString l_plgnUuid = Core::arguments ()->value ("plugin").toString ();

            //qDebug() << "(core) [Factory] Generating plug-in" << Factory::attribute (l_plgnUuid,"Description/Name").toString () << "; uuid:" << l_plgnUuid << "...";

            loadPlugin(l_plgnUuid);
            InstanceAdaptor* l_adpt = new InstanceAdaptor(Factory::currentPlugin());
            IPC::System::registerObject ("/Plugin",l_adpt);
            IPC::System::instance()->m_adapt = l_adpt;
        }

        AbstractPlugin* Factory::currentPlugin() {
            if (IPC::System::module() == "plugin"){
                if (s_rtPlgn)
                    return s_rtPlgn;
                else {
                    qWarning() << "(core) [Factory] No plug-in has been loaded yet.";
                    return 0;
                }
            } else {
                qWarning() << "(core) [Factory] This isn't a plug-in instance; can't find the current plug-in!";
                return 0;
            }
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
            if (Factory::loadedPlugins ().contains (p_plgnUuid)) {
                Instance* l_inst = Factory::instance ()->m_plgnPool.take (p_plgnUuid);
                if (l_inst) {
                    l_inst->stop ();
                    emit Factory::instance ()->pluginUnloaded (p_plgnUuid);
                    qDebug() << "(core) [Factory] Plug-in" << Factory::attribute (p_plgnUuid,"Description/Name").toString () << "unloaded.";
                }
            }
        }

        /// @todo Unload every loaded plugin and free all resources using signals (we have the process, what's the point? just kill them.)
        void Factory::Shutdown () {
            qDebug() << "(core) [Factory] Unloading plugins..";

            foreach ( Instance* l_inst, s_fctry->m_plgnPool )
                unloadPlugin (l_inst->uuid ());

            qDebug() << "(core) [Factory] Plugins unloaded.";
        }

        QSettings* Factory::pluginSettings(const QString &p_plgnUuid) {
            const QString l_plgnSpecPath = QString(WNTR_PLUGINSPEC_PATH) + "/" + p_plgnUuid + ".spec";

            if (!QFile::exists (l_plgnSpecPath)) {
                qWarning() << "(core) [Factory] Failed to load" << l_plgnSpecPath << "for reading!";
                return 0;
            }

            return new QSettings(l_plgnSpecPath,QSettings::IniFormat,Factory::instance ());
        }

        QVariant Factory::attribute(const QString &p_plgnUuid, const QString &p_attrPth) {
            QSettings* l_plgnSpec = pluginSettings (p_plgnUuid);
            QVariant l_val;

            if (l_plgnSpec) {
                l_val = l_plgnSpec->value (p_attrPth);
                if (l_val.isNull() || !l_val.isValid())
                    l_val = l_plgnSpec->value("Configuration/" + const_cast<QString*>(&p_attrPth)->replace("/",":"));
            }

            return l_val;
        }

        void Factory::setAttribute(const QString &p_plgnUuid, const QString &p_attrPth, const QVariant &p_attrVal) {
            QSettings* l_plgnSpec = pluginSettings (p_plgnUuid);
            if (l_plgnSpec)
                l_plgnSpec->setValue (p_attrPth,p_attrVal);
        }

        AbstractPlugin::AbstractPlugin() : QObject(Core::instance()),
            m_loader(0), m_settings(0), m_configuration(0) {
        }

        AbstractPlugin::AbstractPlugin(QPluginLoader *p_pl) : QObject(Core::instance()),
            m_loader(p_pl), m_settings(0), m_configuration(0) {
        }

        AbstractPlugin::AbstractPlugin(const AbstractPlugin &p_pb): QObject(p_pb.m_loader),
                m_loader(p_pb.m_loader), m_settings(p_pb.m_settings), m_configuration(p_pb.m_configuration) {
        }

        const QString AbstractPlugin::author () const {
            return m_settings->value ("Description/Author").toString ();
        }

        const QString AbstractPlugin::name () const {
            return m_settings->value ("Description/Name").toString ();
        }

        const QString AbstractPlugin::vendorName () const {
            return m_settings->value ("Description/Vendor").toString ();
        }

        const QString AbstractPlugin::uuid () const {
            return m_settings->value ("Version/UUID").toString ();
        }

        const QString AbstractPlugin::description () const {
            return m_settings->value ("Description/Blurb").toString ();
        }

        const QString AbstractPlugin::webPage () const {
            return m_settings->value ("Description/WebPage").toString ();
        }

        const double AbstractPlugin::version () const {
            return m_settings->value ("Version/Plugin").toDouble ();
        }

        const double AbstractPlugin::compatVersion () const {
            return m_settings->value ("Version/Compat",WNTR_VERSION).toDouble ();
        }

        const bool AbstractPlugin::isSupported () const {
            return WNTR_VERSION >= compatVersion ();
        }

        const QStringList AbstractPlugin::plugins () const {
            QStringList l_dep = m_settings->value ("Depends/Plugins").toStringList ();
            l_dep.removeDuplicates ();
            l_dep.removeAll ("None");
            return l_dep;
        }

        const bool AbstractPlugin::loadPlugins() const {
            const QStringList l_plgnLst = this->plugins ();
            qDebug () << "(core) [AbstractPlugin] Loading plug-ins for" << name () << ";" << l_plgnLst.length () << "plugin(s) to be loaded.";

            foreach (const QString l_plgn, l_plgnLst) {
                const QString l_plgnUuid = l_plgn.split (" ").at (0);
                if (Factory::loadedPlugins().contains(l_plgnUuid))
                    qDebug () << "(core) [AbstractPlugin] Dependency" << Factory::attribute(l_plgnUuid,"Description/Name").toString() << "already loaded.";
                else {
                    Factory::ShellPlugin* l_gnrc = new Factory::ShellPlugin(l_plgnUuid);
                    qDebug() << "(core) [AbstractPlugin] Loading dependency" << l_gnrc->name() << "...";

                    if (l_gnrc->loadLibrary())
                        qWarning() << "(core) [AbstractPlugin] Loaded symbols of plug-in" << Factory::attribute(l_plgnUuid,"Description/Name").toString() << ".";
                    else {
                        qWarning() << "(core) [AbstractPlugin] Unable to load symbols of dependency" << l_gnrc->name() << ":" << l_gnrc->m_loader->errorString();
                        return false;
                    }
                }
            }
            return true;
        }

        const bool AbstractPlugin::hasPlugins () const {
            const QStringList l_deps = this->plugins ();
            foreach (const QString l_dep, l_deps) {
                const QString l_depName = l_dep.split (" ").at (0);
                const QString l_depComparison = l_dep.split (" ").at (1);
                const QString l_depVersion = l_dep.split (" ").at (2);

                if (Factory::allPlugins ().contains (l_depName)) {
                    if (!Factory::loadedPlugins ().contains (l_depName))
                        qDebug() << "(core) [AbstractPlugin] Dependency" << l_depName << "of" << this->name () << "isn't loaded.";

                    const AbstractPlugin* l_plgn = Factory::s_plgnLst.value(l_depName);
                    if (l_depComparison == "==") {
                        if (!(l_depVersion.toDouble () == l_plgn->version ())) {
                            qDebug() << "(core) [AbstractPlugin] " << this->name () << "requires" << l_depName << "to have a version of" << l_depVersion;
                            return false;
                        }
                    } else if (l_depComparison == ">") {
                        if (!(l_depVersion.toDouble () > l_plgn->version ())) {
                            qDebug() << "(core) [AbstractPlugin] " << this->name () << "requires" << l_depName << "to have a version greater than" << l_depVersion;
                            return false;
                        }
                    } else if (l_depComparison == "<") {
                        if (!(l_depVersion.toDouble () < l_plgn->version ())) {
                            qDebug() << "(core) [AbstractPlugin] " << this->name () << "requires" << l_depName << "to have a version less than" << l_depVersion;
                            return false;
                        }
                    } else if (l_depComparison == ">=") {
                        if (!(l_depVersion.toDouble () >= l_plgn->version ())) {
                            qDebug() << "(core) [AbstractPlugin] " << this->name () << "requires" << l_depName << "to have a version of at least" << l_depVersion;
                            return false;
                        }
                    } else if (l_depComparison == "==") {
                        if (!(l_depVersion.toDouble () > l_plgn->version ())) {
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

        const bool AbstractPlugin::isLoaded() const {
            return (m_loader) && m_loader->isLoaded();
        }

        const QStringList AbstractPlugin::packages () const {
            QStringList l_dep = m_settings->value ("Depends/Packages").toStringList ();
            l_dep.removeDuplicates ();
            l_dep.removeAll ("None");
            return l_dep;
        }

        /// @note This method requires code from QPackageKit.
        /// @todo Add code using QPackageKit's API to detect if packages are installed on a system and if not, install them.
        const bool AbstractPlugin::loadPackages() const {
            const QStringList l_deps = this->packages ();
            //qDebug () << "(core) [AbstractPlugin] Loading packages for" << name () << ";" << l_deps.length () << "package(s) to be loaded.";
            foreach (const QString l_dep, l_deps) {
                const QString l_depName = l_dep.split (" ").at (0);
            }

            return true;
        }

        /// @note This method requires code from QPackageKit.
        /// @todo Add code using QPackageKit's API to detect if packages are installed on a system.
        const bool AbstractPlugin::hasPackages() const {
            const QStringList l_deps = this->packages ();

            foreach (const QString l_dep, l_deps) {
                const QString l_depName = l_dep.split (" ").at (0);
                const QString l_depComparison = l_dep.split (" ").at (1);
                const QString l_depVersion = l_dep.split (" ").at (2);
            }

            return true;
        }

        /// @note This method searches the plug-in's configuration for default settings. \o/
        const QVariant AbstractPlugin::attribute(const QString& p_attrPath) const {
            QVariant l_val;
            l_val = m_configuration->value(p_attrPath);

            if (l_val.isNull() || !l_val.isValid())
                l_val = m_settings->value("Configuration/" + const_cast<QString*>(&p_attrPath)->replace("/",":"));

            return l_val;
        }

        void AbstractPlugin::setAttribute(const QString& p_attrPath, const QVariant& p_attrVal) {
            m_configuration->setValue(p_attrPath, p_attrVal);
        }

        /// @todo Allow resetting of specific attributes.
        void AbstractPlugin::resetAttributes() {
            m_configuration->clear();
        }

        void AbstractPlugin::doStart()
        {
            connect( Core::instance(), SIGNAL(stopped()), this, SLOT(doStop()) );
            start();
            emit started();
        }

        void AbstractPlugin::doStop() {
            stop();
            emit stopped();
        }

        const bool AbstractPlugin::loadRequiredComponents() const
        {
            if ( !isSupported () ) {
                qWarning() << "(plugin) [Factory] Plugin" << name () << "is incompatiable with this version of Wintermute.";
                return false;
            } else
                qDebug() << "(plugin) [Factory] Plugin" << name () << "v." << version() << "is compatiable with this version of Wintermute.";

            if (!loadPackages()) {
                qWarning() << "(plugin) [Factory] Can't load dependency packages for plugin" << name() << ".";
                return false;
            }

            if (!loadPlugins()) {
                qWarning() << "(plugin) [Factory] Can't load dependency plug-ins for plugin" << name() << ".";
                return false;
            }

            return true;
        }

        void AbstractPlugin::loadSettings(const QString& p_uuid) {
            m_settings = Factory::pluginSettings(p_uuid);
            m_configuration = new QSettings("Synthetic Intellect Institute",p_uuid);
        }

        const bool AbstractPlugin::loadLibrary () const {
            QApplication::addLibraryPath(WNTR_PLUGIN_PATH);
            const QString l_plgnLibrary = m_settings->value("Version/Library").toString();
            const QString l_plgPth = QString(WNTR_PLUGIN_PATH) + "/lib" + l_plgnLibrary + ".so";

            if (QLibrary::isLibrary(l_plgPth)) {
                m_loader = new QPluginLoader ( l_plgPth , const_cast<AbstractPlugin*>(this) );
                m_loader->setLoadHints(QLibrary::ExportExternalSymbolsHint | QLibrary::LoadArchiveMemberHint | QLibrary::ResolveAllSymbolsHint);
                if (!m_loader->load ())
                    qDebug() << "(plugin) [AbstractPlugin] Error loading plug-in library"  << l_plgPth << "."
                    << "(plugin) [AbstractPlugin]" << m_loader->errorString();
            } else
                return false;

            return m_loader->isLoaded();
        }

        AbstractPlugin::~AbstractPlugin () {
            if (m_loader)
                m_loader->unload ();
        }

        Instance::Instance() : m_uuid(), m_process(0), m_settings(0), QObject(Factory::instance()) { }

        Instance::Instance(const Instance &p_plgnInst) : m_uuid(p_plgnInst.m_uuid),
                m_process(p_plgnInst.m_process), m_settings(p_plgnInst.m_settings), QObject(p_plgnInst.parent()) { }

        Instance::Instance(const QString& p_plgnUuid, QSettings* p_settings) : m_uuid(p_plgnUuid),
                m_settings(p_settings), m_process(0), QObject(Factory::instance ()) {
            connect(this,SIGNAL(destroyed(QObject*)),this,SLOT(stop()));
            connect(this,SIGNAL(crashed(QString)),Factory::instance (),SLOT(doPluginCrash(QString)));
            connect(this,SIGNAL(started(QString)),Factory::instance (),SLOT(doPluginLoad(QString)));
            connect(this,SIGNAL(stopped(QString)),Factory::instance (),SLOT(doPluginUnload(QString)));
            start();
        }

        Instance::~Instance() {
            this->stop ();
        }

        void Instance::stop () {
            m_process->terminate ();
            m_process->close ();
        }

        void Instance::start () {
            if (!m_process) {
                m_process = new QProcess(Factory::instance ());
                connect(m_process,SIGNAL(started()),this,SLOT(catchStart()));
                connect(m_process,SIGNAL(error(QProcess::ProcessError)),this,SLOT(catchError(QProcess::ProcessError)));
                connect(m_process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(catchExit(int,QProcess::ExitStatus)));

                m_process->setProcessChannelMode (QProcess::ForwardedChannels);
                m_process->start (QApplication::applicationFilePath (),QStringList() << "--ipc" << "plugin" << "--plugin" << m_uuid);
            } else
                qDebug() << "(core) [PluginInstance] Plug-in" << name() << "has already started in pid" << m_process->pid ();
        }

        const bool Instance::isActive () {
            return (m_process != 0 && m_process->state () == QProcess::Running) && !m_uuid.isEmpty ();
        }

        const QSettings* Instance::settings () {
            return m_settings;
        }

        const QString Instance::uuid () {
            return m_uuid;
        }

        const QString Instance::name() {
            return m_settings->value("Description/Name").toString();
        }

        void Instance::catchExit(int p_exitCode, const QProcess::ExitStatus& p_exitStatus) {
            switch (p_exitStatus) {
            case QProcess::NormalExit:
                if (p_exitCode == 0) {
                    qDebug() << "(core) [PluginInstance] Plug-in" << name() << "has exitted normally with code" << p_exitCode << ".";
                    emit stopped();
                } else
                    catchExit(p_exitCode,QProcess::CrashExit);
                break;

            case QProcess::CrashExit:
                if (p_exitCode != 0) {
                    qDebug() << "(core) [PluginInstance] Plug-in" << name() << "has exitted abnormally with code" << p_exitCode << ".";
                    emit crashed();
                } else catchExit(p_exitCode,QProcess::NormalExit);
                break;

            default:
                qDebug() << p_exitCode << p_exitStatus;
                break;
            }
        }

        void Instance::catchStart () {
            qDebug() << "(core) [PluginInstance] Plug-in" << name() << "running.";
            emit started();
        }

        /// @todo Implement more efficient error-handling here (i.e.: what signal was sent, how it crashed, etc.) ...
        void Instance::catchError (const QProcess::ProcessError &p_err) {
            switch (p_err) {
            case QProcess::Crashed:
                if (m_process->exitCode () != 0)
                    qDebug() << "(core) [PluginInstance] Plug-in" << name() << "has crashed.";
                break;

            case QProcess::FailedToStart:
                qDebug() << "(core) [PluginInstance] Plug-in" << name() << "has failed to start.";
                break;

            default:
                qDebug() << m_process->errorString ();
                break;
            }
        }

        Factory::ShellPlugin::ShellPlugin() : AbstractPlugin() {
        }

        Factory::ShellPlugin::ShellPlugin(const QString& p_uuid) : AbstractPlugin() {
            loadSettings(p_uuid);
        }

        Factory::ShellPlugin::~ShellPlugin() { }
    }
}


// kate: indent-mode cstyle; space-indent on; indent-width 4;
