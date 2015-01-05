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
#include "event_timer.hh"
#include "event_loop.hh"
#include "logging.hpp"

using namespace Wintermute::Events;

void w_event_timer_cb(uv_timer_t* handle)
{
  TimerPrivate* d = (TimerPrivate*) handle->data;
  assert(d);
  d->callback(make_shared<TimerPrivate>(*d));
  d->active = false;
}

Timer::Timer(const Loop::Ptr& loopPtr)
  : d_ptr(new TimerPrivate)
{
  if (!loopPtr)
  {
    throw std::invalid_argument("Provided an invalid loop pointer.");
  }

  W_PRV(Timer);
  d->q_ptr = make_shared<Timer>(*this);
  d->loop = loopPtr;
  d->emitter = make_shared<Emitter>(d->loop);
  d->applyCallback(loopPtr->d_ptr->loop);
}

Timer::~Timer()
{
  if (isActive())
  {
    stop();
  }
  // FIMXE: Stop the timer.
}

bool Timer::start(const uint64_t timeout)
{
  W_PRV(Timer);
  int r = 0;
  r = uv_timer_start(&d->handle, &w_event_timer_cb,
    timeout, interval());
  assert ( r == 0 );
  d->active = true;
  return r == 0;
}

bool Timer::stop()
{
  W_PRV(Timer);
  int r = 0;
  r = uv_timer_stop(&d->handle);
  assert ( r == 0 );

  d->active = false;
  return r == 0;
}

bool Timer::isActive() const
{
  W_PRV(const Timer);
  return d->active;
}

uint64_t Timer::interval() const
{
  W_PRV(const Timer);
  return uv_timer_get_repeat(&d->handle);
}

Emitter::Ptr Timer::emitter() const
{
  W_PRV(const Timer);
  return d->emitter;
}

void Timer::setInterval(const uint64_t theInterval)
{
  W_PRV(Timer);
  return uv_timer_set_repeat(&d->handle, theInterval);
}
