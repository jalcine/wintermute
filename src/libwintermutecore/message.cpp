#include "message.hh"
#include "message.hpp"

using Wintermute::Message;
using Wintermute::Module;
using Wintermute::MessagePrivate;

Message::Message(const Message::HashType& data,
                 const Module::Designation& receiver,
                 const Module::Designation& sender) : d_ptr(new MessagePrivate)
{
  setPayload(data);
  setSender(sender);
  setReceiver(receiver);
}

Message::Message(const Message& other) : d_ptr (new MessagePrivate)
{
  W_PRV(Message);
  d->clone(other.d_ptr);
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
  return false;
}
