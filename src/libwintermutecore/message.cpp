#include <sys/types.h>
#include <unistd.h>
#include "message.hh"
#include "message.hpp"

using Wintermute::Procedure::Message;

Message::Message() : d (new MessagePrivate)
{
}

bool Message::isLocal() const
{
  return d->sender.pid() == getpid();
}

Message Message::clone() const
{
  Message clonedMessage;
  clonedMessage.d->clone(d);
  return clonedMessage;
}

Message::~Message()
{
}
