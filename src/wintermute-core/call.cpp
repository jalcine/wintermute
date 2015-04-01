/*
 * Wintermute is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with Wintermute; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "call.hh"
#include "call.hpp"
#include "logging.hpp"

using Wintermute::Call;
using Wintermute::CallPrivate;
using Wintermute::Util::Serializable;

Call::Call(const string& callName) :
  Invokable(), Serializable(),
  d_ptr(new CallPrivate)
{
  Call::d_ptr->name = callName;
}

Call::~Call()
{
}

string Call::name() const
{
  W_PRV(const Call);
  return d->name;
}

void Call::deserialize(const Serializable::Map& map)
{
  W_PRV(Call);
  d->name = map.find("name")->second;
}

Serializable::Map Call::serialize() const
{
  Serializable::Map map;
  map.insert(std::make_pair("name", name()));
  return map;
}
