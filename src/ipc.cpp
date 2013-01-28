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

#include <QApplication>
#include <QProcess>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusAbstractAdaptor>

#include "global.hpp"
#include "factory.hpp"
#include "adaptors.hpp"
#include "core.hpp"
#include "ipc.hpp"
#include "ipcprivate.hpp"

WINTER_USE_NAMESPACE
WINTER_SINGLETON_DEFINE (IPC)

IPCPrivate::IPCPrivate (IPC* p_qPtr) : q_ptr (p_qPtr), connection (0), adaptor (0)
{

}

IPCPrivate::~IPCPrivate()
{
    adaptor->deleteLater();
    connection->disconnectFromPeer ("Wintermute");
    connection->disconnectFromBus ("Wintermute");
}

IPC::IPC() : QObject (qApp->instance()), d_ptr (new IPCPrivate (this))
{

}

const QString IPC::module()
{
    return Core::arguments().value (WINTER_COMMAND_LINE_IPC).toString().toLower();
}

void IPC::start()
{
    instance()->d_func()->connection = new QDBusConnection (QDBusConnection::sessionBus().connectToBus (QDBusConnection::SessionBus, "Wintermute"));
    bool serviceRegistered = false;
    const QString plugin = Core::arguments().value (WINTER_COMMAND_LINE_FACTORY).toString();
    QString serviceName = WINTER_DBUS_SERVICE_NAME;
    QString objectName;

    qDebug() << "(core) [IPC::start()]" << "Wintermute's operating in" << module() << "mode.";

    if (module() == WINTER_COMMAND_LINE_IPC_CORE) {
        objectName = "master";
        QObject::connect (Core::instance(), SIGNAL (started()), Factory::instance(), SLOT (startup()));
        QObject::connect (Core::instance(), SIGNAL (stopped()), Factory::instance(), SLOT (shutdown()));

        CoreAdaptor* coreAdaptor = new CoreAdaptor;
        FactoryAdaptor* pluginFactoryAdaptor = new FactoryAdaptor;

        registerObject (WINTER_DBUS_MASTER_OBJNAME, coreAdaptor);
        registerObject (WINTER_DBUS_FACTORY_OBJNAME, pluginFactoryAdaptor);
        instance()->d_func()->adaptor = coreAdaptor;
    }
    else if (module() == WINTER_COMMAND_LINE_IPC_PLUGIN) {
        objectName = "Plugin." + plugin;
        QObject::connect (Core::instance(), SIGNAL (started()), Factory::instance(), SLOT (loadStandardPlugin()));
        QObject::connect (Core::instance(), SIGNAL (stopped()), Factory::instance(), SLOT (unloadStandardPlugin()));
    }

    if (objectName != "master") serviceName += "." + objectName;

    serviceRegistered = bus()->registerService (serviceName);

    if (!serviceRegistered && objectName == "master") {
        qDebug() << "(core) Fatal: Cannot run more than one Wintermute service (" << serviceName << ")"
                 << "under the same user on the same computer";
        Core::exit (WINTER_EXITCODE_DBUS | WINTER_EXITCODE_NORMAL);
    }

    if (serviceRegistered)
        qDebug() << "(core) [D-Bus] Service" << serviceName << "running.";

    emit instance()->registerDataTypes();
}

bool IPC::registerObject (const QString& p_path, QDBusAbstractAdaptor* p_adaptor)
{
    QDBusConnection::RegisterOptions opts = QDBusConnection::ExportAllContents
                                            | QDBusConnection::ExportAllSignals
                                            | QDBusConnection::ExportAllSlots
                                            | QDBusConnection::ExportAllProperties
                                            | QDBusConnection::ExportAllInvokables
                                            | QDBusConnection::ExportChildObjects
                                            | QDBusConnection::ExportAdaptors;

    if (bus()->objectRegisteredAt (p_path)) {
        qDebug() << "(core) [D-Bus] Object" << p_path << "already registered on" << bus()->interface ()->service ();
        return false;
    }
    else {
        qDebug() << "(core) [D-Bus] Registered" << p_path << "on"
                 << bus()->interface ()->connection().name() << "with"
                 << p_adaptor->metaObject()->className() << ".";
        return bus()->registerObject (p_path , p_adaptor, opts);
    }

    return false;
}

void IPC::stop ()
{
    bus()->disconnectFromBus (bus()->name());
}

QDBusConnection* IPC::bus()
{
    return instance()->d_func()->connection;
}

AbstractAdaptor* IPC::adaptor ()
{
    return instance()->d_func()->adaptor;
}

void IPC::setAdaptor (AbstractAdaptor* p_adaptor)
{
    if (instance()->d_func()->adaptor == 0) {
        instance()->d_func()->adaptor = p_adaptor;
    }
}

void IPC::handleExit()
{
    if ( (module () != "master" && Core::arguments ().value ("help") == "ignore")) {
        qDebug() << "(core) [" << module () << "] Closing root appplication...";
        QDBusMessage msg = QDBusMessage::createMethodCall ("org.thesii.Wintermute", "/Master", "org.thesii.Wintermute.Master", "quit");
        QDBusMessage reply = bus ()->call (msg, QDBus::Block);

        if (reply.type () == QDBusMessage::ErrorMessage)
            qDebug() << "(core) [" << module () << "] Can't terminate master module of Wintermute:" << reply.errorName();
    }
}

IPC::~IPC()
{
    d_ptr.reset();
}

#include "ipc.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
