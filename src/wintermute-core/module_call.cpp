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

#include "logging.hpp"
#include "module.hpp"
#include "module_call.hh"
#include "call.hh"
#include "util/serializable.hpp"

using Wintermute::Call;
using Wintermute::Module;
using Wintermute::ModuleCallPrivate;
using Wintermute::Util::Serializable;

Module::Call::Call(const Module::Ptr modulePtr, const string& callName) :
  Wintermute::Call(callName),
  d_ptr(new ModuleCallPrivate)
{
  W_PRV(ModuleCall);
  d->module = modulePtr;
}

Module::Ptr Module::Call::module() const
{
  W_PRV(const ModuleCall);
  return d->module;
}

Module::Call::~Call()
{
}

Serializable::Map Module::Call::serialize() const
{
  Serializable::Map callMap;
  callMap.insert(std::make_pair("designation", ""));
  callMap.insert(std::make_pair("name", name()));

  return callMap;
}
void Module::Call::deserialize(const Serializable::Map& data)
{
  throw new std::invalid_argument("Cannot deserialize a Module::Call of size " + std::to_string(data.size()) + ".");
}
