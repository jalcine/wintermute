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
class PollerPrivate
{
public:
  W_DEF_PUBLIC(Poller);
  int fd;
  Poller::PollDirection direction;
  Loop::Ptr loop;
  Emitter::Ptr emitter;
  function<void (Poller::Ptr& )> callback;
  SharedPtr<uv_poll_t> handle;

  explicit PollerPrivate(Poller::Ptr qPtr = nullptr) :
    q_ptr(qPtr), fd(0), direction(Poller::PollReadable), loop(nullptr),
    emitter(nullptr), callback(nullptr), handle(nullptr)
  {
  }

  void applyCallback(uv_loop_t* loopHdl)
  {
    assert(this);
    assert(q_ptr && q_ptr.get() != NULL);
    assert(loopHdl);
    assert(fd > 0);

    int r = 0;
    r = uv_poll_init_socket(loopHdl, handle.get(), fd);
    W_CHECK_UV(r, "uv_poll_init_socket");
    handle->data = this;
    assert(handle->data);

    callback = [](Poller::Ptr& pollerPtr) -> void
    {
      assert(pollerPtr.get() != NULL);
      wdebug("Obtained a poll event from the provided file descriptor: "
        + to_string(pollerPtr->fd()));

      PollEvent::Ptr event = make_shared<PollEvent>(pollerPtr);
      assert(event && event.get() != NULL);

      pollerPtr->emitEvent(event);
    };
    assert(callback);
  }
};
}
}
