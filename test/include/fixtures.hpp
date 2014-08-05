#include "libwintermutecore/module.hpp"
#include "libwintermutecore/message.hpp"

using Wintermute::Message;
using Wintermute::Module;

Module::Designation craftRandomDesignation()
{
  return Module::Designation("local", "me.jalcine");
}

Message craftRandomMessage()
{
  Message aMsg;
  aMsg.setSender(craftRandomDesignation());
  aMsg.setReceiver(craftRandomDesignation());
  return aMsg;
}
