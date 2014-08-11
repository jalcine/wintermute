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

/* Serves as the basis for message passing between modules.
 * Message is the data class that allows for passing of data
 * structures over ambigious means of data trasveral.
 *
 * @see Wintermute::Procedure::Dispatcher
 * @see Wintermute::Procedure::Receiver
 */
class WINTERMUTECORE_EXPORT Message : public Util::Serializable
{
  W_DEFINE_PRIVATE(Message)

public:
  /* A type mapping used to handle the data hash's key. */
  typedef string HashKey;

  /* A type mapping used to handle the data hash's value. */
  typedef string HashValue;

  /* A type mapping used to handle the data hash. */
  typedef Serializable::Map HashType;

  /* Builds a new Message with the provided data. */
  explicit Message(const Message::HashType& data,
                   const Module::Designation& receiver = Module::Designation(),
                   const Module::Designation& sender = Module::Designation());

  /* Copy constructor. */
  Message(const Message& other);

  /* Empty constructor. */
  Message();

  /* Destructor */
  virtual ~Message();

  /* Determines if this Message was crafted locally. */
  bool isLocal() const;

  /* Determines if this Message represents no data. */
  bool isEmpty() const;

  /* Copies the content of this Message into another one. */
  Message clone() const;

  /* Obtains the payload that's been held by this message. */
  HashType payload() const;

  /* Obtains the designation of the sending module. */
  Module::Designation sender() const;

  /* Obtains the designation of the receiving module. */
  Module::Designation receiver() const;

  /* Changes the sender of this message. */
  void setSender(const Module::Designation& newSender);

  /* Changes the receiver of this message. */
  void setReceiver(const Module::Designation& newReceiver);

  /* Changes the payload data that this Message contains. */
  void setPayload(const Message::HashType& newData);

  /* Equality operator. */
  bool operator==(const Message& other) const;

  /* Boolean evaluting operator. */
  bool operator!() const;

protected:
  virtual Serializable::Map serialize() const;
  virtual void deserialize(const Serializable::Map& data);
};
}

#endif
