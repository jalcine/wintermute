/***
 *  This file is part of the Wintermute project.
 *
 *  Copyright (C) 2012 Jacky Alciné <me@jalcine.me>
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
 * @author Jacky Alciné <me@jalcine.me>
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
  connection = new QDBusConnection (QDBusConnection::sessionBus().connectToBus (QDBusConnection::SessionBus, "Wintermute"));
}

QDBusConnection* IPCPrivate::bus()
{
    return connection;
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
    instance()->d_func()->connection = new QDBusConnection (QDBusConnection::sessionBus().connectToBus (QDBusConnection::SessionBus, WINTER_DBUS_CONNECTION));
    bool serviceRegistered = false;
    const QString plugin = Core::arguments().value (WINTER_COMMAND_LINE_FACTORY).toString();
    QString serviceName = WINTER_DBUS_CONNECTION;
    QString objectName;

    qDebug() << "(core) [IPC::start()]" << "Wintermute's operating in" << module() << "mode.";

    // *** Determine what module Wintermute will be running as.
    if (module() == WINTER_COMMAND_LINE_IPC_CORE) {
        // *** We want to run as the central process, set up shop.
        objectName = WINTER_DBUS_MODULE_MASTER;
        QObject::connect (Core::instance(), SIGNAL (started()), Factory::instance(), SLOT (startup()));
        QObject::connect (Core::instance(), SIGNAL (stopped()), Factory::instance(), SLOT (shutdown()));

        CoreAdaptor* coreAdaptor = new CoreAdaptor;
        FactoryAdaptor* pluginFactoryAdaptor = new FactoryAdaptor;

        registerObject (WINTER_DBUS_MODULE_MASTER, coreAdaptor);
        registerObject (WINTER_DBUS_MODULE_FACTORY, pluginFactoryAdaptor);
        instance()->d_func()->adaptor = coreAdaptor;
    }
    else if (module() == WINTER_COMMAND_LINE_IPC_PLUGIN) {
        /// *** We're running as a plug-in, batten down the hatches.
        objectName = WINTER_DBUS_MODULE_PLUGIN "." + plugin;
        QObject::connect (Core::instance(), SIGNAL (started()), Factory::instance(), SLOT (loadStandardPlugin()));
        QObject::connect (Core::instance(), SIGNAL (stopped()), Factory::instance(), SLOT (unloadStandardPlugin()));
    }


    serviceRegistered = instance()->d_func()->bus()->registerService (serviceName);

    /// *** Attempt to register this application into the current D-Bus session.

    if (!serviceRegistered && objectName == WINTER_DBUS_MODULE_MASTER) {
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
    QDBusConnection* bus = instance()->d_func()->bus ();
    QDBusConnection::RegisterOptions opts = QDBusConnection::ExportAllContents
                                            | QDBusConnection::ExportAllSignals
                                            | QDBusConnection::ExportAllSlots
                                            | QDBusConnection::ExportAllProperties
                                            | QDBusConnection::ExportAllInvokables
                                            | QDBusConnection::ExportChildObjects
                                            | QDBusConnection::ExportAdaptors;

    if (bus->objectRegisteredAt (p_path)) {
        qDebug() << "(core) [D-Bus] Object" << p_path << "already registered on" << bus->interface ()->service ();
        return false;
    }
    else {
        qDebug() << "(core) [D-Bus] Registered" << p_path << "on"
                 << bus->interface ()->connection().name() << "with"
                 << p_adaptor->metaObject()->className() << ".";
        return bus->registerObject (p_path , p_adaptor, opts);
    }

    return false;
}

void IPC::stop ()
{
  QDBusConnection* bus = instance()->d_func()->bus();
  bus->disconnectFromBus (bus->name());
}

AbstractAdaptor* IPC::localAdaptor ()
{
    return instance()->d_func()->adaptor;
}

void IPC::setLocalAdaptor (AbstractAdaptor* p_adaptor)
{
    if (instance()->d_func()->adaptor == 0) {
        instance()->d_func()->adaptor = p_adaptor;
    }
}

QDBusMessage IPC::callMethod(const QString& p_module, const QString& p_path, const QString& p_objectPath, const QString& p_method, const QVariantList& p_variables)
{
  QDBusMessage msg, reply;
  msg = QDBusMessage::createMethodCall (p_module, p_path, p_objectPath, p_method);
  msg.setAutoStartService(true);
  if (!p_variables.isEmpty()){
      for (int i = 0; i < p_variables.length (); i++){
          msg << p_variables[i];
      }
  }

  reply = instance()->d_func ()->bus ()->call (msg,QDBus::AutoDetect);

  return reply;
}

void IPC::handleExit()
{
    if (module () != "master" && Core::arguments ().value ("help") == "ignore") {
        /// *** Issue a request to the core module to take the system down.
        qDebug() << "(core) [" << module () << "] Closing root appplication...";
        QDBusMessage reply = IPC::callMethod("org.thesii.Wintermute", "/Master", "org.thesii.Wintermute.Master", "quit",QVariantList());

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
