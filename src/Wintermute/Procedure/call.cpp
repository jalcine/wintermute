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

#include "Wintermute/logging.hpp"
#include "Wintermute/application.hpp"
#include "Wintermute/Procedure/call.hpp"
#include "Wintermute/private/Procedure/message.hpp"
#include "Wintermute/Procedure/module.hpp"

using Wintermute::Procedure::Call;
using Wintermute::Procedure::Message;
using Wintermute::Procedure::Module;

Call::Call(const QString &name) : Message()
{
  // Make the map representing this call in dataMap.
  d->dataMap.insert("call", QVariantMap());
  d->dataMap["call"].toMap()["name"] = name;
}

void
Call::setName(const QString &newName)
{
  Q_ASSERT ( valid() );
  d->dataMap["call"].toMap()["name"] = newName;
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
  QVariant callData = d->dataMap.value("call");
  if (callData.isNull()) {
    return false;
  }
  if (!callData.isValid()) {
    return false;
  }
  if (!callData.toMap().contains("name")) {
    return false;
  }
  return true;
}

QVariantMap
Call::callData() const
{
  QVariant callData = d->dataMap["call"];
  Q_ASSERT ( callData.isValid() );
  Q_ASSERT ( !callData.isNull() );
  return callData.toMap();
}

QString
Call::name() const
{
  Q_ASSERT ( valid() );
  QVariant name = callData().value("name");
  Q_ASSERT ( !name.isNull() );
  Q_ASSERT ( name.isValid() );
  Q_ASSERT ( name.canConvert(QVariant::String) );
  return name.toString();
}
