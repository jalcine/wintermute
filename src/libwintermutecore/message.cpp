#include <sys/types.h>
#include <unistd.h>
#include "message.hh"
#include "message.hpp"

using Wintermute::Procedure::Message;
using Wintermute::Procedure::MessagePrivate;

Message::Message(const Message::HashType& data) : d_ptr (new MessagePrivate)
{
  W_PRV(Message);
  d->data = data;
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

void Message::setSender(const Designation& newSender)
{
  // TODO: Use an assertion to prevent an empty sender.
}

void Message::setPayload(const Message::HashType& data){
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
  this->reciever = d->reciever;
}

bool MessagePrivate::isEmpty() const
{
  return false;
}
