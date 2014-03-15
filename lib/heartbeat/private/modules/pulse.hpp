/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2014 Jacky Alciné <me@jalcine.me>
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

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QVariant>
#include <QtCore/QVariantList>
#include <QtCore/QVariantMap>
#include <Wintermute/Procedure/Module>
#include "globals.hpp"
#include "Wintermute/private/Procedure/module.hpp"

namespace Wintermute
{
namespace Heartbeat
{
class PulseModule;
class PulseModulePrivate
{
  public:
    QTimer timer;
    PulseModule* q_ptr;
    quint64 count;
    Q_DECLARE_PUBLIC ( PulseModule );

    PulseModulePrivate ( PulseModule* q ) :
      timer ( q ), q_ptr ( q ), count ( 0 )
    {
      timer.setInterval ( WINTERMUTE_HEARTBEAT_INTERVAL );
      timer.setSingleShot ( true );
      QObject::connect(&timer,SIGNAL(timeout()),q,SLOT(tick()));
    }

    QVariantMap
    getModuleInfo ( const QString& moduleName )
    {
      Procedure::Module* module = wntrApp->findModule ( moduleName );
      QVariantMap values;

      if ( module != nullptr )
      {
        values.insert ( "name",    module->qualifiedName() );
        values.insert ( "domain",  module->domain() );
        values.insert ( "package", module->package() );
        values.insert ( "calls",   module->calls() );
      }

      return values; 
    }

    QVariantList
    getAllModulesInfo()
    {
      QVariantList modules;
      for (Procedure::Module* module: wntrApp->modules())
      {
        modules.push_back ( getModuleInfo ( module->qualifiedName() ) );
      }

      return modules;
    }
};
}
}
