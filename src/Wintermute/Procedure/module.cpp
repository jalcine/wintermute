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
  this->socketIn  = new zmq::socket_t(*ModulePrivate::context, ZMQ_SUB);
  this->socketOut = new zmq::socket_t(*ModulePrivate::context, ZMQ_PUB);

  socketIn->bind("tcp://*:3991");
  socketOut->connect("tcp://*:3991");
}

ModulePrivate::~ModulePrivate() {
}

Module::Module(QObject* parent) : QObject(parent), d_ptr(new ModulePrivate(this)){
}

void Module::mountCall(Call::Signature callSig, const QString& name){
  Q_D(Module);
  d->knownMethods[name] = &callSig;
}

Module::~Module() {
  // TODO: Report to world that you're leaving us.
  // TODO: Disconnect sockets.
}

#include "Wintermute/Procedure/module.moc"
