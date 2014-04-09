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

#include "Wintermute/Events/message.hpp"

using Wintermute::Events::MessageEvent;
using Wintermute::Procedure::Message;

const int MessageEvent::EventType = QEvent::registerEventType();

MessageEvent::MessageEvent(const TransportDirection& direction,
                           const Message& message) : QEvent( (QEvent::Type) MessageEvent::EventType),
  m_direction(direction), m_message(message)
{
}

MessageEvent::TransportDirection
MessageEvent::direction() const
{
  return m_direction;
}

const Message&
MessageEvent::message() const
{
  return m_message;
}

MessageEvent::~MessageEvent()
{
}
