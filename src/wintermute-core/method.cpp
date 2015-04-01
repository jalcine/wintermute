/*
 * Wintermute is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with Wintermute; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "method.hh"
#include "method.hpp"
#include "module.hpp"
#include "message.hpp"
#include "logging.hpp"

using Wintermute::Util::Serializable;
using Wintermute::Method;
using Wintermute::Module;
using Wintermute::Message;
using std::make_pair;

Method::Method(const string & callName,
    const Module::Designation & destinationModule,
    const Module::Designation & originModule,
    const Method::Arguments & providedArguments) :
  d_ptr(new MethodPrivate)
{
  W_PRV(Method);
  d->callName = callName;
  d->originatingModule = originModule;
  d->destinationModule = destinationModule;
  d->arguments = providedArguments;
}

Method::~Method()
{
}

// TODO: Do some exception handling around the invoked function.
string Method::invoke(const Method::Arguments providedArguments)
{
  string result;
  Method::Arguments usedArguments = providedArguments;

  if (usedArguments.empty())
  {
    usedArguments = arguments();
  }

  if (!isInvokable())
  {
    return result;
  }

  Module::Ptr modulePtr = Module::Pool::instance()->find(module());
  winfo("Invoking " + name() + " on " + static_cast<string>(module()) + " ...");
  Module::Call::Ptr moduleCallPtr = modulePtr->call(name());
  assert(moduleCallPtr);

  if (!moduleCallPtr)
  {
    throw std::invalid_argument("The module '" +
      static_cast<string>(modulePtr->designation()) +
      "' does not have the call '" + name() + "'.");
  }

  result = moduleCallPtr->invokeFunction(Serializable::toString(usedArguments));
  return result;
}

bool Method::isInvokable() const
{
  const Module::Ptr aModule = Module::Pool::instance()->find(module());
  if (!aModule)
  {
    return false;
  }

  const Module::Call::Ptr call = aModule->call(name());
  return !!call;
}

string Method::name() const
{
  W_PRV(const Method);
  return d->callName;
}

Module::Designation Method::module() const
{
  W_PRV(const Method);
  return d->destinationModule;
}

Module::Designation Method::originatingModule() const
{
  W_PRV(const Method);
  return d->originatingModule;
}

Method::Arguments Method::arguments() const
{
  W_PRV(const Method);
  return d->arguments;
}

void Method::deserialize(const Serializable::Map& data)
{
  W_PRV(Method);
  d->callName = data.find("name")->second;

  Method::Arguments args = Serializable::fromString(data.find("args")->second);
  d->arguments = static_cast<Method::Arguments>(args);

  Serializable::Map moduleMap = Serializable::fromString(data.find("module")->second);
  d->originatingModule = static_cast<Module::Designation>(moduleMap.find("from")->second);
  d->destinationModule = static_cast<Module::Designation>(moduleMap.find("to")->second);
}

Serializable::Map Method::serialize() const
{
  Serializable::Map data, moduleMap;
  moduleMap.insert(make_pair("from", originatingModule()));
  moduleMap.insert(make_pair("to", module()));
  data.insert(make_pair("name", name()));
  data.insert(make_pair("args", Serializable::toString(arguments())));
  data.insert(make_pair("module", Serializable::toString(moduleMap)));

  return data;
}

Message Method::toMessage() const
{
  Message msg;
  Message::HashType msgHash;
  msgHash.insert(make_pair("call", WINTERMUTE_MESSAGE_CALL_METHOD));
  msgHash.insert(make_pair("args", Serializable::toString(arguments())));

  msg.setSender(originatingModule());
  msg.setReceiver(module());
  msg.setPayload(msgHash);

  return msg;
}
