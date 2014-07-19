#include <sys/types.h>
#include <unistd.h>
#include "message.hh"
#include "message.hpp"

using Wintermute::Message;
using Wintermute::Designation;
using Wintermute::MessagePrivate;

Message::Message(const Message::HashType& data, const Module::Designation& sender,
                 const Module::Designation& receiver) : d_ptr(new MessagePrivate)
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

void Message::setSender(const Module::Designation& newSender)
{
  // TODO: Use an assertion to prevent an empty sender.
  W_PRV(Message);
  d->sender = newSender;
}

void Message::setReceiver(const Module::Designation& newReceiver)
{
  // TODO: Use an assertion to prevent an empty receiver.
  W_PRV(Message);
  d->receiver = newReceiver;
}

void Message::setPayload(const Message::HashType& data)
{
  W_PRV(Message);
  d->data = data;
}

Message::~Message()
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
