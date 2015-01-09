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

#include <unordered_map>
#include <string>
#include <stack>
#include "tunnel.hpp"

using std::unordered_map;
using std::stack;
using std::string;

namespace Wintermute
{
class TunnelPrivate
{
  public:
    typedef unordered_map<string, Tunnel::Dispatcher::Ptr> DispatcherMap;
    typedef unordered_map<string, Tunnel::Receiver::Ptr> ReceiverMap;
    typedef stack<Message> MessageQueue;
    explicit TunnelPrivate();
    virtual ~TunnelPrivate();

    DispatcherMap dispatchers;
    ReceiverMap receivers;
    MessageQueue obtainedMessages;
    Events::Emitter::Ptr emitter;
};
}
