/***
 *  This file is part of the Wintermute project.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
 *
 *  Wintermute is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  Wintermute is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with Wintermute.
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 04/22/12 5:41:48 AM
 */

#include <QtDBus>
#include <QProcess>
#include "ipc.hpp"
#include "core.hpp"
#include "config.hpp"
#include "adaptors.hpp"
#include "factory.hpp"

namespace Wintermute
{
namespace IPC
{
System* System::s_inst = new System;

System::System (QObject* p_obj) : QObject (p_obj) { }

System::~System() { }

void System::start()
{
    instance()->m_cnntn = new QDBusConnection (QDBusConnection::sessionBus().connectToBus (QDBusConnection::SessionBus, "Wintermute"));
    instance()->m_appMod = Core::arguments().value (WINTER_COMMAND_LINE_IPC).toString().toLower();
    bool serviceRegistered = false;
    const QString plgn = Core::arguments().value (WINTER_COMMAND_LINE_FACTORY).toString();
    QString serviceName = WINTER_DBUS_SERVICE_NAME;
    QString objectName;
    qDebug() << "(core) [IPC::System]" << "Mode:" << instance()->m_appMod;

    if (instance()->m_appMod == WINTER_COMMAND_LINE_IPC_CORE) {
        objectName = "master";
        connect (Core::instance(), SIGNAL (started()), Plugins::Factory::instance(), SLOT (startup()));
        connect (Core::instance(), SIGNAL (stopped()), Plugins::Factory::instance(), SLOT (shutdown()));

        CoreAdaptor* coreAdaptor = new CoreAdaptor;
        Plugins::PluginFactoryAdaptor* pluginFactoryAdaptor = new Plugins::PluginFactoryAdaptor;

        registerObject ("/Master", coreAdaptor);
        registerObject ("/Factory", pluginFactoryAdaptor);
        instance()->m_adapt = coreAdaptor;
    }
    else if (instance()->m_appMod == WINTER_COMMAND_LINE_IPC_PLUGIN) {
        objectName = "Plugin." + plgn;
        connect (Core::instance(), SIGNAL (started()), Plugins::Factory::instance(), SLOT (loadStandardPlugin()));
        connect (Core::instance(), SIGNAL (stopped()), Plugins::Factory::instance(), SLOT (unloadStandardPlugin()));
    }

    if (objectName != "master") serviceName += "." + objectName;

    if (instance()->m_cnntn->registerService (serviceName))
        serviceRegistered = true;

    if (!serviceRegistered && objectName == "master") {
        qDebug() << "(core) Fatal: Cannot run more than one Wintermute service (" << serviceName << ")"
                 << "under the same user on the same computer";
        Core::exit (1);
    }

    if (serviceRegistered)
        qDebug() << "(core) [D-Bus] Service" << serviceName << "running.";

    emit instance()->registerDataTypes();
}

/// @todo Find a way to expose adaptors _properly_ over D-Bus.
bool System::registerObject (const QString& p_pth, QDBusAbstractAdaptor* p_obj)
{
    QDBusConnection::RegisterOptions opts = QDBusConnection::ExportAllContents
                                            | QDBusConnection::ExportAllSignals
                                            | QDBusConnection::ExportAllSlots
                                            | QDBusConnection::ExportAllProperties
                                            | QDBusConnection::ExportAllInvokables
                                            | QDBusConnection::ExportChildObjects
                                            | QDBusConnection::ExportAdaptors;

    if (instance()->m_cnntn->objectRegisteredAt (p_pth)) {
        qDebug() << "(core) [D-Bus] Object" << p_pth << "already registered on" << instance()->m_cnntn->interface ()->service ();
        return false;
    }
    else {
        qDebug() << "(core) [D-Bus] Registered" << p_pth << "on" << instance()->m_cnntn->interface ()->connection().name() << "with" << p_obj->metaObject()->className() << ".";
        return instance()->m_cnntn->registerObject (p_pth , p_obj, opts);
    }

    return false;
}

void System::stop ()
{
    instance()->m_cnntn->disconnectFromBus (instance()->m_cnntn->name());
}

QDBusConnection* System::bus()
{
    return instance()->m_cnntn;
}

AbstractAdaptor* System::adaptor ()
{
    return instance()->m_adapt;
}

System* System::instance()
{
    return s_inst;
}
}
}

#include "ipc.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
