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

Message::~Message()
{ }
