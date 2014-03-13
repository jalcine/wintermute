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

#ifndef WINTERMUTE_EVENTS_CALLEVENT_HPP
#define WINTERMUTE_EVENTS_CALLEVENT_HPP

#include <QtCore/QEvent>

namespace Wintermute
{
namespace Procedure
{
class Call;
}

namespace Events
{
class CallEvent : public QEvent
{
  const Procedure::Call* m_call;

public:
  static const int TypeReceive;
  static const int TypeDispatch;
  explicit CallEvent ( const int type, const Procedure::Call* call );
  virtual ~CallEvent();
  const Procedure::Call* call() const;
};
} /* Events */
}

#endif
