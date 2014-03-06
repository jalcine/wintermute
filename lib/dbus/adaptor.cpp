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
#include <Wintermute/Logging>
#include "module.hpp"
#include "receiver.hpp"
#include "adaptor.hpp"
#include "adaptor.moc"

using Wintermute::DBus::Adaptor;
using Wintermute::DBus::Receiver;
using Wintermute::Procedure::Call;

Adaptor::Adaptor( Module* module ) :
    QDBusAbstractAdaptor( module )
{
  winfo (this, "Adaptor created.");
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

  const bool objectRegistered = bus.registerObject( "/Process", parent(),
    QDBusConnection::ExportChildObjects | QDBusConnection::ExportAllSlots 
      | QDBusConnection::ExportAllInvokables | QDBusConnection::ExportAdaptors
      | QDBusConnection::ExportChildObjects
  );

  if ( objectRegistered )
  {
    winfo ( this, QString( "Registered this process into D-Bus service %1." )
        .arg ( bus.baseService() ) );
  }
}

void
Adaptor::handleIncomingCall ( const QString& arguments, const 
    QDBusMessage& message )
{
  const Call* incomingCall = Call::fromString ( arguments );
  ((Module*) parent())->m_receiver->receiveMessage(incomingCall);
}

bool
Adaptor::hasModule ( const QString& name )
{
  return false;
}

Adaptor::~Adaptor()
{
}
