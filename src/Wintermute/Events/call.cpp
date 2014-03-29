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

#include <Wintermute/Application>
#include <Wintermute/Logging>
#include "Wintermute/Procedure/call.hpp"
#include "call.hpp"

using Wintermute::Events::CallEvent;
using Wintermute::Procedure::Call;

const int CallEvent::TypeReceive  = QEvent::registerEventType();
const int CallEvent::TypeDispatch = QEvent::registerEventType();
const int CallEvent::TypeReply    = QEvent::registerEventType();

CallEvent::CallEvent ( const int type, const Call::Pointer &call ) :
  QEvent ( ( QEvent::Type ) type ) , m_callPtr ( call )
{
  Q_ASSERT ( !call.isNull() );
  Q_ASSERT ( call->isValid() );
}

const Call::Pointer
CallEvent::call() const
{
  return m_callPtr;
}

CallEvent::~CallEvent()
{
}
