/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2011 - 2013 Jacky Alciné <me@jalcine.me>
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

#ifndef WINTERMUTE_FACTORY_PRIVATE_HPP
#define WINTERMUTE_FACTORY_PRIVATE_HPP

#include "plugin.hpp"
#include "factory.hpp"
#include "application.hpp"
#include <Wintermute/Globals>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>

namespace Wintermute
{
class FactoryPrivate
{
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
    QStringList
    availablePlugins() const {
      // Grab a list of plug-ins in the designation folder.
      QDir pluginDefDir ( WINTERMUTE_PLUGIN_DEFINITION_DIR );
      QStringList files = pluginDefDir.entryList ( QStringList() << "*.spec",
                          QDir::Files );
      QStringList plugins;
      wdebug ( wntrFactory.data(), QString ( "Scanning %1" ).
               arg(WINTERMUTE_PLUGIN_DEFINITION_DIR));

      for ( QString pluginFile: files ) {
        QString name = pluginFile.remove ( ".spec" );
        wdebug ( wntrFactory.data(),
                 QString ( "Found plugin file '%1'." ).arg ( name ) );
        plugins << name;
      }

      return plugins;
    }

    QStringList
    activePlugins() const {
      return active.keys();
    }

    Plugin::Ptr
    plugin ( const QString& name ) const {
      if ( active.contains ( name ) ) {
        return active[name];
      }

      return Plugin::Ptr(nullptr);
    }

    QSettings*
    obtainConfiguration ( const QString& name ) const {
      const QString settingsPath = QString (
                                     WINTERMUTE_PLUGIN_DEFINITION_DIR "/" + name + ".spec" );

      if ( QFile::exists ( settingsPath ) ) {
        QSettings* settings = new QSettings ( settingsPath,
                                              QSettings::IniFormat, wntrFactory.data() );
        winfo ( wntrFactory.data(), QString (
                  "Found configuraton for plug-in '%1' at '%2'." )
                .arg ( name ).arg ( settingsPath ) );
        return settings;
      }

      wdebug ( wntrFactory.data(),
               QString ( "Can't find configuration data for %1 at %2." )
               .arg ( name ).arg ( settingsPath ) );
      return 0;
    }

    QPluginLoader*
    obtainBinary ( const QString& name ) const {
      QString path;
      QPluginLoader* loader = nullptr;
      const QString libraryName = "lib" + name + ".so";

      if ( QLibrary::isLibrary ( libraryName ) ) {
        wwarn ( wntrFactory.data(),
                QString ( "Library not found on operating system: '%1'" )
                .arg ( libraryName ) );
        Q_FOREACH ( QString libraryPath, QCoreApplication::libraryPaths() ) {
          path = libraryPath + "/" + libraryName;
          wdebug ( wntrFactory.data(),
                   QString ( "Trying library path '%1'..." ).arg ( path ) );

          if ( QLibrary::isLibrary ( path ) ) {
            break;
          } else {
            path = QString::null;
          }
        }

        if ( path == QString::null ) {
          loader = nullptr;
        } else {
          loader = new QPluginLoader ( path );
        }
      }

      if ( loader && loader->fileName().isEmpty() ) {
        werr ( wntrFactory.data(), QString ( "Library '%1' not found." )
               .arg ( libraryName ) );
        return nullptr;
      }

      return loader;
    }
};
}

#endif
