/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2014 Jacky Alciné <me@jalcine.me>
 *
 * Wintermute is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "Wintermute/private/plugin_process.hpp"
#include "plugin.hpp"
#include "plugin_process.hpp"
#include "plugin_process.moc"

using Wintermute::Plugin;
using Wintermute::PluginProcess;

PluginProcess::PluginProcess(const QString& pluginName) :
  d_ptr ( new PluginProcessPrivate(this) )
{
  Q_D ( PluginProcess );
  d->pluginName = pluginName;
}

Plugin::State
PluginProcess::state() const
{
  return Plugin::Loaded;
}

void
PluginProcess::start()
{
  Q_D( PluginProcess );
  d->buildProcess();
}

void
PluginProcess::stop()
{
}

QString
PluginProcess::pluginName() const
{
  Q_D ( const PluginProcess );
  return d->pluginName;
}

PluginProcess::~PluginProcess()
{
}
