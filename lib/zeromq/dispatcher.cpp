/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2013 Jacky Alciné <me@jalcine.me>
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

#include <Wintermute/Logging>
#include <Wintermute/Procedure/Call>
#include <QtZeroMQ/Message>
#include "globals.hpp"
#include "dispatcher.hpp"
#include "module.hpp"
#include "dispatcher.moc"

using Wintermute::ZeroMQ::Dispatcher;
using Wintermute::ZeroMQ::Module;
using Wintermute::Procedure::Call;

Dispatcher::Dispatcher ( Module* a_module ) :
  Wintermute::Procedure::Dispatcher::Dispatcher(),
  m_socket ( 0 )
{
  setParent ( a_module );
  m_socket = dynamic_cast<QtZeroMQ::PollingSocket*> (
     a_module->m_context->createSocket ( QtZeroMQ::Socket::TypePublish, this ) );
  m_socket->bindTo ( WINTERMUTE_SOCKET_IPC );
  winfo ( this, "Hey, sending over ZeroMQ." );
}

Dispatcher::~Dispatcher()
{
}

void
Dispatcher::sendMessage ( const Call* message ) throw ( zmq::error_t )
{
  const QByteArray data = message->toString().toUtf8();
  Module* module = qobject_cast<Module*> ( parent() );
  winfo ( this, QString ( "Sending %1 ..." ).arg ( QString ( data ) ) );
  Q_ASSERT ( module != NULL );
  m_socket->sendMessage ( data );
  winfo ( this, "Message sent." );
}
