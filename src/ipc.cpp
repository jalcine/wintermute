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
 * @author Jacky Alciné <jacky.alcine@thesii.org>
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
    connection->disconnectFromPeer ( WINTER_DBUS_CONNECTION );
    connection->disconnectFromBus ( WINTER_DBUS_CONNECTION );
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
    // ** Grab a new QDBusConnection for this application.
    instance()->d_func()->connection = new QDBusConnection (QDBusConnection::sessionBus().connectToBus (QDBusConnection::SessionBus, WINTER_DBUS_CONNECTION));
    bool serviceRegistered = false;
    const QString plugin = Core::arguments().value (WINTER_COMMAND_LINE_FACTORY).toString();
    QString serviceName = WINTER_DBUS_SERVICE_NAME;
    QString objectName;

    qDebug() << "(core) [IPC::start()]" << "Wintermute's operating in" << module() << "mode.";

    // *** Determine what module Wintermute will be running as.
    if (module() == WINTER_COMMAND_LINE_IPC_CORE) {
        // *** We want to run as the central process, set up shop.
        objectName = WINTER_DBUS_MASTER;
        QObject::connect (Core::instance(), SIGNAL (started()), Factory::instance(), SLOT (startup()));
        QObject::connect (Core::instance(), SIGNAL (stopped()), Factory::instance(), SLOT (shutdown()));

        CoreAdaptor* coreAdaptor = new CoreAdaptor;
        FactoryAdaptor* pluginFactoryAdaptor = new FactoryAdaptor;

        registerObject (WINTER_DBUS_MASTER, coreAdaptor);
        registerObject (WINTER_DBUS_FACTORY, pluginFactoryAdaptor);
        instance()->d_func()->adaptor = coreAdaptor;
    }
    else if (module() == WINTER_COMMAND_LINE_IPC_PLUGIN) {
        /// *** We're running as a plug-in, batten down the hatches.
        objectName = WINTER_DBUS_PLUGIN "." + plugin;
        QObject::connect (Core::instance(), SIGNAL (started()), Factory::instance(), SLOT (loadStandardPlugin()));
        QObject::connect (Core::instance(), SIGNAL (stopped()), Factory::instance(), SLOT (unloadStandardPlugin()));
    }


    if (objectName != WINTER_DBUS_MASTER) serviceName += "." + objectName;

    /// *** Attempt to register this application into the current D-Bus session.
    serviceRegistered = bus()->registerService (serviceName);

    if (!serviceRegistered && objectName == WINTER_DBUS_MASTER) {
        /// *** @TODO: Attempt to issue a signal to the real core about this failure.
        qDebug() << "(core) Fatal: Cannot run more than one Wintermute service (" << serviceName << ")"
                 << "under the same user on the same computer";
        Core::exit (WINTER_EXITCODE_DBUS | WINTER_EXITCODE_NORMAL);
    }

    if (serviceRegistered){
        qDebug() << "(core) [D-Bus] Service" << serviceName << "running.";
        emit instance()->registerDataTypes();
    }
}

bool IPC::registerObject (const QString& p_path, QDBusAbstractAdaptor* p_adaptor)
{
    /// *** Define the options for QtDBus we'd like to use.
    QDBusConnection::RegisterOptions opts = QDBusConnection::ExportAllContents
                                            | QDBusConnection::ExportAllSignals
                                            | QDBusConnection::ExportAllSlots
                                            | QDBusConnection::ExportAllProperties
                                            | QDBusConnection::ExportAllInvokables
                                            | QDBusConnection::ExportChildObjects
                                            | QDBusConnection::ExportAdaptors;

    if (bus()->objectRegisteredAt (WINTER_DBUS_SERVICE_NAME "."+ p_path)) {
        /// @TODO: Inform the core instance about this failure.
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

void IPC::setLocalAdaptor (AbstractAdaptor* p_adaptor)
{
    if (instance()->d_func()->adaptor == 0) {
        instance()->d_func()->adaptor = p_adaptor;
    }
}

QDBusMessage IPC::call(const QString& p_modulePath, const QString& p_moduleClass, const QString& p_method){
    QDBusMessage msg = QDBusMessage::createMethodCall (WINTER_DBUS_SERVICE_NAME,p_modulePath,p_moduleClass,p_method);
    return bus()->call (msg,QDBus::Block);
}

void IPC::handleExit()
{
    if (module () != "master" && Core::arguments ().value ("help") == "ignore") {
        /// *** Issue a request to the core module to take the system down.
        qDebug() << "(core) [" << module () << "] Closing root appplication...";
        QDBusMessage msg = QDBusMessage::createMethodCall (WINTER_DBUS_SERVICE_NAME, "/" WINTER_DBUS_MASTER, WINTER_DBUS_SERVICE_NAME "." WINTER_DBUS_MASTER, "quit");
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
