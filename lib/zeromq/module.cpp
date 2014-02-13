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
#include <QtZeroMQ/PollingSocket>
#include "module.hpp"
#include "plugin.hpp"
#include "dispatcher.hpp"
#include "module.moc"

using Wintermute::ZeroMQ::Module;
using Wintermute::ZeroMQ::Plugin;
using Wintermute::ZeroMQ::Dispatcher;

Module::Module ( ZeroMQ::Plugin* plugin ) : Wintermute::Procedure::Module ( plugin )
{
  setDomain ( WINTERMUTE_DOMAIN );
  setPackage ( "zeromq" );
  winfo(this, "Pumping in ZeroMQ goodness to Wintermute...");
  Dispatcher* dispatcher = new Dispatcher;
  dispatcher->setParent(this);
  winfo(this, "Thanks Pete, Wintermute's on-line.");
}

void
Module::start()
{
  winfo(this, "Building ZeroMQ polling context...");
  m_context = new QtZeroMQ::PollingContext(this);
  winfo(this, "Starting ZeroMQ polling context...");
  m_context->start();
  m_incomingSocket = dynamic_cast<QtZeroMQ::PollingSocket*>(m_context->createSocket(QtZeroMQ::Socket::TypeSubscribe, this));
  m_outgoingSocket = dynamic_cast<QtZeroMQ::PollingSocket*>(m_context->createSocket(QtZeroMQ::Socket::TypePublish, this));
  connect(m_incomingSocket, SIGNAL(messageReceived(const QList<QByteArray>&)), this, SLOT(onMessageReceived(const QList<QByteArray>&)));
  m_incomingSocket->connectTo("ipc:///tmp/wintermute.socket");
  m_incomingSocket->setIdentity("wintermute:in");

  m_outgoingSocket->connectTo("ipc:///tmp/wintermute.socket");
  m_outgoingSocket->setIdentity("wintermute:out");
  winfo(this, "Started ZeroMQ polling context.");
  m_context->run();
}

void
Module::onMessageReceived(const QList<QByteArray>& message)
{
  winfo(this, "Going");
}

void
Module::stop()
{
  winfo(this, "Stopping ZeroMQ polling context...");
  m_context->start();
  winfo(this, "Stopped ZeroMQ polling context.");
}

Module::~Module()
{
}
