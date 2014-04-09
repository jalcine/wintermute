/**
 * @author Jacky Alciné <me@jalcine.me>
 * @copyright © 2011, 2012, 2013, 2014 Jacky Alciné <me@jalcine.me>
 * @if 0
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
 * @endif
 **/

#include <QtCore/QCoreApplication>
#include "Wintermute/application.hpp"
#include "module.hpp"
#include "message.hpp"
#include "Wintermute/private/Procedure/receiver.hpp"
#include "receiver.hpp"

using Wintermute::Procedure::Message;
using Wintermute::Procedure::Receiver;
using Wintermute::Procedure::ReceiverList;
using Wintermute::Procedure::ReceiverPrivate;

ReceiverList ReceiverPrivate::receivers = ReceiverList();

Receiver::Receiver (QObject* parent) :
  QObject ( parent == nullptr ? wntrApp : parent )
{
  ReceiverPrivate::addReceiver ( this );
}

void
Receiver::receiveMessage ( const Message& message )
{
  QPointer<Module> module = Module::findModule ( message.receivingModule() );

  if ( module.isNull() ) {
    wtrace(this, QString("No module found by %1.").arg(message.receivingModule()));
    return;
  } else {
    /// @todo Use MessageEvent and send a 'TypeReceived' kind of event.
    wtrace(this, "Received message but not able to handle yet.");
  }
}

Receiver::~Receiver()
{
  ReceiverPrivate::removeReceiver ( this );
}
