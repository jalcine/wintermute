/**
 * @author Jacky Alciné <me@jalcine.me>
 * @copyright © 2011, 2012, 2013, 2014 Jacky Alciné <me@jalcine.me>
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

#include <QtCore/QMap>
#include <QtCore/QPointer>
#include "Wintermute/logging.hpp"
#include "Wintermute/application.hpp"
#include "Wintermute/Procedure/module.hpp"
#include "Wintermute/Procedure/module_call.hpp"

namespace Wintermute
{
namespace Procedure
{
class Module;
class ModuleCall;
class ModulePrivate
{
  Q_DECLARE_PUBLIC ( Module );

public:
  static ModuleHash modules;
  QPointer<Module> q_ptr;
  Designation designation;
  QHash<QString, ModuleCall*> calls;

  ModulePrivate ( Module* a_qPtr ) : q_ptr ( a_qPtr ),
    designation ( Designation::Null ), calls ( )
  {
    wtrace(q_ptr, "Have yet to set a designation.");
  }

  void setDefinition(const Designation& a_designation)
  {
    wtrace ( q_ptr, QString("Setting up...") );
    const bool isDefTaken = ModulePrivate::modules.contains(a_designation);
    Q_ASSERT ( !isDefTaken );

    if ( !isDefTaken )
    {
      wtrace(q_ptr, QString("Registered %1.").arg(a_designation.package));
      ModulePrivate::modules.insert ( a_designation, q_ptr );
    }
    else
    {
      wfatal(q_ptr, "The designation provided was already taken.");
    }
  }

  virtual ~ModulePrivate ()
  {
    ModulePrivate::modules.remove ( designation );
  }
};
} /* Procedure */
} /* Wintermute  */
