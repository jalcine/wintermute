/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2014 Jacky Alciné <me@jalcine.me>
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
#include <QtCore/QCoreApplication>
#include <Wintermute/Application>
#include <Wintermute/Logging>
#include "module.hpp"
#include "receiver.hpp"
#include "adaptor.hpp"

using Wintermute::DBus::Adaptor;
using Wintermute::DBus::Receiver;
using Wintermute::Procedure::Call;

Adaptor::Adaptor( Module* module ) :
    QDBusAbstractAdaptor( module )
{
}

void
Adaptor::registerOnDBus()
{
  QDBusConnection bus = QDBusConnection::sessionBus();
  const bool serviceRegistered = bus.registerService ( QString ( 
      "in.wintermute.p%1" ).arg( QCoreApplication::applicationPid() ) );

  if ( !serviceRegistered )
  {
    werr (this, "Failed to register service with D-Bus.");
    return;
  }

  bus.registerObject( "/Process", this, QDBusConnection::ExportAllInvokables );
}

void
Adaptor::deregisterFromDBus()
{
  QDBusConnection bus = QDBusConnection::sessionBus();
  bus.unregisterService ( QString ( "in.wintermute.p%1" ).arg(
    QCoreApplication::applicationPid() ) );

  for (Procedure::Module* module: Procedure::Module::knownModules() )
  {
    const QString objectName = "/" + module->package();
    bus.unregisterObject ( objectName );
  }
}

void
Adaptor::handleIncomingCall ( const QString& arguments, const
    QDBusMessage& message )
{
  Q_ASSERT ( !arguments.isNull() );
  Q_ASSERT ( !arguments.isEmpty() );
  const Call::Pointer incomingCall = Call::fromString ( arguments );
  Module* module = qobject_cast<Wintermute::DBus::Module*>(parent());
  Q_CHECK_PTR ( module );
  module->m_receiver->receiveMessage ( incomingCall );
}

bool
Adaptor::hasModule ( const QString& name )
{
  return Procedure::Module::findModule ( name ) != nullptr;
}

Adaptor::~Adaptor()
{
}
