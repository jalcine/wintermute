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

#include "Wintermute/application.hpp"
#include "Wintermute/Procedure/dispatcher.hpp"
#include "Wintermute/logging.hpp"

namespace Wintermute
{
namespace Procedure
{
class Dispatcher;
class DispatcherPrivate
{
public:
  static QList<Dispatcher*> dispatchers;

  static bool isDispatcherKnown ( Dispatcher* dispatcher )
  {
    for ( Dispatcher * aDispatcher : DispatcherPrivate::dispatchers )
    {
      if ( aDispatcher->metaObject()->className()
           == dispatcher->metaObject()->className() )
      {
        return true;
      }
    }
    return false;
  }

  static bool dispatchMessage ( const Message& message )
  {
    Q_ASSERT ( message.valid() );
    if ( dispatchers.empty() )
    {
      wwarn ( "DispatcherPrivate", "Wintermute is running with no dispatchers." );
      return false;
    }
    else
    {
      for ( Dispatcher * dispatchClient : dispatchers )
      {
        dispatchClient->sendMessage ( message );
      }
      return true;
    }
  }

  static bool addDispatcher ( Dispatcher* dispatcher )
  {
    if ( !dispatcher )
    {
      return false;
    }
    if ( isDispatcherKnown ( dispatcher ) )
    {
      wdebug ( dispatcher, "Already added into dispatcher index." );
      return false;
    }
    DispatcherPrivate::dispatchers << dispatcher;
    wdebug ( "DispatcherPrivate", QString ( "%1 added to dispatcher pool." ).
             arg ( dispatcher->metaObject()->className() ) );
    return true;
  }

  static bool removeDispatcher ( Dispatcher* dispatcher )
  {
    if ( !isDispatcherKnown ( dispatcher ) )
    {
      return false;
    }
    DispatcherPrivate::dispatchers.removeAll ( dispatcher );
    wdebug ( "DispatcherPrivate", QString ( "%1 removed from dispatcher pool." ).
             arg ( dispatcher->metaObject()->className() ) );
    return true;
  }
};
}
}
