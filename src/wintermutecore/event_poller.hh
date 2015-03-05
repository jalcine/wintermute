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
using namespace std::placeholders;

namespace Wintermute
{
namespace Events
{
struct PollerPrivate
{
  W_DEF_PUBLIC(Poller);
  int fd;
  Poller::PollDirection direction;
  Loop::Ptr loop;
  Emitter::Ptr emitter;
  function<void (Poller::Ptr& )> callback;
  uv_poll_t handle;

  explicit PollerPrivate(Poller::Ptr qPtr) :
    q_ptr(qPtr), fd(0), direction(Poller::PollReadable) { }

  void applyCallback(uv_loop_t* loopHdl)
  {
    assert(q_ptr);
    int r = 0;
    assert(fd > 0);
    r = uv_poll_init(loopHdl, &handle, fd);
    assert ( r == 0 );
    handle.data = this;

    callback = [](Poller::Ptr& pollerPtr) -> void
    {
      assert(pollerPtr);
      wdebug("Obtained a poll event from the provided FD: " + to_string(pollerPtr->fd()));
      Event::Ptr event = make_shared<PollEvent>(pollerPtr);
      pollerPtr->emitEvent(event);
    };
    assert(callback);
  }
};
}
}
