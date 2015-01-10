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
#include "events.hpp"
#include "event_poller.hh"
#include "event_loop.hh"
#include "logging.hpp"

using namespace Wintermute::Events;

Poller::Poller(Poller::FileDescriptor aFd, Poller::PollDirection aDirection,
  const Loop::Ptr& loopPtr) : d_ptr(new PollerPrivate)
{
  if (aFd == 0)
  {
    throw std::invalid_argument("Provided an invalid file descriptor.");
  }

  if (!loopPtr)
  {
    throw std::invalid_argument("Provided an invalid loop pointer.");
  }

  W_PRV(Poller);
  // FIXME: This is (possibly) causing a std::bad_weak_ptr to be risen.
  d->q_ptr = shared_from_this();
  d->emitter = make_shared<Emitter>(loopPtr);
  d->fd = aFd;
  d->loop = loopPtr;
  d->direction = aDirection;
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
  return d->loop;
}

bool Poller::start()
{
  W_PRV(Poller);
  int r = 0;
  uv_poll_cb cb = (uv_poll_cb) d->callback.target<void(uv_poll_t*, int, int)>();
  assert(d->callback);
  assert(cb);
  r = uv_poll_start(&d->handle, d->direction, cb);
  wdebug(to_string(r));
  return r == 0;
}

bool Poller::stop()
{
  W_PRV(Poller);
  int r;
  r = uv_poll_stop(&d->handle);
  return r == 0;
}
