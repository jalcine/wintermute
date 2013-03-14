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

#include "factory.hpp"
#include "plugin.hpp"
#include "logging.hpp"
#include "application.hpp"

using namespace Wintermute;
using Wintermute::FactoryPrivate;

Factory* Factory::self = 0;

class FactoryPrivate {
  PluginMap active;
  public:
    FactoryPrivate()  {
    }

    PluginList
    availablePlugins() const {
      return PluginList();
    }

    PluginList
    activePlugins() const {
      return active.values();
    }

    Plugin*
    plugin(const QUuid& id) const {
      if (active.contains(id))
        return active[id];
      
      return 0;
    }
};

Factory::Factory() : QObject(Application::instance()), d_ptr(new FactoryPrivate) {

}

PluginList
Factory::availablePlugins() const {
  Q_D(const Factory);
  return d->availablePlugins();
}

PluginList
Factory::activePlugins() const {
  Q_D(const Factory);
  return d->activePlugins();
}

bool
Factory::loadPlugin(const QUuid& id){
  return false;
}

bool
Factory::unloadPlugin(const QUuid& id){
  return false;
}

bool
Factory::autoloadPlugins(){
  return false;
}

bool
Factory::unloadAllPlugins(){
  return false;
}

void
Factory::pluginLoaded(const QUuid& id){
}

void
Factory::pluginUnloaded(const QUuid& id){
}

void
Factory::pluginStateChange(const QUuid& id, const Plugin::State& state)
{

}

void
Factory::start() {
  Logger* log = Logging::obtainLogger(this);
  log->info("Starting..");
  log->info("Started.");
}

void
Factory::stop() {
}

Factory::~Factory(){
}

#include "factory.moc"
