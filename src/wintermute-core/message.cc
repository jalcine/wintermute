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

#include <algorithm>
#include "message.hh"
#include "message.hpp"
#include "logging.hpp"

using Wintermute::Message;
using Wintermute::Module;
using Wintermute::MessagePrivate;
using Wintermute::Util::Serializable;
using std::chrono::high_resolution_clock;

MessagePrivate::MessagePrivate() :
  data(Message::HashType()),
  sender(),
  receiver(),
  timestamp(time(nullptr))
{
}

void MessagePrivate::clone(const SharedPtr<MessagePrivate>& d)
{
  this->data = d->data;
  this->sender = d->sender;
  this->receiver = d->receiver;
  this->timestamp = d->timestamp;
}

bool MessagePrivate::isEmpty() const
{
  /// TODO This needs a better check. If anything, check only if the sender AND
  //payload are empty/null.
  return !sender.isNull() && !receiver.isNull() &&
         !data.empty() && timestamp != 0;
}
