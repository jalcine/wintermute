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

#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QByteArray>
#include "Wintermute/Procedure/message.hpp"
#include "Wintermute/Procedure/dispatcher.hpp"
#include "Wintermute/private/Procedure/message.hpp"

using Wintermute::Procedure::Message;
using Wintermute::Procedure::Dispatcher;
using Wintermute::Procedure::Designation;
using Wintermute::Procedure::MessagePrivate;

const int Message::MetaTypeId =
  qRegisterMetaType<Message>("Wintermute::Procedure::Message");

QDataStream& operator<<(QDataStream& out, const Message& myObj)
{
  Q_ASSERT ( myObj.valid() );
  out << myObj.rawData();
  return out;
}

QDataStream& operator>>(QDataStream& in, Message& myObj)
{
  QVariantList maps;
  in >> maps;
  myObj.d->sender   = maps[0].value<Designation>();
  myObj.d->receiver = maps[1].value<Designation>();
  myObj.d->dataMap  = maps[2].toMap();
  Q_ASSERT ( myObj.valid() );
  return in;
}

Message::Message() : d ( new MessagePrivate )
{ }

Message::Message(const Message& other) : d ( other.d )
{ }

Message::operator QString() const
{
  return toString();
}

Message::operator QVariant() const
{
  return toString();
}

Message::operator const char* () const
{
  return toString().toStdString().data();
}

QVariantMap
Message::rawData() const
{
  QVariantMap theFields;
  theFields.insert("sender", static_cast<QString>(d->sender));
  theFields.insert("receiver", static_cast<QString>(d->receiver));
  theFields.insert("data", d->dataMap);
  return theFields;
}

QString
Message::toString() const
{
  Q_ASSERT ( valid() );
  QJson::Serializer jsonSerializer;
  return jsonSerializer.serialize(rawData(), 0);
}

Message
Message::fromString(const QString& string)
{
  Q_ASSERT ( string.isNull() );
  Q_ASSERT ( !string.isEmpty() );
  QJson::Parser jsonParser;
  Message msg;
  QVariantMap map = jsonParser.parse(string.toLocal8Bit()).toMap();
  const QString senderString = map.value("sender").toString();
  const QString receiverString = map.value("receiver").toString();
  msg.d->sender = Designation::fromString(senderString);
  msg.d->receiver = Designation::fromString(receiverString);
  msg.d->dataMap = map.value("data").toMap();
  Q_ASSERT ( msg.valid() );
  return msg;
}

bool
Message::valid() const
{
  const bool isValid = d->valid();
  Q_ASSERT ( isValid == true );
  return isValid;
}

bool
Message::isLocal() const
{
  return d->sender.isLocal();
}

const Designation&
Message::receivingModule() const
{
  Q_ASSERT ( valid() );
  Q_ASSERT ( d->receiver.valid() );
  return d->receiver;
}

const Designation&
Message::sendingModule() const
{
  Q_ASSERT ( valid() );
  Q_ASSERT ( d->sender.valid() );
  return d->sender;
}

void
Message::setData ( const QVariant& newData )
{
  Q_ASSERT ( newData.isValid() );
  Q_ASSERT ( !newData.isNull() );
  d->dataMap = newData.toMap();
}

void
Message::setReceiver( const Designation& newReceiver )
{
  Q_ASSERT ( newReceiver.isNull() == false );
  Q_ASSERT ( newReceiver.valid() == true );
  d->receiver = newReceiver;
}

void
Message::setSender( const Designation& newSender )
{
  Q_ASSERT ( newSender.isNull() == false );
  Q_ASSERT ( newSender.valid() == true );
  d->sender = newSender;
  qDebug() << "Set" << newSender << "to this message.";
}

void
Message::queueForDispatch() const
{
  Q_ASSERT ( valid() == true );

  if ( valid() && isLocal() ) {
    wtrace("(Message)", QString("Queued %1 for dispatch.").arg(*this));
    Dispatcher::queueMessage ( *this );
  }
}

Message::~Message()
{
}
