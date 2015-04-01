/*
   Wintermute - A foundation for intelligent computing.
   Copyright (c) 2010 - 2015 by Jacky Alcine

   Wintermute is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   Wintermute is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with Wintermute; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
   */

#ifndef WINTERMUTE_HEARTBEAT_MESSAGES_HPP_
# define WINTERMUTE_HEARTBEAT_MESSAGES_HPP_

#include <wintermute-core/message.hpp>
#include <wintermute-core/module.hpp>
#include "module.hpp"
#include "globals.hpp"

namespace Wintermute
{
  namespace Heartbeat
  {
    /**
     * A ping message to be sent by a child process of Wintermute.
     * @sa Wintermute::Message
     */
    class PingMessage : public Wintermute::Message
    {
      friend class Wintermute::Heartbeat::Module;

      protected:
        explicit PingMessage();
        virtual ~PingMessage();

      public:
        static Message craft();
    };

    /**
     * A pong message to be sent by the heartbeat center of Wintermute.
     */
    class PongMessage : public Wintermute::Message
    {
      friend class Wintermute::Heartbeat::Module;

      protected:
        explicit PongMessage();
        virtual ~PongMessage();

      public:
        static Message craft();
    };
  }
}

#endif
