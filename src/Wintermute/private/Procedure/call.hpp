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

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QMap>
#include <QtCore/QCache>
#include <QtCore/QDateTime>
#include "Wintermute/application.hpp"
#include "Wintermute/logging.hpp"
#include "Wintermute/Procedure/call.hpp"

namespace Wintermute
{
namespace Procedure
{
class CallPrivate
{
public:
  static QCache<quint16, Call> calls;
  Call* q_ptr;
  QString recipient;
  QString name;
  Call::Type type;
  QVariantMap data;
  quint16 id;
  Call::CallbackSignature callback;

  explicit CallPrivate ( Call* q ) : q_ptr ( q ),
    recipient(), name(), type ( Call::TypeUndefined ), 
    data(), id ( CallPrivate::calls.size() ), callback ( )
  {
    if ( q_ptr != nullptr )
    {
      CallPrivate::calls.insert ( id, q_ptr );
    }

    data["id"] = id;
    data["timestamp"] = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
  }

  virtual bool hasValidData() const
  {
    if ( !data.contains ( "id" ) ) return false;
    if ( !data.contains ( "timestamp" ) ) return false;
    if ( type == Call::TypeUndefined ) return false;
    if ( recipient.isEmpty() || recipient.isNull() ) return false;

    return true;
  }

  virtual ~CallPrivate()
  {
  }
};
}
}
