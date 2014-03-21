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
#include <QtCore/QHash>
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
  typedef QHash<quint64, Call*> CallCache;
  static CallCache calls;
  Call* q_ptr;
  QString recipient;
  QString name;
  Call::Type type;
  QVariantMap data;
  QDateTime id;
  Call::CallbackSignature callback;

  explicit CallPrivate ( Call* q ) : q_ptr ( q ),
    recipient( wntrApp->processName() ), name( QString::null), 
    type ( Call::TypeUndefined ), data(), 
    id ( QDateTime::currentDateTimeUtc() ), callback ( nullptr )
  {
    if ( q_ptr != nullptr ) CallPrivate::calls.insert ( id.toTime_t(), q_ptr );
    data["timestamp"] = id.toTime_t();
  }

  static CallPrivate* fromVariantMap (const QVariantMap& data)
  {
    CallPrivate *d_ptr = new CallPrivate ( nullptr );
    d_ptr->type      = (Call::Type) data["type"].toUInt();
    d_ptr->id        = data["timestamp"].toDateTime();
    d_ptr->recipient = data["recipient"].toString();
    d_ptr->data      = data["data"].toMap();
    
    return ( d_ptr->isValid() ? d_ptr : nullptr );
  }

  QVariantMap toVariantMap() const
  {
    QVariantMap callData;
    callData["type"]      = (quint64) type;
    callData["timestamp"] = id.toTime_t();
    callData["recipient"] = recipient;
    callData["data"]      = data;

    return callData;
  }

  virtual bool isValid() const
  {
    if ( id.isNull() ) return false;
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
