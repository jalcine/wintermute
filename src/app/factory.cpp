/*
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
 *
 */
/**
 * @file factory.cpp
 * @author Jacky Alcine <jacky.alcine@thesii.org>
 * @author Adrian Borucki <adrian@thesii.org>
 */

#include <QApplication>
#include <QDebug>
#include <QDir>

#include <app/core.hpp>
#include <app/adaptors.hpp>
#include <app/plugin.hpp>
#include <app/ipc.hpp>
#include <app/factory.hpp>
#include <app/factory.hxx>
#include <app/plugin.hxx>
#include <app/shellplugin.h>

using namespace Wintermute;
using namespace Wintermute::Plugins;

WINTER_SINGLETON_DEFINE (Factory)

Factory::Factory() : QObject (Core::instance()), d_ptr (new FactoryPrivate)
{
}

void Factory::startup()
{
    const bool isDaemon = Core::arguments().value ("daemon").toBool();

    if (!isDaemon) {
        qDebug() << "(core) [Factory] Starting up...";
        QSettings* settings = new QSettings ("Synthetic Intellect Institute", "Wintermute");
        const QVariant vrtAutoStartPlugins = settings->value ("Plugins/AutoStart");

        if (vrtAutoStartPlugins.isValid()) {
            QStringList autoPluginList = vrtAutoStartPlugins.toStringList();

            if (!autoPluginList.isEmpty()) {
                foreach (QString plgnUuid, autoPluginList) {
                    bool isDisabled = false;

                    if (plgnUuid.at (0) == QString ("*").at (0)) {
                        plgnUuid.remove ("*");
                        isDisabled = true;
                    }

                    QSettings* pluginSettings = Factory::getPluginSettings (plgnUuid);

                    if (!pluginSettings) {
                        qWarning() << "Settings of plug-in with UUID" << plgnUuid
                                   << "are not accessible." << "Setting Description/Name field to UUID.";
                        Factory::setAttribute (plgnUuid, "Description/Name", QVariant (plgnUuid));
                        isDisabled = true;
                    }

                    if (isDisabled) {
                        qDebug() << "(core) [Factory] Plugin"
                                 << Factory::attribute (plgnUuid, "Description/Name").toString()
                                 << "disabled for start-up.";
                    }
                    else {
                        qDebug() << "(core) [Factory] Obtaining plugin"
                                 << Factory::attribute (plgnUuid, "Description/Name").toString()
                                 << "...";
                        Factory::loadPlugin (plgnUuid);
                    }
                }
            }
        }
        else {
            qWarning() << "(core) [Factory] No plug-ins determined for loading in configuration file!"      << endl
                       << "Please check" << settings->fileName() << "for the option 'Plugins/AutoStart'"  << endl
                       << "and ensure that plug-ins are defined for initial loading of Wintermute." << endl << endl
                       << "*** Reset to default plug-in list.";
            //settings->setValue("Plugins/AutoStart",
            //"a9b6b020-f4f2-11e0-be50-0800200c9a66, 81da3bd6-bed5-4c74-aae6-44f48cd5330c, 6d2a54ae-043d-11e1-b46a-93253d2b7d89, 5f0741d0-026b-11e1-8f4e-5999c633b9c0");
        }

        emit Factory::instance()->started();
        qDebug() << "(core) [Factory] Started.";

    }
    else {   // daemonMode
        qDebug() << "(core] [Factory] Executing in daemon mode. Plug-ins are loaded at will.";
        emit Factory::instance()->started();
    }
}

