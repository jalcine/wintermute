/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2013, 2014 Jacky Alciné <me@jalcine.me>
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

#include <QtDBus/QDBusConnection>
#include <Wintermute/Application>
#include <Wintermute/Globals>
#include <Wintermute/Logging>
#include <Wintermute/Procedure/Call>
#include "module.hpp"
#include "plugin.hpp"
#include "adaptor.hpp"
#include "dispatcher.hpp"
#include "receiver.hpp"

using Wintermute::DBus::Module;
using Wintermute::DBus::Plugin;
using Wintermute::DBus::Dispatcher;
using Wintermute::Procedure::Call;

Module::Module ( DBus::Plugin* plugin ) :
  Wintermute::Procedure::Module ( plugin ),
  m_dispatcher ( 0 ), m_receiver ( 0 )
{
  setDomain ( WINTERMUTE_DOMAIN );
  setPackage ( "dbus" );
  m_dispatcher = new Dispatcher;
  m_receiver = new Receiver;
  m_dispatcher->setParent ( this );
  m_receiver->setParent ( this );
  m_adaptor = new Adaptor ( this );
}

void
Module::start()
{
  m_adaptor->registerOnDBus();
}

void
Module::stop()
{
  m_adaptor->deregisterFromDBus();
}

Module::~Module()
{
}
