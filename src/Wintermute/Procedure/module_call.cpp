/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2011 - 2014 Jacky Alciné <me@jalcine.me>
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

#include "module.hpp"
#include "module_call.hpp"
#include "Wintermute/private/Procedure/call.hpp"

using Wintermute::Procedure::Call;
using Wintermute::Procedure::Module;
using Wintermute::Procedure::ModuleCall;

ModuleCall::ModuleCall( const QString& name, const Module* module ) :
  Call ( const_cast<Module*>(module) )
{
  d->name = name;
}

ModuleCall::CallbackSignature
ModuleCall::callback() const
{
  return m_callback;
}

const Module&
ModuleCall::module() const
{
  return *m_module;
}

void
ModuleCall::setCallback ( ModuleCall::CallbackSignature& signature )
{
  m_callback = signature;
}

ModuleCall::~ModuleCall()
{
}
