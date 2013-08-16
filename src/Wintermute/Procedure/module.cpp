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

Module::Module(QObject* parent) : QObject(parent), d_ptr(new ModulePrivate(this)){
}

QVariant
Module::dispatch(Call* call) {
  const QString callStr = call->toString();
  return QVariant(callStr);
}

QVariant
Module::invoke(const QString& callName, const QVariantList& arguments){
  Q_D(Module);

  if (!d->calls.contains(callName))
    return QVariant(-1);

  CallPointer call = d->calls[callName];
  return call->invoke(arguments);
}

void
Module::mount(CallPointer call){
  Q_D(Module);

  // TODO: Use a shared pointer to prevent a segfault.
  d->calls[call->name()].swap(call);
}

Module::~Module() {
  // TODO: Report to world that you're leaving us.
  // TODO: Disconnect sockets.
}

#include "Wintermute/Procedure/module.moc"
