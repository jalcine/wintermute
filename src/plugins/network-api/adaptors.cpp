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

#include <QDBusConnection>
#include <app/adaptorsprivate.hpp>
#include <network-api/adaptors.hpp>
#include <network-api/wntrntwk.hpp>

namespace Wintermute
{
namespace Network
{
SystemAdaptor::SystemAdaptor() : AbstractAdaptor (Network::Interface::instance ())
{
    setAutoRelaySignals (true);
    connect (this, SIGNAL (aboutToQuit()), Network::Interface::instance (), SLOT (stop()));
    QDBusConnection::sessionBus().connect ("org.thesii.Wintermute", "/Master",
                                           "org.thesii.Wintermute.Master", "aboutToQuit",
                                           this, SLOT (quit()));
}

const bool SystemAdaptor::isActive () const
{
    //QDBusConnection::sessionBus ().send(p_msg.createReply (Network::Interface::instance ()->isActive ()));
    return Network::Interface::instance()->isActive ();
}

void SystemAdaptor::sendMessage (const Network::Message& p_msg)
{
    QMetaObject::invokeMethod (Network::System::instance(), "send", Q_ARG (Message, p_msg));
}

void SystemAdaptor::quit () const
{
    CoreAdaptor::haltSystem();
    //QDBusConnection::sessionBus ().send(p_msg.createReply(true));
}

BroadcastAdaptor::BroadcastAdaptor() : AbstractAdaptor (Broadcast::instance())
{
    setAutoRelaySignals (true);
    connect (this, SIGNAL (aboutToQuit()), Broadcast::instance(), SLOT (stop()));
}

const bool BroadcastAdaptor::isBroadcasting () const
{
    return Broadcast::isActive ();
}

void BroadcastAdaptor::startBroadcasting ()
{
    if (!Broadcast::isActive ()) {
        Broadcast::start ();
        emit started ();
    }
}

void BroadcastAdaptor::stopBroadcasting ()
{
    if (Broadcast::isActive ()) {
        Broadcast::stop ();
        emit stopped ();
    }
}

void BroadcastAdaptor::sendBroadcast ()
{
    Broadcast::forceSignal ();
}

void BroadcastAdaptor::quit () const
{
    CoreAdaptor::haltSystem();
}
}
}

#include "adaptors.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
