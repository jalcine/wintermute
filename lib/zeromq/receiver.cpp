/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2011, 2012, 2013, 2014 Jacky Alciné <me@jalcine.me>
 *
 * Wintermute is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <QtZeroMQ/Message>
#include <QtZeroMQ/PollingSocket>
#include <Wintermute/Logging>
#include <Wintermute/Procedure/Call>
#include "globals.hpp"
#include "module.hpp"
#include "receiver.hpp"
#include "receiver.moc"

using Wintermute::ZeroMQ::Receiver;
using Wintermute::ZeroMQ::Module;
using Wintermute::Procedure::Call;

Receiver::Receiver ( Module* a_module ) :
  Wintermute::Procedure::Receiver(), m_socket ( 0 )
{
  setParent ( a_module );
  m_socket = dynamic_cast<QtZeroMQ::PollingSocket*> (
               a_module->m_context->createSocket ( QtZeroMQ::Socket::TypeSubscribe, this ) );
  m_socket->subscribeTo ( QString ( WINTERMUTE_SOCKET_IPC ) );
  m_socket->connectTo ( WINTERMUTE_SOCKET_IPC );
  winfo ( this, "Hey, listening on ZeroMQ." );
}

Receiver::~Receiver()
{
}

void
Receiver::onMessageReceived ( const QList<QByteArray>& data )
{
  QByteArray chunks;
  foreach ( QByteArray chunk, data ) {
    chunks += chunk;
  }
  winfo ( this, QString ( "Obtained incoming call of %1 bytes." ).arg ( chunks.size() ) );
  Call* receivedCall = Call::fromString ( chunks );
  receivedCall->operator()();
}
