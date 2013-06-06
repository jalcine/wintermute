/**
 *
 * Copyright (C) 2013 Jacky Alcine <jacky.alcine@thesii.org>
 *
 * This file is part of Wintermute, the extensible AI platform.
 *
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

#include "plugininterfaceable.hpp"
#include "temporaryplugin.hpp"
#include "private/plugin.hpp"

using Wintermute::Plugin;
using Wintermute::PluginPrivate;
using Wintermute::TemporaryPlugin;

TemporaryPlugin::TemporaryPlugin(const QUuid& id, QPluginLoader* theLoader) :
  Plugin(id) {
  Q_D(Plugin);

  d->loader = theLoader;
}

PluginInterfaceable*
TemporaryPlugin::tryLoad(QPluginLoader* loader){
  Q_D(Plugin);
  Plugin* loadedPlugin = 0;

  if (d->loadBinary()){
    loadedPlugin = d->getPluginInterface();
    // TODO: Change the internal QPluginLoader;
  }

  return loadedPlugin;
}

TemporaryPlugin::~TemporaryPlugin() {
}

#include "temporaryplugin.moc"
