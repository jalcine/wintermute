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

#include "Wintermute/Procedure/call.hpp"
#include <QtCore/QMap>
#include <zmq.hpp>


namespace Wintermute {
  class ApplicationPrivate;
  namespace Procedure {
    class Module;
    class ModulePrivate {
      friend class Wintermute::ApplicationPrivate;
      static zmq::context_t* context;

      public:
      Module* q_ptr;
      zmq::socket_t* socketIn;
      zmq::socket_t* socketOut;
      QMap<QString, CallPointer> calls;

      ModulePrivate (Module* q) : q_ptr(q) {
        // Create our lovely ZMQ sockets using a pub/sub setup.
        this->socketIn  = new zmq::socket_t(*ModulePrivate::context, ZMQ_SUB);
        this->socketOut = new zmq::socket_t(*ModulePrivate::context, ZMQ_PUB);

        // Now listen for and send messages over our favorite port, 3991.
        // TODO: Make port number changable.
        socketIn->bind("tcp://*:3991");
        socketOut->connect("tcp://*:3991");

        // TODO: Send out a hello message to the heartbeat module.
        sendHeartbeat();
      }

    }

    void
      ModulePrivate::sendHeartbeat() {
        Q_Q(Module);
        HeartbeatCall heartbeat(q);
        q->dispatch("me.jalcine.heartbeat", heartbeat.toString());
      }

    virtual ~ModulePrivate () {
      socketIn->close();
      socketOut->close();

    }
  };
} /* Procedure */
} /* Wintermute  */