AbstractPlugin* Factory::loadPlugin (const QString& p_uuid)
{
    if (IPC::System::module () == "plugin") {
        AbstractPlugin* plgnBase = 0;

        // Is the plug-in enabled?
        if (!Factory::attribute (p_uuid, "Plugin/Enabled").toBool ()) {
            qWarning() << "(plugin) [Factory] Plugin" << p_uuid << "disabled.";
            Core::exit (1, true);
            return 0;
        }

        // Check if this shouldn't be loaded in this module but rather in it's specified API module.
        if (loadBackendPlugin (p_uuid))
            return 0;

        const ShellPlugin* gnrcPlgn = new ShellPlugin (p_uuid);

        // Is everything needed for this plug-in available?
        if (!gnrcPlgn->loadRequiredComponents())
            return 0;

        if (!gnrcPlgn->loadLibrary()) {
            qWarning() << "(plugin) [Factory] Error loading plug-in" << gnrcPlgn->name();

            if (gnrcPlgn->d_func()->m_plgnLdr)
                qDebug() << "(plugin) [Factory] Library loading error: " << gnrcPlgn->d_func()->m_plgnLdr->errorString();

            emit Factory::instance()->pluginCrashed (p_uuid);
            return 0;
        }

        plgnBase = gnrcPlgn->obtainInstance();

        if (plgnBase) {
            plgnBase->d_func()->m_plgnLdr = gnrcPlgn->d_func()->m_plgnLdr;
            plgnBase->d_func()->loadSettings (p_uuid);
            plgnBase->d_func()->doStart();
            instance()->d_func()->s_rtPlgn = plgnBase;
            emit Factory::instance ()->pluginLoaded (plgnBase->uuid ());
            qDebug() << "(plugin) [Factory] Plug-in" << plgnBase->name () << "loaded.";
            return plgnBase;
        }
        else {
            qFatal ("(plugin) [Factory] Plug-in object could not be pulled from loaded library.");
            return 0;
        }
    }
    else {
        qDebug() << "Inserting plug-in" << p_uuid << "into the pool.";
        PluginHandle* inst = new PluginHandle (p_uuid, Factory::getPluginSettings (p_uuid));
        instance()->d_func()->m_plgnPool.insert (p_uuid, inst);
    }

    return 0;
}

QStringList Factory::loadedPlugins ()
{
    QStringList uuids;

    foreach (PluginHandle * instance, instance()->d_func()->m_plgnPool) {
        uuids << instance->uuid();
    }

    return uuids;
}

QStringList Factory::allPlugins ()
{
    QDir dir (WNTR_PLUGINSPEC_PATH);
    dir.setFilter (QDir::Files | QDir::Readable | QDir::NoSymLinks);
    dir.setNameFilters (QString ("*.spec").split (" "));
    dir.setSorting (QDir::Name);
    return dir.entryList().replaceInStrings (".spec", "");
}

void Factory::unloadPlugin (const QString& p_uuid)
{
    doPluginUnload (p_uuid);

    //qDebug() << Factory::PluginHandle ()->m_plgnPool.count ();
    if (instance()->d_func()->m_plgnPool.count () == 0) {
        qWarning() << "(core) [Factory] No plug-ins running; exitting..";
        Core::quit();
    }
}

void Factory::loadStandardPlugin()
{
    const QString plgnUuid = Core::arguments().value ("plugin").toString();

    qDebug() << "(core) [Factory] Generating plug-in" << Factory::attribute (plgnUuid, "Description/Name").toString() << "UUID:" << plgnUuid << "...";

    instance()->d_func()->s_rtPlgn = loadPlugin (plgnUuid);

    if (instance()->d_func()->s_rtPlgn) {
        PluginHandleAdaptor* adpt = new PluginHandleAdaptor (instance()->d_func()->s_rtPlgn);
        IPC::System::registerObject ("/Plugin", adpt);
        IPC::System::instance()->m_adapt = adpt;
    }
    else {
        qDebug() << "(core) [Factory] Unable to load standard plug-in; exiting.";
        Core::exit();
    }
}

AbstractPlugin* Factory::currentPlugin()
{
    if (IPC::System::module() == "plugin")
        return instance()->d_func()->s_rtPlgn;
    else {
        qWarning() << "(core) [Factory] This isn't a plug-in PluginHandle; can't find the current plug-in!";
        return 0;
    }
}

void Factory::unloadStandardPlugin ()
{
    const QString plgnUuid = Core::arguments ().value ("plugin").toString ();
    qDebug() << "(core) [Factory] Removing plug-in" << plgnUuid << "...";
    unloadPlugin (plgnUuid);
}

void Factory::doPluginCrash (const QString& p_plgnUuid)
{
    qDebug() << "(core) [Factory] Plug-in" << p_plgnUuid << "crashed.";
    doPluginUnload (p_plgnUuid);
    emit instance()->pluginCrashed (p_plgnUuid);
}

void Factory::doPluginLoad (const QString& p_plgName)
{
    qDebug() << "(core) [Factory] Plug-in" << p_plgName << "fully loaded.";
    emit instance()->pluginLoaded (p_plgName);
}

void Factory::doPluginUnload (const QString& p_plgnUuid)
{
    if (loadedPlugins ().contains (p_plgnUuid)) {
        PluginHandle* inst = instance()->d_func()->m_plgnPool.take (p_plgnUuid);

        if (inst) {
            inst->stop ();
            emit instance()->pluginUnloaded (p_plgnUuid);
            qDebug() << "(core) [Factory] Plug-in" << attribute (p_plgnUuid, "Description/Name").toString () << "unloaded.";
        }
    }
}

