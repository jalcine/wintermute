/*
 * Wintermute is free software; you can redistribute it and/or * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with Wintermute; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <algorithm>
#include "message.hh"
#include "message.hpp"
#include "logging.hpp"

using Wintermute::Message;
using Wintermute::Module;
using Wintermute::MessagePrivate;
using Wintermute::Util::Serializable;

Message::Message(const Message::HashType& data,
                 const Module::Designation& receiver,
                 const Module::Designation& sender) :
  Serializable(), d_ptr(new MessagePrivate)
{
  setPayload(data);
  setSender(sender);
  setReceiver(receiver);
}

Message::Message(const Message& other) : Serializable(), d_ptr (new MessagePrivate)
{
  W_PRV(Message);
  d->clone(other.d_ptr);
}

Message::Message() : Serializable(), d_ptr (new MessagePrivate)
{
}

bool Message::isLocal() const
{
  W_PRV(Message);
  return d->sender.pid() == getpid();
}

bool Message::isEmpty() const
{
  W_PRV(Message);
  return !d->isEmpty();
}

Message Message::clone() const
{
  Message clonedMessage;
  clonedMessage.d_ptr->clone(d_ptr);
  return clonedMessage;
}

Message::HashType Message::payload() const
{
  W_PRV(Message);
  return d->data;
}

Module::Designation Message::sender() const
{
  W_PRV(Message);
  return d->sender;
}

Module::Designation Message::receiver() const
{
  W_PRV(Message);
  return d->receiver;
}

void Message::setSender(const Module::Designation& newSender)
{
  W_PRV(Message);
  assert(!newSender.isNull() || newSender.isNull());
  d->sender = newSender;
}

void Message::setReceiver(const Module::Designation& newReceiver)
{
  W_PRV(Message);
  assert(!newReceiver.isNull() || newReceiver.isNull());
  d->receiver = newReceiver;
}

void Message::setPayload(const Message::HashType& data)
{
  W_PRV(Message);
  assert(!data.empty() || d->data.empty());
  d->data = data;
}

Serializable::Map Message::serialize() const
{
  Serializable::Map serializedMessage;
  Serializable::Map boostedMap;
  auto payloadMap = payload();

  for (auto itr = payloadMap.cbegin(); itr != payloadMap.cend(); itr++)
  {
    boostedMap.insert(std::make_pair(itr->first, itr->second));
  }

  const std::string payloadString = Serializable::toString(boostedMap);

  serializedMessage.insert(std::make_pair("sender", sender()));
  serializedMessage.insert(std::make_pair("receiver", receiver()));
  serializedMessage.insert(std::make_pair("payload", payloadString));
  return serializedMessage;
}

void Message::deserialize(const Serializable::Map& data)
{
  const string payloadString = data.at("payload");
  const string senderString = data.at("sender");
  const string receiverString = data.at("receiver");
  assert(!payloadString.empty());
  assert(!senderString.empty());
  assert(!receiverString.empty());

  const Serializable::Map payloadMap = Serializable::fromString(payloadString);
  assert(!payloadMap.empty());

  const Module::Designation senderDes(senderString);
  const Module::Designation receiverDes(receiverString);
  assert(!senderDes.isNull());
  assert(!receiverDes.isNull());

  setPayload(payloadMap);
  setReceiver(receiverDes);
  setSender(senderDes);
}

bool Message::operator!() const
{
  return !isEmpty();
}

bool Message::operator==(const Message& other) const
{
  return other.sender() == sender() &&
         other.receiver() == receiver() &&
         other.payload() == other.payload();
}

Message::~Message()
{
}

MessagePrivate::MessagePrivate() : data(Message::HashType())
{
}

void MessagePrivate::clone(const SharedPtr<MessagePrivate>& d)
{
  this->data = d->data;
  this->sender = d->sender;
  this->receiver = d->receiver;
}

bool MessagePrivate::isEmpty() const
{
  /// TODO This needs a better check. If anything, check only if the sender AND
  //payload are empty/null.
  return !sender.isNull() && !receiver.isNull() && !data.empty();
}
