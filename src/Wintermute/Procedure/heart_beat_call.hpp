/**
 * vim: ft=qt.cpp
 * Copyright (C) 2013 Jacky Alcine <me@jalcine.me>
 *
 * This file is part of Wintermute, the extensible AI platform.
 *
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

#ifndef WINTERMUTE_CORE_PROCEDURE_HEARTBEAT_HPP
#define WINTERMUTE_CORE_PROCEDURE_HEARTBEAT_HPP

#include <Wintermute/Procedure/Call>

namespace Wintermute {
  namespace Procedure {
    class Module;
    class HeartbeatCall : public Call {
      Q_OBJECT;
      Q_DISABLE_COPY(HeartbeatCall);

      public:
        explicit HeartbeatCall(Module* module);
        virtual ~HeartbeatCall();
        virtual inline QString name() const { return "heartbeat"; }
        virtual inline Call::Type type() const{ return Call::Signal; }
    };
  }
}

#endif
