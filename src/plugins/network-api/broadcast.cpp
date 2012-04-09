/**
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

 * @file broadcast.cpp
 * @date April 9, 2011, 1:45 PM
 */

#include "broadcast.hpp"
#include "system.hpp"
#include <QTimer>
#include <QtDebug>

using namespace std;

namespace Wintermute
{
namespace Network
{
Broadcast* Broadcast::s_brdcst = 0;

Broadcast::Broadcast()
{
    Broadcast::s_brdcst = this;
}

Broadcast::~Broadcast()
{
    System::instance()->disconnect (s_brdcst, SLOT (readSignal (Message)));
}

void Broadcast::load()
{
    connect (System::instance (), SIGNAL (messageRecieved (Message)), s_brdcst, SLOT (readSignal (Message)));
    start();
    qDebug() << "(ntwk) [Broadcast] Loaded.";
}

void Broadcast::unload()
{
    stop();
    qDebug() << "(ntwk) [Broadcast] Unloaded.";
}

void Broadcast::start()
{
    if (!Broadcast::isActive ()) {
        s_brdcst->sendSignal ();
        qDebug() << "(ntwk) [Broadcast] Started broadcasting activity.";
    }
}

void Broadcast::stop()
{
    if (Broadcast::isActive ()) {
        s_brdcst->deleteLater ();
        qDebug() << "(ntwk) [Broadcast] Stopped broadcasting activity.";
    }
}

void Broadcast::sendSignal()
{
    qDebug() << "(ntwk) [Broadcast] Attempting to send broadcast...";
    System::send (BroadcastMessage (BroadcastMessage::Online));
    QTimer::singleShot (WNTRNTWK_BROADCAST_INTERVAL, s_brdcst, SLOT (sendSignal()));
}

void Broadcast::readSignal (const Message& p_msg)
{
    qDebug() << "(ntwk) [Broadcast] Attempting to read any messages..";

    if (p_msg.type() == "Broadcast") {
        BroadcastMessage msg (p_msg);

        switch (msg.broadcastType()) {
        case BroadcastMessage::Ping:
            emit s_brdcst->pingReply (msg.property ("Sender").toString ());
            break;
        }
    }
}

void Broadcast::forceSignal ()
{
    System::send ( (BroadcastMessage (BroadcastMessage::Online)));
}

const bool Broadcast::isActive ()
{
    return (s_brdcst != 0);
}

Broadcast* Broadcast::instance ()
{
    if (!s_brdcst) s_brdcst = new Broadcast;

    return s_brdcst;
}

void Broadcast::ping (const QString& p_qualifier)
{
    BroadcastMessage msg (BroadcastMessage::Ping);
    msg.setProperty ("Recipient", p_qualifier);
    System::send (msg);
}

/// @todo Just convert the address into a qualifier and send it to the other version of the method.
void Broadcast::ping (const QHostAddress& p_addr)
{
    Broadcast::ping (System::toQualifier (p_addr));
}

BroadcastMessage::BroadcastMessage (const Message& p_msg) : Message (p_msg) { }

BroadcastMessage::BroadcastMessage (const BroadcastType& brdtype) : Message()
{
    this->setProperty ("Type" , "Broadcast");
    this->setProperty ("BroadcastType" , ( (int) brdtype));
    this->setProperty ("Recipient" , "broadcast");
}

const BroadcastMessage::BroadcastType BroadcastMessage::broadcastType() const
{
    const int val = this->property ("BroadcastType").toInt ();
    return static_cast< const BroadcastType > (val);
}
}
}

#include "broadcast.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
