/*
 * Wintermute is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
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
#include "events.hpp"

using std::unordered_multimap;

namespace Wintermute
{
namespace Events
{
class EmitterPrivate
{
public:
  Loop::Ptr loop;
  unordered_multimap<string, Listener::Ptr> listeners;

  void queueEventForListener(const Listener::Ptr& listener, const Event::Ptr& event)
  {
    assert(listener);
    assert(event);
    // TODO: Invoke the listener here.
  }
};
}
}

