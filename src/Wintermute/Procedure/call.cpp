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

#include "Wintermute/Procedure/call.hpp"
#include "Wintermute/private/Procedure/message.hpp"
#include "Wintermute/Procedure/module.hpp"

using Wintermute::Procedure::Call;
using Wintermute::Procedure::Message;
using Wintermute::Procedure::Module;

Call Call::Null("");

Call::Call(const QString& name) : Message()
{
  QVariantMap initialCallMap;
  initialCallMap.insert("name",name);
  setCallData(initialCallMap);
}

Call::Call(const Call& other) : Message(other)
{
}

Call::Call(const Message& other) : Message(other)
{
}

void
Call::setName(const QString& newName)
{
  Q_ASSERT ( valid() );
  QVariantMap newNameMap;
  newNameMap.insert("name", newName);
  setCallData(newNameMap);
}

void
Call::setCallData(const QVariantMap& newData)
{
  Q_ASSERT ( !newData.empty() );

  if ( d->dataMap.contains("call") ) {
    d->dataMap.insert("call", d->dataMap.value("call").toMap().unite(newData));
  } else {
    d->dataMap.insert("call", newData);
  }
}

bool
Call::valid() const
{
  // If it's not valid upstream or we don't exist, forget it.
  if (!Message::valid()) {
    return false;
  }

  if (!d->dataMap.contains("call")) {
    return false;
  }

  QVariant callVariant = d->dataMap.value("call");

  if (callVariant.isNull()) {
    return false;
  }

  if (!callVariant.isValid()) {
    return false;
  }

  if (!callVariant.toMap().contains("name")) {
    return false;
  }

  return true;
}

QVariantMap
Call::callData() const
{
  QVariant callData = d->dataMap.value("call");
  Q_ASSERT ( callData.isValid() );
  Q_ASSERT ( !callData.isNull() );
  return callData.value<QVariantMap>();
}

QString
Call::name() const
{
  Q_ASSERT ( valid() );
  return callData().value("name").toString();
}

Call::~Call()
{}
