/**
 *
 * Copyright (C) 2013 Jacky Alciné <me@jalcine.me>
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

#include <QtCore/QSettings>
#include <QtCore/QPluginLoader>
#include <Wintermute/Logging>

namespace Wintermute
{
class PluginPrivate
{
public:
  Q_DECLARE_PUBLIC ( Plugin );
  Plugin* q_ptr;
  QSettings* settings;
  QPluginLoader* loader;

  PluginPrivate ( Plugin* q ) : q_ptr ( q ), settings ( 0 ), loader ( 0 ) { }

  ~PluginPrivate() { }

  bool loadBinary() {
    loader->setLoadHints ( QLibrary::ResolveAllSymbolsHint | QLibrary::ExportExternalSymbolsHint );
    return loader->load();
  }

  bool unloadBinary() {
    return loader->unload();
  }

  bool tryLoad ( QPluginLoader* pluginLoader ) {
    Q_Q ( Plugin );
    this->loader = pluginLoader;
    if ( !this->loadBinary() ) {
      werr ( q_ptr, QString ( "Can't load binary!" ).arg ( pluginLoader->errorString() ) );
      this->loader = 0;
      return false;
    }
    winfo ( q_ptr, QString ( "Plugin interface loaded for %1" ).arg ( q->name() ) );
    return true;
  }
};
}
