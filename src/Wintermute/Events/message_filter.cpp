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

#include "Wintermute/Globals"
#include "Wintermute/logging.hpp"
#include "Wintermute/application.hpp"
#include "Wintermute/Events/message.hpp"
#include "Wintermute/Events/message_filter.hpp"
#include "Wintermute/Procedure/message.hpp"
#include "Wintermute/private/Procedure/dispatcher.hpp"

using Wintermute::Procedure::Message;
using Wintermute::Events::MessageEvent;
using Wintermute::Events::MessageEventFilter;
using Wintermute::Procedure::DispatcherPrivate;

MessageEventFilter::MessageEventFilter() :
  QObject ( wntrApp )
{
  wtrace("MessageEventFilter", "Prepared.");
}

MessageEventFilter::~MessageEventFilter()
{
  wtrace("MessageEventFilter", "Removed.");
}

bool
MessageEventFilter::eventFilter(QObject* object, QEvent* event)
{
  if ( typeid(*event) == typeid(MessageEvent) ) {
    wtrace("MessageEventFilter", "Obtained a MessageEvent.");
    const MessageEvent* messageEvent = dynamic_cast<MessageEvent*>(event);

    switch (messageEvent->direction()) {
      case MessageEvent::DirectionReceive:
        wtrace("MessageEventFilter", "Got a message for retrieval.");
        return handleReceivingMessage(messageEvent->message());
        break;

      case MessageEvent::DirectionDispatch:
        wtrace("MessageEventFilter", "Got a message for dispatch.");
        return handleDispatchingMessage(messageEvent->message());
        break;

      default:
        wtrace("MessageEventFilter", "Unknown direction provided.");
        break;
    }
  }

  return false;
}

bool
MessageEventFilter::handleDispatchingMessage(const Message& message)
{
  Q_ASSERT ( message.valid() );
  wtrace("MessageEventFilter", QString("Sending message %1.").arg(message));
  DispatcherPrivate::dispatchMessage ( message );
  return true;
}

bool
MessageEventFilter::handleReceivingMessage(const Message& message)
{
  Q_ASSERT ( message.valid() );
  wtrace("MessageEventFilter", QString("Obtained message %1").arg(message));
  /// @todo Determine the type of message and invoke handlers based on that.
  return true;
}
