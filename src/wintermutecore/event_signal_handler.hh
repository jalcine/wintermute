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

#ifndef WINTERMUTE_EVENT_SIGNAL_HANDLER_HH_
#define WINTERMUTE_EVENT_SIGNAL_HANDLER_HH_

#include <uv.h>
#include "events.hpp"

namespace Wintermute
{
namespace Events
{
class SignalHandlerPrivate
{
  public:
    W_DEF_PUBLIC(SignalHandler);
    typedef uv_signal_t SignalType;
    typedef SharedPtr<SignalType> SignalPtr;

    explicit SignalHandlerPrivate(const int& signum);
    virtual ~SignalHandlerPrivate();
    void attachSignal();

    SignalPtr handle;
    int signum;
    Emitter::Ptr emitter;
    SignalHandler* qptr;
};
}
}

void w_event_signal_cb(uv_signal_t* handle, int signum);

#endif
