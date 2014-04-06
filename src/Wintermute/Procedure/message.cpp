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

#include <QtCore/QCoreApplication>
#include "Wintermute/Procedure/message.hpp"
#include "Wintermute/private/Procedure/message.hpp"

using Wintermute::Procedure::Message;
using Wintermute::Procedure::MessagePrivate;

const int Message::MetaTypeId =
  qRegisterMetaType<Message>("Wintermute::Procedure::Message");

QDataStream &operator<<(QDataStream &out, const Message &myObj)
{
  out << myObj.rawData();
  return out;
}

QDataStream &operator>>(QDataStream &in, Message &myObj)
{
  QVariantList maps;
  in >> maps;
  myObj.d->senderMap = maps[0].toMap();
  myObj.d->receiverMap = maps[1].toMap();
  myObj.d->dataMap = maps[2].toMap();
  Q_ASSERT ( myObj.valid() );
  return in;
}

Message::Message() : d ( new MessagePrivate )
{ }

Message::Message(const Message &other) : d ( other.d )
{ }

Message::operator QString() const
{
  return Message::operator QVariant().toString();
}

Message::operator QVariant() const
{
  return QVariant::fromValue(*this);
}

Message::operator const char *() const
{
  return Message::operator QString().toStdString().data();
}

QVariantList
Message::rawData() const
{
  return QVariantList() << d->senderMap << d->receiverMap << d->dataMap;
}

QString
Message::toString() const
{
  return Message::operator QString();
}

bool
Message::valid() const
{
  Q_ASSERT ( d->valid() );
  return d->valid();
}

bool
Message::isLocal() const
{
  const quint64 localPid = QCoreApplication::applicationPid(),
                aPid = d->senderMap.value("pid").toUInt()
                       ;
  Q_ASSERT(localPid == aPid);
  return (localPid == aPid);
}

const QString
Message::receivingModuleName() const
{
  Q_ASSERT ( valid() );
  QVariant name = d->receiverMap.value("module");
  Q_ASSERT ( name.isValid() );
  Q_ASSERT ( !name.isNull() );
  Q_ASSERT ( name.canConvert(QVariant::String) );
  return name.toString();
}

const QString
Message::sendingModuleName() const
{
  Q_ASSERT ( valid() );
  QVariant name = d->senderMap.value("module");
  Q_ASSERT ( name.isValid() );
  Q_ASSERT ( !name.isNull() );
  Q_ASSERT ( name.canConvert(QVariant::String) );
  return name.toString();
}

Message::~Message()
{ }
