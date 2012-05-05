/***
 *  This file is part of the Wintermute project.
 *
 *  Copyright (C) 2012 Jacky Alciné <jacky.alcine@thesii.org>
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
 *  License along with Wintermute .
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jacky.alcine@thesii.org>
 * @date 04/18/12 4:16:41 PM
 */

#include <QTimer>
#include <QtDebug>
#include <QApplication>
#include <QDBusMessage>
#include <QDBusConnection>

#include "adaptorsprivate.hpp"
#include "adaptors.hpp"
#include "core.hpp"
#include "backend.hpp"
#include "ipc.hpp"
#include "plugin.hpp"
#include "factory.hpp"
#include "pluginhandle.hpp"
#include "global.hpp"

WINTER_USE_NAMESPACE

AbstractAdaptorPrivate::AbstractAdaptorPrivate (AbstractAdaptor* p_qPtr) : q_ptr (p_qPtr)
{

}

void AbstractAdaptorPrivate::detect() const
{
    Q_Q (const AbstractAdaptor);

    timer->stop();
    const bool prv = isCore;
    QDBusMessage ping = QDBusMessage::createMethodCall (WINTER_DBUS_SERVICE_NAME, WINTER_DBUS_MASTER_OBJNAME, WINTER_DBUS_MASTER_NAME, "ping");
    ping << IPC::module ();
    ping.setAutoStartService (true);

    QDBusMessage pingReply = IPC::bus ()->call (ping, QDBus::BlockWithGui);
    isCore = pingReply.type () != QDBusMessage::ErrorMessage;

    if (isCore != prv) {
        if (isCore) {
            qDebug() << "(core) [D-Bus] Core module found.";
            emit q->coreModuleLoaded ();
        }
        else {
            qDebug() << "(core) [D-Bus] Core module lost.";
            emit q->coreModuleUnloaded ();
        }
    }

#if WINTER_DBUS_VERBOSE_PINGS

    if (pingReply.type () == QDBusMessage::ErrorMessage) {
        qDebug() << "(core) [D-Bus] Pong from core module:" << pingReply.errorMessage ();

        if (!Core::arguments().value ("daemon").toBool ())
            CoreAdaptor::haltSystem ();
    }

#endif

    timer->start ();
}

AbstractAdaptorPrivate::~AbstractAdaptorPrivate()
{
    timer->stop();
}

AbstractAdaptor::AbstractAdaptor (QObject* p_parent) : QDBusAbstractAdaptor (p_parent),
    d_ptr (new AbstractAdaptorPrivate (this))
{
    Q_D (AbstractAdaptor);

    if (IPC::module() != "master") {
        d->timer = new QTimer (this);
        d->detect();
        connect (d->timer, SIGNAL (timeout()), this, SLOT (doDetect()));
        d->timer->setInterval (WINTER_DBUS_UPDATE_TIMEOUT);
        d->timer->start();
    }
}

void AbstractAdaptor::doDetect()
{
    Q_D (AbstractAdaptor);
    d->detect();
}

int AbstractAdaptor::pid() const
{
    return QApplication::applicationPid();
}

const QString AbstractAdaptor::module() const
{
    return IPC::module();
}

FactoryAdaptor::FactoryAdaptor() : AbstractAdaptor (Factory::instance())
{
    setAutoRelaySignals (true);
}

void FactoryAdaptor::loadPlugin (const QString& p_plgnName)
{
    if (!p_plgnName.isEmpty() && !p_plgnName.isNull()) {
        Factory::instance()->loadPlugin (p_plgnName);
    }
}

void FactoryAdaptor::unloadPlugin (const QString& p_plgnName)
{
    if (!p_plgnName.isEmpty() && !p_plgnName.isNull()) {
        Factory::instance()->unloadPlugin (p_plgnName);
    }
}

QStringList FactoryAdaptor::allPlugins() const
{
    return Factory::instance()->allPlugins();
}

