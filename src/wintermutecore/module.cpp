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

#include "module.hh"
#include "module.hpp"
#include "logging.hpp"

using Wintermute::Module;
using Wintermute::Module;
using Wintermute::ModulePrivate;

Module::Module(const Designation& aDesignation) : d_ptr(new ModulePrivate)
{
  W_PRV(Module);
  d->designation = aDesignation;
  wtrace("Module " + static_cast<string>(aDesignation) + " started.");
}

Module::Designation Module::designation() const
{
  W_PRV(Module);
  return d->designation;
}

bool Module::sendMessage(const Message& message) const
{
  throw std::invalid_argument("This method has not be overriden.");
  return false;
}

bool Module::receiveMessage(const Message& message) const
{
  throw std::invalid_argument("This method has not be overriden.");
  return false;
}

Module::~Module()
{
}

ModulePrivate::ModulePrivate()
{
}

ModulePrivate::~ModulePrivate()
{
}
