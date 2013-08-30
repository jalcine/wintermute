#include "plugin.hpp"
#include <Wintermute/Globals>
#include <QCoreApplication>
#include <QDir>
#include <QFile>

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
  QStringList availablePlugins() const {
    // Grab a list of plug-ins in the definition folder.
    QDir pluginDefDir ( WINTERMUTE_PLUGIN_DEFINITION_DIR );
    QStringList files = pluginDefDir.entryList ( QStringList() << "*.spec", QDir::Files );
    QStringList plugins;
    wdebug ( Factory::instance(), QString ( "Plugins: %1" ).arg ( files.join ( "," ) ) );
    wdebug ( Factory::instance(), WINTERMUTE_PLUGIN_DEFINITION_DIR );
    Q_FOREACH ( QString pluginFile, files ) {
      QString name = pluginFile.remove ( ".spec" );
      wdebug ( Wintermute::Application::instance(), QString ( "Found plugin file '%1'." ).arg ( name ) );
      plugins << name;
    }
    return plugins;
  }

  QStringList activePlugins() const {
    return active.keys();
  }

  Plugin* plugin ( const QString& name ) const {
    if ( active.contains ( name ) ) {
      return active[name];
    }
    return 0;
  }

  QSettings* obtainConfiguration ( const QString& name ) const {
    const QString settingsPath = QString ( WINTERMUTE_PLUGIN_DEFINITION_DIR "/" + name + ".spec" );
    if ( QFile::exists ( settingsPath ) ) {
      QSettings* settings = new QSettings ( settingsPath, QSettings::IniFormat, Factory::instance() );
      winfo ( Factory::instance(), QString ( "Found configuraton for plug-in '%1' at '%2'." ).arg ( name ).arg ( settingsPath ) );
      return settings;
    }
    wdebug ( Factory::instance(), QString ( "Can't find configuration data for %1 at %2." ).arg ( name ).arg ( settingsPath ) );
    return 0;
  }

  QPluginLoader* obtainBinary ( const QString& name ) const {
    QString path;
    const QString libraryName = "lib" + name + ".so";
    if ( QLibrary::isLibrary ( libraryName ) ) {
      wdebug ( Factory::instance(), QString ( "Library not found on operating system: '%1'" ).arg ( name ) );
      Q_FOREACH ( QString libraryPath, QCoreApplication::libraryPaths() ) {
        wdebug ( Factory::instance(), QString ( "Trying libpath: %1/%2" ).arg ( libraryPath, libraryName ) );
        path = libraryPath + "/" + libraryName;
        if ( QFile::exists ( path ) ) {
          break;
        } else {
          path = QString::null;
        }
      }
      if ( path == QString::null ) {
        return 0;
      }
    }
    return new QPluginLoader ( path );
  }
};
}
