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

#ifndef WINTERMUTE_DAEMON_PLUGIN_HPP_
# define WINTERMUTE_DAEMON_PLUGIN_HPP_

#include <wintermute-core/plugin.hpp>
#include "globals.hpp"

namespace Wintermute
{
namespace Daemon
{
class PluginPrivate;
class Plugin : public Wintermute::Plugin
{
  W_DEF_PRIVATE(Wintermute::Daemon::Plugin)

  public:
    explicit Plugin();
    virtual ~Plugin();
    virtual bool startup();
    virtual bool shutdown();
    virtual Plugin::PluginType type() const;

  private:
    void startHeartbeatInstance();
    void stopHeartbeatInstance();
    void startModule();
    void stopModule();
    void startDesignatedPlugins();
    void stopDesignatedPlugins();
};
}
}

W_DECL_PLUGIN(Wintermute::Daemon::Plugin, WINTERMUTE_DAEMON_VERSION)

#endif
