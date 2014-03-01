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

Module::Module ( ZeroMQ::Plugin* plugin ) : 
  Wintermute::Procedure::Module ( plugin ),
  m_context ( new QtZeroMQ::PollingContext ( this ) ), 
  m_dispatcher ( 0 ),
  m_receiver ( 0 )
{
  setDomain ( WINTERMUTE_DOMAIN );
  setPackage ( "zeromq" );
  m_receiver = new Receiver ( this );
  m_dispatcher = new Dispatcher ( this );
  connect ( m_context, SIGNAL ( polled() ), this, SLOT ( pollInvoked() ) );
  connect ( m_context, SIGNAL ( pollError() ), this, SLOT ( pollError() ) );
}

void
Module::pollInvoked()
{
  // TODO Query context for more messages.
}

void
Module::pollError()
{
  // TODO Handle errors with polling.
}

void
Module::start()
{
  m_context->start();
}

void
Module::stop()
{
  m_context->stop();
}

Module::~Module()
{
  stop();
}
