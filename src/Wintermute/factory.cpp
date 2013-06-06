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
#include "temporaryplugin.hpp"
#include <QDir>
#include <QFile>

using namespace Wintermute;
using Wintermute::Factory;
using Wintermute::FactoryPrivate;

namespace Wintermute {
  class FactoryPrivate {
    public:
      PluginMap active;

      explicit FactoryPrivate() { }
      
      ~FactoryPrivate() { }
      
      /**
       * @fn availablePlugins
       *
       * Obtains a list of the plug-ins that Wintermute can find on this local 
       * system.
       *
       * TODO: Incorporate a means of collecting a remote list of plug-ins?
       */
      PluginList availablePlugins() const {
        // Grab a list of plug-ins in the definition folder.
        QDir pluginDefDir(WINTERMUTE_PLUGIN_DEFINITION_DIR);
        QStringList files = pluginDefDir.entryList(QStringList() << "*.spec", QDir::Files);
        PluginList plugins;

        Q_FOREACH(QString pluginFile, files){
          QString uuid = pluginFile.remove(".spec");
          wdebug(Wintermute::Application::instance(), QString("Found plugin file '%1'.").arg(uuid));
          plugins << new TemporaryPlugin(uuid, 0);
        }

        return plugins;
      }
      
      PluginList activePlugins() const {
        return active.values();
      }

      Plugin* plugin(const QUuid& id) const {
        if (active.contains(id))
          return active[id];

        return 0;
      }

      QSettings* obtainConfiguration(const QUuid& id) const {
        const QString settingsPath = QString(WINTERMUTE_PLUGIN_DEFINITION_DIR "/" + id.toString() + ".spec");
        if (QFile::exists(settingsPath)){
          QSettings* settings = new QSettings(settingsPath);
          return settings;
        }

        return 0;
      }
      
      QPluginLoader* obtainBinary(const QUuid& id) const {
        const QSettings* config = this->obtainConfiguration(id);

        if (!config)
          return 0;

        const QString libraryName = config->value("Plugin/LibraryName").toString();
        QPluginLoader* loader = new QPluginLoader(libraryName);

        if (loader->fileName().isEmpty())
          return 0;

        return loader;
      }
  };
}
Factory* Factory::self = 0;

Factory::Factory() : QObject(Application::instance()), d_ptr(new FactoryPrivate) {
  wdebug(this,"Factory created.");
}

Factory*
Factory::instance() {
  if (!self)
    self = new Factory;

  return self;
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

// TODO: Ensure plug-in loading process.
bool
Factory::loadPlugin(const QUuid& id){
  Q_D(Factory);
  PluginInterfaceable* obtainedPluginInterface = 0;
  Logger* log = wlog(this);
  QPluginLoader* loader = d->obtainBinary(id);
  TemporaryPlugin* plugin = new TemporaryPlugin(id, loader);
  
  if (!loader){
    log->debug(QString("Couldn't find binary for plugin '%1'.").arg(id.toString()));
    return false;
  } else {
    log->debug(QString("Found binary for plugin '%1'.").arg(id.toString()));
  }

  log->info(QString("Attempted to load plug-in instance for %1...").arg(id.toString()));
  obtainedPluginInterface = plugin->tryLoad(loader);

  if (obtainedPluginInterface){
    log->info(QString("Plug-in instance for %1 obtained.").arg(id.toString()));
    return true;
  } else {
    log->error(QString("Plug-in instance for %1 failed to load.").arg(id.toString()));
  }

  return false;
}

// TODO: Verify the unloading process.
bool
Factory::unloadPlugin(const QUuid& id){
  Q_D(Factory);

  Plugin* plugin = d->plugin(id);
  if (!plugin->isLoaded()){
    return true;
  }

  return plugin->unload();
}

// TODO: Auto load plugins on start.
bool
Factory::autoloadPlugins(){
  Logger* log = wlog(this);
  QVariantList autoloadList = Wintermute::Application::setting("Plugins/Autoload", QVariantList()).toList();
  PluginList all = this->availablePlugins();

  log->info(QString("Loading %1 of %2 plugins...").arg(autoloadList.length()).arg(all.length()));

  Q_FOREACH(Plugin* plugin, all){
    bool pluginLoaded = this->loadPlugin(plugin->id().toString());
    if (!pluginLoaded){
      log->info(QString("Autoload of %1 failed; thus cancelling the auto-loading process.").arg(plugin->id().toString()));
      //return false;
    }
  }

  return true;
}

bool
Factory::unloadAllPlugins(){
  return false;
}

void
Factory::pluginStateChange(const QUuid& id, const Plugin::State& state)
{
  Logger* log = wlog(this);
  log->info("Passing signal.");
  emit this->pluginStateChanged(id, state);
}

void
Factory::start() {
  Logger* log = wlog(this);
  log->info("Starting..");
  this->autoloadPlugins();
  log->info("Started.");
}

void
Factory::stop() {
  Logger* log = wlog(this);
  log->info("Stopping..");
  log->info("Stopped.");
}

Factory::~Factory(){
}

#include "factory.moc"
