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
#include "Wintermute/logging.hpp"
#include "Wintermute/Procedure/message.hpp"
#include "Wintermute/Events/message.hpp"
#include "Wintermute/private/Procedure/dispatcher.hpp"

using Wintermute::Events::MessageEvent;
using Wintermute::Procedure::Dispatcher;
using Wintermute::Procedure::DispatcherPrivate;

QList<Dispatcher*> DispatcherPrivate::dispatchers = QList<Dispatcher*>();

Dispatcher::Dispatcher() :
  QObject ( wntrApp )
{
  DispatcherPrivate::addDispatcher ( this );
}

void
Dispatcher::queueMessage ( const Message& message )
{
  Q_ASSERT ( message.valid() );
  wtrace("(Dispatcher::static)",
         QString("Got %1 for dispatching; but not yet implemented.").arg(message));
  MessageEvent* event = new MessageEvent(MessageEvent::DirectionDispatch, message);
  QCoreApplication::postEvent(wntrApp, event);
}

Dispatcher::~Dispatcher()
{
  DispatcherPrivate::removeDispatcher ( this );
}
