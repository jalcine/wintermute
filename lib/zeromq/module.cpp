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

#include <Wintermute/Application>
#include <Wintermute/Globals>
#include <Wintermute/Logging>
#include <QtZeroMQ/Socket>
#include <QtZeroMQ/Message>
#include <QtZeroMQ/PollingSocket>
#include "module.hpp"
#include "plugin.hpp"
#include "globals.hpp"
#include "dispatcher.hpp"
#include "receiver.hpp"
#include "module.moc"

using Wintermute::ZeroMQ::Module;
using Wintermute::ZeroMQ::Plugin;
using Wintermute::ZeroMQ::Dispatcher;
using Wintermute::ZeroMQ::Receiver;

Module::Module ( ZeroMQ::Plugin* plugin ) : Wintermute::Procedure::Module ( plugin )
{
  setDomain ( WINTERMUTE_DOMAIN );
  setPackage ( "zeromq" );
  winfo(this, "Pumping in ZeroMQ goodness to Wintermute...");
  Dispatcher* dispatcher = new Dispatcher;
  dispatcher->setParent(this);
  Receiver* receiver = new Receiver;
  receiver->setParent(this);
  winfo(this, "Thanks Pete, Wintermute's on-line.");
}

void
Module::start()
{
  winfo(this, "Building ZeroMQ polling context...");
  m_context = new QtZeroMQ::PollingContext(this);
  winfo(this, "Starting ZeroMQ polling context...");
  m_context->start();
  bindIncomingSocket();
  bindOutgoingSocket();

  m_context->run();
  m_context->poll(300);
}

void
Module::bindIncomingSocket(){
  m_incomingSocket = dynamic_cast<QtZeroMQ::PollingSocket*>(m_context->createSocket(QtZeroMQ::Socket::TypeSubscribe, this));
  connect(m_incomingSocket, SIGNAL(messageReceived(const QList<QByteArray>&)), this, SLOT(onMessageReceived(const QList<QByteArray>&)));
  m_incomingSocket->setIdentity("wintermute:in");
  m_incomingSocket->connectTo(WINTERMUTE_SOCKET_IPC);
  m_incomingSocket->subscribeTo("wintermute:global");
}

void
Module::bindOutgoingSocket(){
  m_outgoingSocket = dynamic_cast<QtZeroMQ::PollingSocket*>(m_context->createSocket(QtZeroMQ::Socket::TypePublish, this));
  m_outgoingSocket->setIdentity("wintermute:out");
  m_outgoingSocket->bindTo(WINTERMUTE_SOCKET_IPC);
}

void
Module::onMessageReceived(const QList<QByteArray>& message)
{
  winfo(this, "Got some messages...");
  QByteArray data;
  foreach(const QByteArray& part, message){
    data += part;
  }

  QtZeroMQ::Message msg(data);
  winfo(this, QString(msg.toByteArray()));
}

void
Module::stop()
{
  winfo(this, "Stopping ZeroMQ polling context...");
  m_context->stop();
  winfo(this, "Stopped ZeroMQ polling context.");
}

Module::~Module()
{
}
