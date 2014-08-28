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

Module::Module::Designation::Designation(const string& name, const string& domain, const pid_t& pid) :
  Serializable(""), d_ptr (new DesignationPrivate)
{
  W_PRV(Designation);
  d->name = name;
  d->domain = domain;
  d->pid = pid;
}

Module::Designation::Designation(const Designation& other) : Serializable(other),
	d_ptr(new DesignationPrivate)
{
  W_PRV(Designation);
  d->clone(other.d_ptr);
}

Module::Designation::Designation() : d_ptr(new DesignationPrivate)
{
  W_PRV(Designation);
  d->name = std::string();
  d->domain = std::string();
  d->pid = 0;
}

Module::Designation::Designation(const string& jsonString) : d_ptr(new DesignationPrivate)
{
  deserialize(Serializable::fromString(jsonString));
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

pid_t Module::Designation::pid() const
{
  W_PRV(const Designation);
  return d->pid;
}

bool Module::Designation::isLocal() const
{
  W_PRV(const Designation);
  return d->pid == getpid();
}

bool Module::Designation::isNull() const
{
  W_PRV(const Designation);
  return d->domain.empty() && d->name.empty();
}

bool Module::Designation::operator!=(const Designation& other) const
{
  return !this->operator==(other);
}

bool Module::Designation::operator==(const Designation& other) const
{
  return other.name() == name() &&
         other.domain() == domain() &&
         other.pid () == pid();
}

void Module::Designation::deserialize(const Serializable::Map& data)
{
  W_PRV(Designation);
  assert(data.count("pid") == 1);
  assert(data.count("domain") == 1);
  assert(data.count("name") == 1);

  d->pid = stoi(data.at("pid"));
  d->name = data.at("name");
  d->domain = data.at("domain");
}

Serializable::Map Module::Designation::serialize() const
{
  W_PRV(const Designation);
  Serializable::Map theMap;
  theMap.insert(std::make_pair("pid", std::to_string(d->pid)));
  theMap.insert(std::make_pair("name", d->name));
  theMap.insert(std::make_pair("domain", d->domain));

  return theMap;
}

Module::Designation::~Designation()
{
}

void DesignationPrivate::clone(const SharedPtr<DesignationPrivate>& other)
{
  this->pid = other->pid;
  this->name = other->name;
  this->domain = other->domain;
}
