/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2011 - 2013 Jacky Alciné <me@jalcine.me>
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

#include "Wintermute/Procedure/call.hpp"
#include "Wintermute/Procedure/method_call.hpp"
#include "Wintermute/Procedure/module.hpp"
#include "Wintermute/Procedure/dispatcher.hpp"
#include "Wintermute/logging.hpp"
#include "Wintermute/application.hpp"
#include "Wintermute/private/application.hpp"
#include <QtCore/QMap>

namespace Wintermute
{
class ApplicationPrivate;
namespace Procedure
{
class Module;
class ModulePrivate
{
  Q_DECLARE_PUBLIC ( Module );

public:
  Module* q_ptr;
  QString package;
  QString domain;
  QMap<QString, CallPointer> calls;

  ModulePrivate ( Module* q ) :
    q_ptr ( q ), package ( "" ), domain ( "" ), calls()
  {
    winfo ( q_ptr, "Module setting up..." );
  }

  void checkQualifiedName()
  {
    if ( !domain.isEmpty() && !package.isEmpty() )
    {
      if ( !wntrApp->findModule ( q_ptr->qualifiedName() ) )
      {
         wntrApp->d_ptr->modules << q_ptr;
         // TODO: Emit an signal about 'moduleAdded' here.
      }
    }
  }

  void sendData ( const QString& data ) const {
    Dispatcher::postDispatch ( Call::fromString ( data ) );
  }

  virtual ~ModulePrivate () {
    winfo ( q_ptr, "We out!" );
  }
};
} /* Procedure */
} /* Wintermute  */
