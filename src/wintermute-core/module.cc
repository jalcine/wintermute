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
#include "logging.hpp"

using Wintermute::ModulePrivate;
using Wintermute::Events::Emitter;

ModulePrivate::ModulePrivate(const Module::Designation& des) :
  designation(des),
  emitter(nullptr)
{
  emitter = make_shared<Emitter>();
  calls.clear();
}

ModulePrivate::~ModulePrivate()
{
  wdebug("Flushing out calls for " + static_cast<string>(designation) + "...");
  calls.clear();
  wdebug("Cleared calls for " + static_cast<string>(designation) + ".");
}
