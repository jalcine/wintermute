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
  function<void (uv_poll_t*, int, int)> callback;
  uv_poll_t handle;

  void applyCallback(uv_loop_t* loopHdl)
  {
    W_PUB(Poller);
    assert(fd);
    uv_poll_init_socket(loopHdl, &handle, fd);
    handle.data = q;

    callback = [&q, this](uv_poll_t* uvHandle, int events, int status) -> void {
      Poller::Ptr qPtr = ((Poller*)(uvHandle->data))->shared_from_this();
      assert(qPtr);

      wdebug("Obtained a poll event from the provided FD: " + to_string(qPtr->fd()));
      assert(uvHandle);
      assert(events);
      assert(status);

      q->emitEvent(make_shared<PollEvent>(qPtr));
    };
    assert(callback);
  }
};
}
}

