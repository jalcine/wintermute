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

#include <QUuid>
#include <QSettings>
#include <QPluginLoader>
#include <Wintermute/PluginInterfaceable>

namespace Wintermute {
  class PluginPrivate {
    public:
      QUuid id;
      QSettings* settings;
      QPluginLoader* loader;

      PluginPrivate(QUuid const id) : id(id), settings(0), loader(0) { }

      ~PluginPrivate() { }

      bool loadBinary() {
        return false;
      }

      bool unloadBinary() { 
        return false;
      }

      Plugin* tryLoad(QPluginLoader* plugin){
        return 0;
      }

      Plugin* getPluginInstance(){
        return 0;
      }
  };
}

