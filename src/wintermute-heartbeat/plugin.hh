/*
   Wintermute - A foundation for intelligent computing.
   Copyright (c) 2010 - 2015 by Jacky Alcine

   Wintermute is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   Wintermute is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with Wintermute; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
   */

#ifndef WINTERMUTE_HEARTBEAT_PLUGIN_HH_
# define WINTERMUTE_HEARTBEAT_PLUGIN_HH_

#include <wintermutecore/events.hpp>
#include "globals.hpp"
#include "module.hpp"
#include "plugin.hpp"

namespace Wintermute
{
  namespace Heartbeat {
    class PluginPrivate
    {
      public:
        explicit PluginPrivate();
        ~PluginPrivate();
        /*Heartbeat::Module::Ptr module;*/
        Events::Timer::Ptr timer;
        Heartbeat::Plugin::Mode mode;
    };
  }
}

#endif
