/**
 * @author Wintermute Development <wntr-devel@thesii.org>
 * @section lcns Licensing
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
 *
 */

#include "wntrntwk.hpp"
#include "adaptors.hpp"
#include <QDebug>
#include <QObject>
#include <QtPlugin>
#include <QtDBus/QDBusMetaType>
#include <ipc.hpp>
#include <core.hpp>

using namespace Wintermute;
using namespace Wintermute::Network;

namespace Wintermute
{
namespace Network
{
Interface* Interface::s_ints = 0;

Interface::Interface()
{
    Interface::s_ints = this;
}

Interface* Interface::instance()
{
    if (s_ints == 0) {
        s_ints = new Interface;
        qDebug() << "(ntwk) [Interface] Created core interface.";
    }

    return s_ints;
}

Interface::~Interface()
{
    qDebug() << "(ntwk) [Interface] Destroyed core interface.";
}

void Interface::start() const
{
    System::start();
    Broadcast::load ();

    emit started();
}

void Interface::stop() const
{
    System::stop ();
    Broadcast::unload ();
    emit stopped();
}

const bool Interface::isActive()
{
    return System::isActive();
}

void Plugin::start () const
{
    connect (this, SIGNAL (started()), Wintermute::Network::Interface::instance (), SLOT (start()));
    connect (this, SIGNAL (stopped()), Wintermute::Network::Interface::instance (), SLOT (stop()));

    qDBusRegisterMetaType<Network::Message>();

    Network::SystemAdaptor* adpt = new Network::SystemAdaptor;
    Network::BroadcastAdaptor* adpt2 = new Network::BroadcastAdaptor;

    if (!Wintermute::IPC::System::registerObject ("/System",    adpt) ||
            !Wintermute::IPC::System::registerObject ("/Broadcast", adpt2))
        qDebug() << "(ntwk) [Plugin] Failed to register D-Bus objects.";
}

void Plugin::stop () const { }

QObject* Plugin::instance () const
{
    return System::instance();
}
}
}

Q_EXPORT_PLUGIN2 (WntrNtwk, Wintermute::Network::Plugin)

#include "wntrntwk.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
