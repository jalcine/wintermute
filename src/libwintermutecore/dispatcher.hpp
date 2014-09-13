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

#ifndef WINTERMUTE_CORE_DISPATCHER_HPP
#define WINTERMUTE_CORE_DISPATCHER_HPP

#include <string>
#include "globals.hpp"

using std::string;

namespace Wintermute
{
class DispatcherPrivate;
class Message;
/* Serves as an abstract basis for sending messages. */
class Dispatcher :
  W_DECLARE_SHAREABLE(Dispatcher)
{

public:
  W_DECLARE_PTR_TYPE(Dispatcher)
  explicit Dispatcher();
  virtual ~Dispatcher();

  virtual string name() const = 0;
  virtual bool send(const Message& message) = 0;
};
}

#endif
