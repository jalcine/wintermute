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

#include <string>
#include <limits>
#include "events.hpp"
#include "event_poller.hh"
#include "event_loop.hh"
#include "logging.hpp"

using namespace Wintermute::Events;

void w_event_poller_cb(uv_poll_t* handle, int status, int events)
{
  assert(handle);
  //assert(status && status < std::numeric_limits<int>::max());

  if (status == 0)
  {
    wdebug("Found " + to_string(events) + " polling events available from libuv.");
    PollerPrivate* dptr = (PollerPrivate*) handle->data;
    assert(dptr);
    dptr->callback(dptr->q_ptr);
  }
  else
  {
    werror("Error polling: " + string(uv_strerror(status)) + string(uv_err_name(status)));
  }
}

Poller::Poller(const Poller::FileDescriptor& aFd,
  const Poller::PollDirection& aDirection,
  const Loop::Ptr& loopPtr) : d_ptr(new PollerPrivate)
{
  assert(d_ptr && d_ptr.get() != NULL);

  if (aFd == 0)
  {
    throw std::invalid_argument("Provided an invalid file descriptor.");
  }

  if ((int) aDirection > 2 || (int) aDirection < 0)
  {
    throw std::invalid_argument("Provided an invalid direction to listen for.");
  }

  if (!loopPtr)
  {
    throw std::invalid_argument("Provided an invalid loop pointer.");
  }

  W_PRV(Poller);
  d->q_ptr = make_shared<Poller>(*this);
  d->emitter = make_shared<Emitter>(loopPtr);
  d->handle.reset(new uv_poll_t);
  d->fd = aFd;
  d->loop = loopPtr;
  d->direction = aDirection;
  assert(d->handle);

  d->applyCallback(loopPtr->d_ptr->loop);
}

Emitter::Ptr Poller::emitter() const
{
  W_PRV(Poller);
  return d->emitter;
}

Poller::~Poller()
{
  stop();
}

Poller::FileDescriptor Poller::fd() const
{
  W_PRV(const Poller);
  return d->fd;
}

Poller::PollDirection Poller::direction() const
{
  W_PRV(const Poller);
  return d->direction;
}

Loop::Ptr Poller::loop() const
{
  W_PRV(const Poller);
  assert(d->loop);
  return d->loop;
}

bool Poller::start()
{
  if (isActive()) return true;
  W_PRV(Poller);
  int r = 0;
  const uv_poll_event uvDir = static_cast<uv_poll_event>(d->direction);
  assert(d->handle);

  wdebug("Starting poller for file descriptor " + to_string(fd()) + " in "
      + to_string(uvDir) + "...");

  r = uv_poll_start(d->handle.get(), uvDir, &w_event_poller_cb);
  W_CHECK_UV(r, "uv_poll_start");

  wdebug("Was poller for file descriptor " + to_string(fd()) + " started? " +
    to_string( r == 0 ));
  return (r == 0) && isActive();
}

bool Poller::stop()
{
  if (!isActive()) return true;
  W_PRV(Poller);
  wdebug("Stopping poller for file descriptor " + to_string(fd()) + "...");

  int r = 0;
  r = uv_poll_stop(d->handle.get());
  W_CHECK_UV(r, "uv_poll_stop");

  wdebug("Was poller for file descriptor " + to_string(fd()) + " stopped? " +
    to_string( r == 0 ));
  return r == 0;
}

bool Poller::isActive() const
{
  W_PRV(const Poller);
  assert(d->handle);
  auto hdl = (const uv_handle_t*) d->handle.get();
  return uv_is_active(hdl) > 0;
}
