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

#include "events.hpp"
#include <uv.h>

using namespace Wintermute::Events;

namespace Wintermute
{
namespace Events
{
class LoopPrivate
{
public:
  LoopPrivate() : loop(NULL)
  {
  }

  void useDefaultLoop()
  {
    // NOTE: Should we clear out 'loop' if it's already there?
    loop = uv_default_loop();
    assert(loop);
  }

  void createNewLoop()
  {
    // NOTE: Should we clear out 'loop' if it's already there?
    loop = new uv_loop_t;
    uv_loop_init(loop);
    assert(loop);
  }

  ~LoopPrivate()
  {
    // TODO: Close the loop if it's active.
  }

  uv_loop_t* loop;
};
}
}
