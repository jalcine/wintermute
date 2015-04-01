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

using std::hash;
using std::string;
using Wintermute::Module;
using Wintermute::DesignationPrivate;

DesignationPrivate::DesignationPrivate()
  : pid(0), name(), domain()
{
}

DesignationPrivate::~DesignationPrivate()
{
}

void DesignationPrivate::clone(const SharedPtr<DesignationPrivate>& other)
{
  assert(other);
  this->pid = other->pid;
  this->name = other->name;
  this->domain = other->domain;
}

size_t DesignationPrivate::Hash::operator()(const Module::Designation& des) const
{
  hash<string> hashStr_fn;
  return hashStr_fn((string) des);
}
