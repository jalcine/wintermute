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

#include <uv.h>
#include <functional>
#include "logging.hpp"
#include "events.hpp"

using std::function;
using std::to_string;

namespace Wintermute
{
namespace Events
{
class TimerPrivate
{
public:
  W_DEF_PUBLIC(Timer);
  Loop::Ptr loop;
  Emitter::Ptr emitter;
  function<void (SharedPtr<TimerPrivate>)> callback;
  uv_timer_t handle;
  bool active;

  TimerPrivate() : loop(nullptr), emitter(nullptr), callback(nullptr),
    handle(), active(false) { }

  void applyCallback(uv_loop_t* loopHdl)
  {
    W_PUB(Timer);
    int r = 0;
    r = uv_timer_init(loopHdl, &handle);
    assert ( r == 0 );
    handle.data = this;

    callback = [&q](SharedPtr<TimerPrivate> dPtr) -> void
    {
      Timer::Ptr qPtr = dPtr->q_ptr;
      qPtr->emitEvent(make_shared<TimerEvent>(qPtr));
    };

    assert(callback);
  }
};
}
}
