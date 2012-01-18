/**
 * @file factory.cpp
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
 *
 * @legalese
 * This program is free software; you can redistribute it and/or
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

// Local
#include "core.hpp"
#include "adaptors.hpp"
#include "factory.hpp"
#include "abstractplugin.hpp"
#include "ipc.hpp"

// Qt
#include <QApplication>
#include <QDebug>
#include <QDir>

// Namespaces
using namespace Wintermute::Plugins;

PluginTable Factory::s_plugins;
Factory* Factory::s_factory = NULL;
AbstractPlugin* Factory::s_plgn = NULL;

Factory::Factory() : QObject(Core::instance()) {}

void Factory::Startup()
{
    const bool daemonMode = Core::arguments()->value("daemon").toBool();
    if (!daemonMode)
    {
        qDebug() << "(core) [Factory] Starting up...";
        QSettings* l_settings = new QSettings("Synthetic Intellect Institute", "Wintermute");
        const QVariant l_plgnLstVrnt = l_settings->value("Plugins/AutoStart");

        if (l_plgnLstVrnt.isValid())
        {
            QStringList l_plgnLst = l_plgnLstVrnt.toStringList();
            if (!l_plgnLst.isEmpty())
            {
                foreach (QString l_plgnUuid, l_plgnLst)
                {
                    if (l_plgnUuid.at(0) == QString("*").at(0))
                    {
                        l_plgnUuid.chop(1);
                        qDebug() << "(core) [Factory] Plugin"
                                 << Factory::attribute(l_plgnUuid, "Description/Name").toString()
                                 << "disabled for start-up.";
                    }
                    else {
                        qDebug() << "(core) [Factory] Obtaining plugin"
                                 << Factory::attribute(l_plgnUuid, "Description/Name").toString()
                                 << "..";
                        Factory::loadPlugin(l_plgnUuid);
                    }
                }
            }
        } else {
            qWarning() << "(core) [Factory] No plug-ins determined for loading in configuration file!"      << endl
                       << "Please check" << l_settings->fileName() << "for the option 'Plugins/AutoStart'"  << endl
                       << "and ensure that plug-ins are defined for initial loading of Wintermute." << endl << endl
                       << "*** Reset to default plug-in list.";
            l_settings->setValue("Plugins/AutoStart",
                                 "a9b6b020-f4f2-11e0-be50-0800200c9a66, 81da3bd6-bed5-4c74-aae6-44f48cd5330c, 6d2a54ae-043d-11e1-b46a-93253d2b7d89, 5f0741d0-026b-11e1-8f4e-5999c633b9c0");
        }

        emit Factory::instance()->started();
        qDebug() << "(core) [Factory] Started.";
    } else {
        qDebug() << "(core] [Factory] Executing in daemon mode. Plug-ins are loaded at will.";
        emit Factory::instance()->started ();
    }
}

AbstractPlugin* Factory::loadPlugin(const QString &p_plgnUuid, const bool& p_forceLoad)
{
  if (IPC::System::module () == "plugin" || p_forceLoad )
  {
    AbstractPlugin* l_plgnBase = 0;

    if (!Factory::attribute(p_plgnUuid,"Plugin/Enabled").toBool ())
    {
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

    if ( l_gnrcPlgn->loadLibrary() )
    {
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

QStringList Factory::loadedPlugins ()
{
    QStringList l_st;
    foreach (PluginHandle* l_inst, Factory::instance()->m_plgnPool)
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

Factory* Factory::instance()
{
    if (s_factory == NULL)
        s_factory = new Factory;

    return s_factory;
}

void Factory::unloadPlugin ( const QString& p_plgnUuid ) {
    doPluginUnload (p_plgnUuid);

    //qDebug() << Factory::PluginHandle ()->m_plgnPool.count ();
    if (Factory::instance()->m_plgnPool.count () == 0){
        qWarning() << "(core) [Factory] No plug-ins running; exitting..";
        Core::quit();
    }
}

void Factory::loadStandardPlugin () {
    const QString l_plgnUuid = Core::arguments ()->value ("plugin").toString ();

    //qDebug() << "(core) [Factory] Generating plug-in" << Factory::attribute (l_plgnUuid,"Description/Name").toString () << "; uuid:" << l_plgnUuid << "...";

    s_plgn = loadPlugin(l_plgnUuid);
    PluginHandleAdaptor* l_adpt = new PluginHandleAdaptor(s_plgn);
    IPC::System::registerObject ("/Plugin",l_adpt);
    IPC::System::instance()->m_adapt = l_adpt;
}

AbstractPlugin* Factory::currentPlugin() {
    if (IPC::System::module() == "plugin")
        return s_plgn;
    else {
        qWarning() << "(core) [Factory] This isn't a plug-in PluginHandle; can't find the current plug-in!";
        return NULL;
    }
}

void Factory::unloadStandardPlugin () {
    const QString l_plgnUuid = Core::arguments ()->value ("plugin").toString ();
    //qDebug() << "(core) [Factory] Removing plug-in" << l_plgnUuid << "...";
    unloadPlugin(l_plgnUuid);
}

void Factory::doPluginCrash (const QString &p_plgnUuid) {
    qDebug() << "(core) [Factory] Plug-in" << p_plgnUuid << "crashed.";
    doPluginUnload (p_plgnUuid);
    emit Factory::instance()->pluginCrashed (p_plgnUuid);
}

void Factory::doPluginLoad (const QString &p_plgName) {
    qDebug() << "(core) [Factory] Plug-in" << p_plgName << "fully loaded.";
    emit Factory::instance()->pluginLoaded (p_plgName);
}

void Factory::doPluginUnload (const QString &p_plgnUuid) {
    if (Factory::loadedPlugins ().contains (p_plgnUuid)){
        PluginHandle* l_inst = Factory::instance()->m_plgnPool.take (p_plgnUuid);
        if (l_inst){
            l_inst->stop ();
            emit Factory::instance()->pluginUnloaded (p_plgnUuid);
            qDebug() << "(core) [Factory] Plug-in" << Factory::attribute (p_plgnUuid,"Description/Name").toString () << "unloaded.";
        }
   }
}

/// @todo Unload every loaded plugin and free all resources using signals (we have the process, what's the point? just kill them.)
void Factory::Shutdown () {
    qDebug() << "(core) [Factory] Unloading plugins..";

    foreach ( PluginHandle* l_inst, s_factory->m_plgnPool )
        unloadPlugin (l_inst->uuid ());

    qDebug() << "(core) [Factory] Plugins unloaded.";
}

QSettings* Factory::pluginSettings(const QString &p_plgnUuid){
    const QString l_plgnSpecPath = QString(WNTR_PLUGINSPEC_PATH) + "/" + p_plgnUuid + ".spec";

    if (!QFile::exists (l_plgnSpecPath)){
        qWarning() << "(core) [Factory] Failed to load" << l_plgnSpecPath << "for reading!";
        return NULL;
    }

    return new QSettings(l_plgnSpecPath,QSettings::IniFormat,Factory::instance());
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

const bool Factory::loadBackendPlugin(const QString& p_plgnUuid){
  const QString l_apiUuid = Factory::attribute(p_plgnUuid,"Plugin/API").toString();
  const QString l_plgnTyp = Factory::attribute(p_plgnUuid,"Plugin/Type").toString()

  if ( l_plgnTyp == "Backend" && Factory::currentPlugin() && Factory::currentPlugin()->uuid() != l_apiUuid)
  {
    const QDBusMessage l_callRunningList = QDBusMessage::createMethodCall (WNTR_DBUS_FACTORY_NAME,WNTR_DBUS_FACTORY_OBJNAME,WNTR_DBUS_FACTORY_OBJPATH,"loadedPlugins");
    const QDBusMessage l_replyRunningList = QDBusConnection::sessionBus ().call(l_callRunningList,QDBus::BlockWithGui);

    if (l_replyRunningList.arguments().at(0).toStringList().contains(l_apiUuid))
    {
      QDBusMessage l_callLoadBackend = QDBusMessage::createMethodCall (QString(WNTR_DBUS_PLUGIN_NAME) + "." + l_apiUuid,WNTR_DBUS_PLUGIN_OBJNAME,WNTR_DBUS_PLUGIN_OBJPATH,"loadBackend");
      l_callLoadBackend << p_plgnUuid;
      const QDBusMessage l_replyLoadBackend = QDBusConnection::sessionBus ().call(l_callLoadBackend,QDBus::BlockWithGui);

      if (!l_replyLoadBackend.arguments().at(0).toBool())
	  qDebug() << "(plugin) [Factory] Invoking load of back-end" << Factory::attribute(p_plgnUuid,"Description/Name").toString() << "to API" << Factory::attribute(l_apiUuid,"Description/Name").toString() << "failed.";
      else
	  qDebug() << "(plugin) [Factory] Invoking load of back-end" << Factory::attribute(p_plgnUuid,"Description/Name").toString() << "to API" << Factory::attribute(l_apiUuid,"Description/Name").toString() << "succeeded.";

    } else
	qDebug() << "(plugin) [Factory] API" << Factory::attribute(l_apiUuid,"Description/Name").toString() << "isn't running for back-end" << Factory::attribute(p_plgnUuid,"Description/Name").toString();

    Core::exit();
    return true;
  }
  return false;
}

Factory::ShellPlugin::ShellPlugin() : AbstractPlugin() {
}

Factory::ShellPlugin::ShellPlugin(const QString& p_uuid) : AbstractPlugin() {
    loadSettings(p_uuid);
}

Factory::ShellPlugin::~ShellPlugin() { }
