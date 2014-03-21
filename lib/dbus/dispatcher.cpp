/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2011 - 2013 Jacky Alciné <me@jalcine.me>
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
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusReply>
#include "dispatcher.hpp"
#include "receiver.hpp"
#include "module.hpp"

using Wintermute::DBus::Dispatcher;
using Wintermute::DBus::Receiver;
using Wintermute::DBus::Module;
using Wintermute::Procedure::Call;

Dispatcher::Dispatcher() :
  Wintermute::Procedure::Dispatcher::Dispatcher()
{
}

Dispatcher::~Dispatcher()
{
}

void
Dispatcher::sendMessage ( const Call& call )
{
  Q_ASSERT ( call.isValid() );
  QDBusConnection sessionBus = QDBusConnection::sessionBus();
  QDBusConnectionInterface* interface = sessionBus.interface();
  QStringList remoteServices = interface->registeredServiceNames();
  QStringList friendlyServices = remoteServices.filter ( WINTERMUTE_DOMAIN );
  friendlyServices.removeAll ( sessionBus.name() );

  if ( friendlyServices.empty() ) { return; }
  else
  {
    for (const QString remoteService: friendlyServices)
    {
      QDBusMessage methodCall = QDBusMessage::createMethodCall ( remoteService, 
          "/Process", WINTERMUTE_DOMAIN ".dbus" , "handleIncomingCall" );
      methodCall << call.toString();

      QDBusPendingReply<QString> asyncMethodCall = sessionBus.asyncCall ( methodCall );
      DBus::Receiver* reciever = ( ( DBus::Module* ) parent() )->m_receiver;
      QDBusPendingCallWatcher* watcher = new QDBusPendingCallWatcher ( 
          asyncMethodCall, this );
      QObject::connect ( watcher, SIGNAL ( finished (QDBusPendingCallWatcher*) ),
        reciever, SLOT ( handleAsyncCallReply (QDBusPendingCallWatcher*) ) );
    }
  }
}
