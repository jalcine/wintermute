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

#include "logging.hpp"
#include "events.hpp"
#include "event_loop.hh"

using namespace Wintermute::Events;

Loop::Loop(const bool useDefault) : d_ptr(make_shared<LoopPrivate>())
{
  W_PRV(Loop);
  useDefault ? d->useDefaultLoop() : d->createNewLoop();
  assert(d->loop);
  d->loop->data = this;
  wdebug("Crafted loop.");
}

Loop::~Loop()
{
  wdebug("Loop destroyed.");
}

bool Loop::isRunning() const
{
  W_PRV(Loop);
  assert(d->loop);
  const bool isActive = (uv_loop_alive(d->loop) > 0);
  return isActive;
}

bool Loop::isPrimary() const
{
  W_PRV(const Loop);
  assert(d->loop);
  const bool isDefault = (uv_default_loop() == d->loop);
  return isDefault;
}

bool Loop::run()
{
  W_PRV(Loop);
  assert(d->loop);
  wdebug("Running loop...");
  const int resultRun = uv_run(d->loop, UV_RUN_DEFAULT);
  wdebug("Ran loop.");
  return resultRun == 0;
}

bool Loop::stop()
{
  W_PRV(Loop);
  bool wasStopped = false;
  wdebug("Stopping loop...");
  uv_stop(d->loop);

  wdebug("Loop stopped; closing loop...");
  //const int resultRun = uv_loop_close(d->loop);
  //W_CHECK_UV(resultRun, "uv_loop_close");
  wasStopped = true;

  wdebug("Loop closed.");
  return wasStopped;
}

Loop::Ptr Loop::primary()
{
  Loop::Ptr primaryLoop = make_shared<Loop>(true);
  assert(primaryLoop->isPrimary());
  wdebug("Handle to primary loop obtained.");
  return primaryLoop;
}
