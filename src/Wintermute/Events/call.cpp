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

#include "Wintermute/Procedure/call.hpp"
#include "Wintermute/Events/call.hpp"

using Wintermute::Events::CallEvent;
using Wintermute::Procedure::Call;

const int CallEvent::TypeReceive  = QEvent::registerEventType();
const int CallEvent::TypeDispatch = QEvent::registerEventType();

CallEvent::CallEvent(int type, const Call* call) :
  QEvent((QEvent::Type) type) , m_call(call)
{

}

const Call*
CallEvent::call() const
{
  return m_call;
}

CallEvent::~CallEvent()
{

}
