/**
 * @author Jacky Alciné <me@jalcine.me>
 * @copyright © 2014 Jacky Alciné <me@jalcine.me>
 * @if 0
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
 * @endif
 **/

#include "Wintermute/Procedure/module.hpp"
#include "Wintermute/Procedure/module_call.hpp"

using Wintermute::Procedure::Module;
using Wintermute::Procedure::ModuleCall;

ModuleCall::ModuleCall(const QString& name, const Module* module) : Call(name)
{
  m_module = const_cast<Module*>(module);
  Message::setSender(m_module->definition());
  Message::setReceiver(Module::Definition::compose(WINTERMUTE_DOMAIN, "process",
                       0));
}

bool
ModuleCall::valid() const
{
  if ( !Call::valid() ) {
    return false;
  }

  if ( m_module.isNull() ) {
    return false;
  }

  return true;
}

const Module*
ModuleCall::module() const
{
  Q_ASSERT ( valid() );
  return m_module.data();
}

ModuleCall::~ModuleCall()
{}
