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

Module::Module ( ZeroMQ::Plugin* plugin ) : Wintermute::Procedure::Module ( plugin ),
  m_context(new QtZeroMQ::PollingContext(this)), m_dispatcher(0), m_receiver(0)
{
  setDomain ( WINTERMUTE_DOMAIN );
  setPackage ( "zeromq" );
  m_dispatcher = new Dispatcher(this);
  m_receiver = new Receiver(this);
}

void
Module::start()
{
  m_context->start();
}

void
Module::bindIncomingSocket(){
  //m_incomingSocket = dynamic_cast<QtZeroMQ::PollingSocket*>(m_context->createSocket(QtZeroMQ::Socket::TypeSubscribe, this));
  //m_incomingSocket->setIdentity("wintermute:in");
  //m_incomingSocket->connectTo(WINTERMUTE_SOCKET_IPC);
  //m_incomingSocket->subscribeTo("wintermute:global");
}

//void
//Module::onMessageReceived(const QList<QByteArray>& message)
//{
  //winfo(this, "Got some messages...");
  //QByteArray data;
  //foreach(const QByteArray& part, message){
    //data += part;
  //}

  //QtZeroMQ::Message msg(data);
  //winfo(this, QString(msg.toByteArray()));
//}

void
Module::stop()
{
  m_context->stop();
}

Module::~Module()
{
  stop();
}
