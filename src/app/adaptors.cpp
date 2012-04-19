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
#include <QDBusConnection>

#include "adaptorsprivate.hpp"
#include "adaptors.hpp"
#include "core.hpp"
#include "backend.hpp"
#include "ipc.hpp"

#include "plugin.hpp"
#include "factory.hpp"
#include "pluginhandle.hpp"

using namespace Wintermute;
using namespace Wintermute::IPC;
using namespace Wintermute::Plugins;

GenericAdaptorPrivate::GenericAdaptorPrivate (GenericAdaptor* p_qPtr) : q_ptr (p_qPtr)
{

}

void GenericAdaptorPrivate::detect() const
{
    Q_Q (const GenericAdaptor);
    m_tmr->stop();
    const bool prv = m_core;
    QDBusMessage ping = QDBusMessage::createMethodCall (WNTR_DBUS_SERVICE_NAME, "/Master", WNTR_DBUS_MASTER_NAME, "ping");
    ping << System::module ();
    ping.setAutoStartService (true);
    QDBusMessage pingReply = IPC::System::bus ()->call (ping, QDBus::BlockWithGui);
    m_core = pingReply.type () != QDBusMessage::ErrorMessage;

    if (m_core != prv) {
        if (m_core) {
            qDebug() << "(core) [D-Bus] Core module found.";
            emit q->coreModuleLoaded ();
        }
        else {
            qDebug() << "(core) [D-Bus] Core module lost.";
            emit q->coreModuleUnloaded ();
        }
    }

    if (pingReply.type () == QDBusMessage::ErrorMessage) {
        //qDebug() << "(core) [D-Bus] Pong from core module:" << pingReply.errorMessage ();
        /*if (!Core::arguments().value ("daemon").toBool ())
         *            CoreAdaptor::haltSystem ();*/
    }

    m_tmr->start ();
}

GenericAdaptorPrivate::~GenericAdaptorPrivate()
{

}

GenericAdaptor::GenericAdaptor (QObject* p_parent) : QDBusAbstractAdaptor (p_parent),
    d_ptr (new GenericAdaptorPrivate (this))
{
    Q_D (GenericAdaptor);

    if (IPC::System::module() != "master") {
        d->m_tmr = new QTimer (this);
        d->detect();
        connect (d->m_tmr, SIGNAL (timeout()), this, SLOT (detect()));
        d->m_tmr->setInterval (1000);
        d->m_tmr->start();
    }
}

int GenericAdaptor::pid() const
{
    return QApplication::applicationPid();
}

const QString GenericAdaptor::module() const
{
    return IPC::System::module();
}

PluginFactoryAdaptor::PluginFactoryAdaptor() : AbstractAdaptor (Factory::instance())
{
    setAutoRelaySignals (true);
}

void PluginFactoryAdaptor::loadPlugin (const QString& p_plgnName)
{
    if (!p_plgnName.isEmpty() && !p_plgnName.isNull()) {
        Factory::instance()->loadPlugin (p_plgnName);
    }
}

void PluginFactoryAdaptor::unloadPlugin (const QString& p_plgnName)
{
    if (!p_plgnName.isEmpty() && !p_plgnName.isNull()) {
        Factory::instance()->unloadPlugin (p_plgnName);
    }
}

const QStringList PluginFactoryAdaptor::allPlugins() const
{
    return Factory::instance()->allPlugins();
}

const QStringList PluginFactoryAdaptor::loadedPlugins() const
{
    return Factory::instance()->loadedPlugins();
}
void PluginFactoryAdaptor::quit() const
{
    emit aboutToQuit();
    Factory::shutdown();
}

PluginHandleAdaptor::PluginHandleAdaptor (AbstractPlugin* p_plgn) : AbstractAdaptor (Core::instance())
{
    if (p_plgn == 0) {
        emit pluginCantLoad (Core::arguments().value ("plugin").toString ());
        QApplication::quit ();
    }
    else {
        connect (QApplication::instance (), SIGNAL (aboutToQuit()), this, SIGNAL (aboutToQuit()));
        QDBusConnection::sessionBus ().connect (WNTR_DBUS_SERVICE_NAME, "/Factory",
                                                WNTR_DBUS_FACTORY_NAME, "pluginCantLoad",
                                                this, SIGNAL (pluginCantLoad (QString)));
        QDBusConnection::sessionBus ().connect (WNTR_DBUS_SERVICE_NAME, "/Factory",
                                                WNTR_DBUS_FACTORY_NAME, "pluginLoaded",
                                                this, SIGNAL (pluginLoaded (QString)));
        QDBusConnection::sessionBus ().connect (WNTR_DBUS_SERVICE_NAME, "/Factory",
                                                WNTR_DBUS_FACTORY_NAME, "pluginUnloaded",
                                                this, SIGNAL (pluginUnloaded (QString)));

        QDBusConnection::sessionBus ().connect (WNTR_DBUS_SERVICE_NAME, "/Factory",
                                                WNTR_DBUS_FACTORY_NAME, "aboutToQuit",
                                                this, SLOT (aboutToQuit()));

        setParent (p_plgn);
        setAutoRelaySignals (true);
    }
}

void PluginHandleAdaptor::quit () const
{
    AbstractPlugin* plgn = qobject_cast<AbstractPlugin*> (parent());
    emit aboutToQuit ();
    plgn->stop();
    emit pluginUnloaded (plgn->uuid());
}

void PluginHandleAdaptor::loadBackend (const QString& p_uuid)
{
    if (!p_uuid.isEmpty() && !p_uuid.isNull()) {
        AbstractPlugin* plgn = qobject_cast<AbstractPlugin*> (parent());
        Backends::AbstractFramework* frmk = Backends::AbstractFramework::obtainFramework (plgn->uuid());
        frmk->isBackendListed (p_uuid);
    }
}

CoreAdaptor::CoreAdaptor() : AbstractAdaptor (Core::instance()) { }

const QVariantMap CoreAdaptor::arguments() const
{
    return Core::arguments();
}

void CoreAdaptor::ping (const QString& p_src)
{
    (p_src.length() == 0) + 0;
    //qDebug() << "(core) [D-Bus] Ping from object" << p_src << "received.";
}

void CoreAdaptor::quit () const
{
    emit aboutToQuit ();
    QDBusMessage msg = QDBusMessage::createMethodCall (WNTR_DBUS_SERVICE_NAME, "/Factory", WNTR_DBUS_FACTORY_NAME, "quit");
    QDBusConnection::sessionBus ().call (msg, QDBus::NoBlock);
    haltSystem ();
}

void CoreAdaptor::haltSystem ()
{
    if (Core::arguments().value ("ipc").toString () != "master") {
        QDBusMessage call = QDBusMessage::createMethodCall (WNTR_DBUS_SERVICE_NAME, "/Master", WNTR_DBUS_MASTER_NAME, "haltSystem");
        QDBusConnection::sessionBus ().send (call);
    }

    QApplication::quit ();
}

#include "adaptors.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