/// @todo Unload every loaded plugin and free all resources using signals (we have the process, what's the point? just kill them.)
void Factory::shutdown ()
{
    qDebug() << "(core) [Factory] Unloading plugins..";

    foreach (PluginHandle * inst, instance()->d_func()->m_plgnPool) {
        unloadPlugin (inst->uuid ());
    }

    qDebug() << "(core) [Factory] Plugins unloaded.";
}

QSettings* Factory::getPluginSettings (const QString& p_plgnUuid)
{
    const QString plgnSpecPath = QString (WNTR_PLUGINSPEC_PATH) + "/" + p_plgnUuid + ".spec";

    if (!QFile::exists (plgnSpecPath)) {
        qWarning() << "(core) [Factory]" << plgnSpecPath << "does not exist.";
        QSettings* newSettings = new QSettings;
        newSettings->setValue ("Misc/HaveSpec", QVariant (false));
        //Factory::pluginSettings.insert(p_plgnUuid, newSettings);
        return NULL;
    }

    QSettings* newSettings = new QSettings (plgnSpecPath, QSettings::IniFormat, Factory::instance());
    //Factory::pluginSettings.insert(p_plgnUuid, newSettings);
    return newSettings;
}

QVariant Factory::attribute (const QString& p_uuid, const QString& p_attributePath)
{
    QSettings* plgnSpec = getPluginSettings (p_uuid);

    if (plgnSpec)
        return plgnSpec->value (p_attributePath);
    else
        return QVariant();
}

AbstractPlugin* Factory::obtainPlugin (const QString& p_uuid)
{
    QHash<const QString, AbstractPlugin*> table = instance()->d_func()->s_plgnLst;

    if (table.contains (p_uuid)) {
        return table.value (p_uuid);
    }

    return 0;
}

void Factory::setAttribute (const QString& p_uuid, const QString& p_attributePath, const QVariant& p_attributeValue)
{
    QSettings* plgnSpec = getPluginSettings (p_uuid);

    if (plgnSpec)
        plgnSpec->setValue (p_attributePath, p_attributeValue);
}

bool Factory::loadBackendPlugin (const QString& p_uuid)
{
    const QString apiUuid = Factory::attribute (p_uuid, "Plugin/API").toString();
    const QString plgnTyp = Factory::attribute (p_uuid, "Plugin/Type").toString();

    if (plgnTyp == "Backend" && currentPlugin() && currentPlugin()->uuid() != apiUuid) {
        const QDBusMessage callRunningList = QDBusMessage::createMethodCall (WNTR_DBUS_FACTORY_NAME, WNTR_DBUS_FACTORY_OBJNAME, WNTR_DBUS_FACTORY_OBJPATH, "loadedPlugins");
        const QDBusMessage replyRunningList = QDBusConnection::sessionBus ().call (callRunningList, QDBus::BlockWithGui);

        if (replyRunningList.arguments().at (0).toStringList().contains (apiUuid)) {
            QDBusMessage callLoadBackend = QDBusMessage::createMethodCall (QString (WNTR_DBUS_PLUGIN_NAME) + "." + apiUuid, WNTR_DBUS_PLUGIN_OBJNAME, WNTR_DBUS_PLUGIN_OBJPATH, "loadBackend");
            callLoadBackend << p_uuid;
            const QDBusMessage replyLoadBackend = QDBusConnection::sessionBus ().call (callLoadBackend, QDBus::BlockWithGui);

            if (!replyLoadBackend.arguments().at (0).toBool())
                qDebug() << "(plugin) [Factory] Invoking load of back-end" << attribute (p_uuid, "Description/Name").toString() << "to API" << attribute (apiUuid, "Description/Name").toString() << "failed.";
            else
                qDebug() << "(plugin) [Factory] Invoking load of back-end" << attribute (p_uuid, "Description/Name").toString() << "to API" << attribute (apiUuid, "Description/Name").toString() << "succeeded.";

        }
        else
            qDebug() << "(plugin) [Factory] API" << attribute (apiUuid, "Description/Name").toString() << "isn't running for back-end" << attribute (p_uuid, "Description/Name").toString();

        Core::exit();
        return true;
    }

    return false;
}

#include "factory.hpp.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
