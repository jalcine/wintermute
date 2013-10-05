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

#ifndef WINTERMUTE_PROCEDURE_DUMMY_RECEIVER_HPP
#define WINTERMUTE_PROCEDURE_DUMMY_RECEIVER_HPP

#include <Wintermute/Procedure/Receiver>

namespace Wintermute {
  namespace Procedure {
    class DummyReceiver : public Receiver {
      Q_OBJECT;

      public:
        explicit DummyReceiver();
        void receiveMessage(const Call* call);
        virtual ~DummyReceiver();
    };
  }
}

#endif
