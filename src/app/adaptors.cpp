/**
 * @file    adaptors.cpp
 * @author  Wintermute Development <wntr-devel@thesii.org>
 */
/*
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute Linguistics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute Linguistics; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

// Local
#include "adaptors.hxx"
#include "adaptors.hpp"
#include "core.hpp"
#include "backend.hpp"
#include "ipc.hpp"

#include "plugin.hpp"
#include "factory.hpp"
#include "pluginhandle.hpp"

// Qt
#include <QTimer>
#include <QtDebug>
#include <QApplication>
#include <QDBusConnection>

namespace Wintermute
{
namespace IPC
{
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
} // namespace

namespace Plugins
{
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

} // namespace

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
} // namespace

#include "adaptors.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
