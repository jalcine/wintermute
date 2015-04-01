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

#include "module_designation.hh"
#include "module.hpp"
#include "logging.hpp"

using Wintermute::Module;
using Wintermute::ModulePrivate;
using Wintermute::DesignationPrivate;
using Wintermute::Util::Serializable;

Module::Module::Designation::Designation(const string& aName, const string& aDomain) :
  Serializable(""), d_ptr (std::make_shared<DesignationPrivate>())
{
  W_PRV(Designation);
  d->name = aName;
  d->domain = aDomain;
}

Module::Designation::Designation(const Designation& other) : Serializable(other),
  d_ptr (std::make_shared<DesignationPrivate>())
{
  W_PRV(Designation);
  assert(other.d_ptr);
  d->clone(other.d_ptr);
}

Module::Designation::Designation(const string& jsonString) :
  d_ptr (std::make_shared<DesignationPrivate>())
{
  deserialize(Serializable::fromString(jsonString));
}

Module::Designation::Designation() :
  d_ptr (std::make_shared<DesignationPrivate>())
{
  W_PRV(Designation);
  d->name = std::string();
  d->domain = std::string();
}

string Module::Designation::name() const
{
  W_PRV(const Designation);
  return d->name;
}

string Module::Designation::domain() const
{
  W_PRV(const Designation);
  return d->domain;
}

bool Module::Designation::isNull() const
{
  W_PRV(const Designation);
  return d && d->domain.empty() && d->name.empty();
}

bool Module::Designation::operator!=(const Designation& other) const
{
  return ! (*this == other);
}

bool Module::Designation::operator==(const Designation& other) const
{
  return name() == other.name() &&
         domain() == other.domain();
}

void Module::Designation::deserialize(const Serializable::Map& data)
{
  W_PRV(Designation);
  assert(data.count("domain") == 1);
  assert(data.count("name") == 1);

  d->name = data.at("name");
  d->domain = data.at("domain");
}

Serializable::Map Module::Designation::serialize() const
{
  W_PRV(const Designation);
  Serializable::Map theMap;
  theMap.insert(std::make_pair("name", d->name));
  theMap.insert(std::make_pair("domain", d->domain));

  return theMap;
}

bool Module::Designation::isLocal() const
{
  return true;
}

Module::Designation::~Designation()
{
}
