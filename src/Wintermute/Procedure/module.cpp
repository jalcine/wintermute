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

#include "Wintermute/Procedure/module.hpp"
#include "Wintermute/private/Procedure/module.hpp"

using Wintermute::Procedure::Module;
using Wintermute::Procedure::ModulePrivate;

zmq::context_t* ModulePrivate::context;

ModulePrivate::ModulePrivate(Module* q) : q_ptr(q) {
  Q_Q(Module);
  // Create our lovely ZMQ sockets using a pub/sub setup.
  this->socketIn  = new zmq::socket_t(*ModulePrivate::context, ZMQ_SUB);
  this->socketOut = new zmq::socket_t(*ModulePrivate::context, ZMQ_PUB);

  // Now listen for and send messages over our favorite port, 3991.
  // TODO: Make port number changable.
  socketIn->bind("tcp://*:3991");
  socketOut->connect("tcp://*:3991");

  // TODO: Send out a hello message to the heartbeat module.
}

ModulePrivate::~ModulePrivate() {
  socketIn->close();
  socketOut->close();
}

Module::Module(QObject* parent) : QObject(parent), d_ptr(new ModulePrivate(this)){
}

QVariant
Module::invokeCall(const Call* call){
  return QVariant();
}

void
Module::mountCall(const Call* call){
  Q_D(Module);

  // TODO: Use a shared pointer to prevent a segfault.
  d->knownCalls[call->name()] = call;
}

QVariant
Module::dispatchCall(const QString& name, const QVariantList& arguments = QVariantList()){
  Q_D(Module);

 Call::Signature callSig = *(d->knownMethods.value(name));

 if (!callSig)
   return QVariant();

 return callSig(arguments);
}

Module::~Module() {
  // TODO: Report to world that you're leaving us.
  // TODO: Disconnect sockets.
}

#include "Wintermute/Procedure/module.moc"
