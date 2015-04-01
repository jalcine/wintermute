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

#ifndef WINTERMUTE_DAEMON_MODULE_HH_
#define WINTERMUTE_DAEMON_MODULE_HH_

#include <uv.h>
#include <functional>
#include <wintermute-core/globals.hpp>
#include <wintermute-core/events.hpp>

namespace Wintermute
{
namespace Daemon
{
class ModulePrivate
{
public:
  Events::SignalHandler::Ptr signalHandler;

  explicit ModulePrivate();
  virtual ~ModulePrivate();
};
}
}

#endif