QStringList FactoryAdaptor::loadedPlugins() const
{
    return Factory::instance()->loadedPlugins();
}
void FactoryAdaptor::quit() const
{
    emit aboutToQuit();
    Factory::shutdown();
}

PluginAdaptor::PluginAdaptor (AbstractPlugin* p_plgn) : AbstractAdaptor (Core::instance())
{
    if (p_plgn == 0) {
        emit pluginCantLoad (Core::arguments().value ("plugin").toString ());
        QApplication::quit ();
    }
    else {
        connect (QApplication::instance (), SIGNAL (aboutToQuit()), this, SIGNAL (aboutToQuit()));
        QDBusConnection::sessionBus ().connect (WINTER_DBUS_SERVICE_NAME, WINTER_DBUS_FACTORY_OBJNAME,
                                                WINTER_DBUS_FACTORY_NAME, "pluginCantLoad",
                                                this, SIGNAL (pluginCantLoad (QString)));
        QDBusConnection::sessionBus ().connect (WINTER_DBUS_SERVICE_NAME, WINTER_DBUS_FACTORY_OBJNAME,
                                                WINTER_DBUS_FACTORY_NAME, "pluginLoaded",
                                                this, SIGNAL (pluginLoaded (QString)));
        QDBusConnection::sessionBus ().connect (WINTER_DBUS_SERVICE_NAME, WINTER_DBUS_FACTORY_OBJNAME,
                                                WINTER_DBUS_FACTORY_NAME, "pluginUnloaded",
                                                this, SIGNAL (pluginUnloaded (QString)));
        QDBusConnection::sessionBus ().connect (WINTER_DBUS_SERVICE_NAME, WINTER_DBUS_FACTORY_OBJNAME,
                                                WINTER_DBUS_FACTORY_NAME, "aboutToQuit",
                                                this, SLOT (aboutToQuit()));

        setParent (p_plgn);
        setAutoRelaySignals (true);
    }
}

void PluginAdaptor::quit () const
{
    AbstractPlugin* plgn = qobject_cast<AbstractPlugin*> (parent());
    emit aboutToQuit ();
    Factory::unloadPlugin(plgn->uuid());
    //plgn->stop();
    emit pluginUnloaded (plgn->uuid());
}

void PluginAdaptor::loadBackend (const QString& p_uuid)
{
    if (!p_uuid.isEmpty() && !p_uuid.isNull()) {
        AbstractPlugin* plgn = qobject_cast<AbstractPlugin*> (parent());
        AbstractFramework* frmk = AbstractFramework::obtainFramework (plgn->uuid());
        frmk->isBackendListed (p_uuid);
    }
}

CoreAdaptor::CoreAdaptor() : AbstractAdaptor (Core::instance())
{

}

const QVariantMap CoreAdaptor::arguments() const
{
    return Core::arguments();
}

void CoreAdaptor::ping (const QString& p_src)
{
    (p_src.length() == 0) + 0;
#if WINTER_DBUS_VERBOSE_PINGS
    qDebug() << "(core) [D-Bus] Ping from object" << p_src << "received.";
#endif
}

void CoreAdaptor::quit () const
{
    emit aboutToQuit ();
    QDBusMessage msg = QDBusMessage::createMethodCall (WINTER_DBUS_SERVICE_NAME, WINTER_DBUS_FACTORY_OBJNAME, WINTER_DBUS_FACTORY_NAME, "quit");
    QDBusConnection::sessionBus ().call (msg, QDBus::NoBlock);
    haltSystem ();
}

void CoreAdaptor::haltSystem ()
{
    if (Core::arguments().value ("ipc").toString () != "master") {
        QDBusMessage call = QDBusMessage::createMethodCall (WINTER_DBUS_SERVICE_NAME, WINTER_DBUS_MASTER_OBJNAME, WINTER_DBUS_MASTER_NAME, "haltSystem");
        QDBusConnection::sessionBus ().send (call);
    }

    QApplication::quit ();
}

#include "adaptors.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
