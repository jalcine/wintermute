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
#include "Wintermute/Procedure/message.hpp"
#include "Wintermute/Procedure/dispatcher.hpp"
#include "Wintermute/private/Procedure/message.hpp"

using Wintermute::Procedure::Module;
using Wintermute::Procedure::Message;
using Wintermute::Procedure::Dispatcher;
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
  myObj.d->sender   = maps[0].value<Module::Definition>();
  myObj.d->receiver = maps[1].value<Module::Definition>();
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
  msg.d->sender = Module::Definition::fromString(senderString);
  msg.d->receiver = Module::Definition::fromString(receiverString);
  msg.d->dataMap = map.value("data").toMap();
  Q_ASSERT ( msg.valid() );
  return msg;
}

bool
Message::valid() const
{
  const bool isValid = d->valid();
  Q_ASSERT ( isValid );
  return isValid;
}

bool
Message::isLocal() const
{
  const qint64 appId = QCoreApplication::applicationPid(),
               recvId = d->receiver.pid, sendId = d->sender.pid;
  Q_ASSERT ( sendId == appId );
  Q_ASSERT ( appId != recvId || recvId == 0);

  if ( sendId == appId && (appId != recvId || recvId == 0) ) {
    return true;
  }

  return false;
}

const Module::Definition&
Message::receivingModule() const
{
  Q_ASSERT ( valid() );
  Q_ASSERT ( d->receiver.valid() );
  return d->receiver;
}

const Module::Definition&
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
Message::setReceiver( const Module::Definition& newReceiver )
{
  Q_ASSERT ( newReceiver.valid() );
  Q_ASSERT ( newReceiver.pid != (quint64) QCoreApplication::applicationPid() );
  d->receiver = newReceiver;
}

void
Message::setSender( const Module::Definition& newSender )
{
  Q_ASSERT ( newSender.valid() );
  d->sender = newSender;
}

void
Message::queueForDispatch() const
{
  Q_ASSERT ( valid() );

  if ( valid() && isLocal() ) {
    wtrace("(Message)", QString("Queued %1 for dispatch.").arg(*this));
    Dispatcher::queueMessage ( *this );
  }
}

Message::~Message()
{
}
