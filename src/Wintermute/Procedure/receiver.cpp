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

/**
 * Receivers do nothing more besides collecting inbound information and
 * deserializing it from its obtained format into something that Wintermute
 * can use to invoke a local call or response to a call.
 */

#include <QtCore/QCoreApplication>
#include <Wintermute/Procedure/Module>
#include "Wintermute/private/Procedure/receiver.hpp"
#include "Wintermute/application.hpp"
#include "Wintermute/Events/call.hpp"
#include "Wintermute/Procedure/receiver.hpp"

using Wintermute::Procedure::Receiver;
using Wintermute::Events::CallEvent;

QList<Receiver*> Wintermute::Procedure::ReceiverPrivate::receivers = QList<Receiver*>();

Receiver::Receiver() :
  QObject ( wntrApp )
{
  ReceiverPrivate::addReceiver ( this );
}

void
Receiver::receiveMessage ( const Call* call )
{
  Procedure::Module* module = wntrApp->findModule ( call->recipient() );
  if ( module != nullptr ) {
    CallEvent* event = new CallEvent ( CallEvent::TypeReceive, call );
    QCoreApplication::postEvent ( wntrApp, event );
  }
}

Receiver::~Receiver()
{
  ReceiverPrivate::removeReceiver ( this );
}
