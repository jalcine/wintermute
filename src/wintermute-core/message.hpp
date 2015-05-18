/*
 * Wintermute is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
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

#ifndef WINTERMUTE_MESSAGE_HPP
#define WINTERMUTE_MESSAGE_HPP

#include <string>
#include <map>
#include "globals.hpp"
#include "module.hpp"
#include "util/serializable.hpp"

using std::string;
using std::map;

namespace Wintermute
{
class MessagePrivate;

/**
 * Serves as the basis for message passing between modules.
 * Message is the data class that allows for passing of data
 * structures over ambigious means of data trasveral.
 *
 * @sa Wintermute::Tunnel::Dispatcher
 * @sa Wintermute::Tunnel::Receiver
 */
class WINTERMUTE_EXPORT_PUBLIC Message :
  public Util::Serializable
{
private:
  W_DEF_PRIVATE(Message)
  W_SERIALIZABLE(Message)

public:
  typedef string HashKey;
  typedef string HashValue;
  typedef Serializable::Map HashType;

  explicit Message(const Message::HashType& data,
    const Module::Designation& receiver = Module::Designation(),
    const Module::Designation& sender = Module::Designation());

  Message(const Message& other);

  Message();

  virtual ~Message();

  bool isEmpty() const;

  Message clone() const;

  HashType payload() const;

  Module::Designation sender() const;

  Module::Designation receiver() const;

  string timestamp() const;

  void setSender(const Module::Designation& newSender);

  void setReceiver(const Module::Designation& newReceiver);

  void setPayload(const Message::HashType& newData);

  bool operator==(const Message& other) const;

  bool operator!() const;

  using Serializable::operator=;
};
}

#endif
