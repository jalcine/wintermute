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
#include <QtCore/QBitArray>
#include <QtCore/QCache>
#include <QtCore/QDateTime>
#include "Wintermute/Globals"
#include "Wintermute/application.hpp"
#include "Wintermute/logging.hpp"
#include "Wintermute/Procedure/call.hpp"
#include "Wintermute/Procedure/module.hpp"

namespace Wintermute
{
namespace Procedure
{
class Module;
class CallPrivate : public QSharedData
{
public:
  typedef QSharedDataPointer<CallPrivate> Pointer;

  QString recipient;
  QString name;
  QVariantMap data;
  QDateTime id;
  Call* q_ptr;
  Call::Type type;

  /**
   * @brief Creates a new CallPrivate object.
   */
  explicit CallPrivate ( Call* q = nullptr ) :
    recipient ( QString::null ), name ( QString::null ), data ( ),
    id ( QDateTime::currentDateTimeUtc() ), q_ptr ( q ),
    type ( Call::TypeUndefined ) { }

  /**
   * @brief Copies an existing CallPrivate object.
   * @note  The ID and callback are *not* copied.
   */
  CallPrivate ( const CallPrivate& other, Call* q ) :
    QSharedData ( other ), recipient ( other.recipient ), name ( other.name ),
    data ( other.data ), id ( QDateTime::currentDateTimeUtc() ), q_ptr ( q ),
    type ( other.type ) { }

  /**
   * @dtor
   */
  virtual ~CallPrivate() { }

  /**
   * @function fromVariantMap
   * @param    data Map representing data to fill up a CallPrivate.
   */
  static CallPrivate::Pointer fromVariantMap ( const QVariantMap& data ) {
    CallPrivate::Pointer d_ptr ( new CallPrivate );
    Q_ASSERT ( !data.empty() );
    Q_ASSERT ( data.contains("type") );
    Q_ASSERT ( data.contains("timestamp") );
    Q_ASSERT ( data.contains("recipient") );
    Q_ASSERT ( data.contains("data") );
    d_ptr->id = data["timestamp"].toDateTime();
    d_ptr->type = ( Call::Type ) data["type"].toInt();
    d_ptr->recipient = data["recipient"].toString();
    d_ptr->data = data["data"].toMap();
    return d_ptr.data() != nullptr && d_ptr->isValid() ?
           d_ptr : CallPrivate::Pointer ( nullptr );
  }

  /**
   * @function toVariantMap
   * @return   QVariantMap A map of data that represents this CallPrivate.
   */
  QVariantMap toVariantMap() const {
    QVariantMap callData;
    callData["type"] = (int) type;
    callData["timestamp"] = id;
    callData["recipient"] = recipient;
    callData["data"] = data;
    return callData;
  }

  virtual bool isValid() const {
    const bool validId        = !id.isNull() && id.isValid(),
               validRecipient = !recipient.isNull() && !recipient.isEmpty()
                                ;
    return ( validId && validRecipient );
  }
};
}
}
