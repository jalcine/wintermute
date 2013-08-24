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
  PluginList availablePlugins() const {
    // Grab a list of plug-ins in the definition folder.
    QDir pluginDefDir ( WINTERMUTE_PLUGIN_DEFINITION_DIR );
    QStringList files = pluginDefDir.entryList ( QStringList() << "*.spec", QDir::Files );
    PluginList plugins;
    wdebug ( Factory::instance(), QString ( "Plugins: %1" ).arg ( files.join ( "," ) ) );
    wdebug ( Factory::instance(), WINTERMUTE_PLUGIN_DEFINITION_DIR );
    Q_FOREACH ( QString pluginFile, files ) {
      QString uuid = pluginFile.remove ( ".spec" );
      wdebug ( Wintermute::Application::instance(), QString ( "Found plugin file '%1'." ).arg ( uuid ) );
      plugins << new TemporaryPlugin ( uuid, 0 );
    }
    return plugins;
  }

  PluginList activePlugins() const {
    return active.values();
  }

  Plugin* plugin ( const QUuid& id ) const {
    if ( active.contains ( id ) )
      { return active[id]; }
    return 0;
  }

  QSettings* obtainConfiguration ( const QUuid& id ) const {
    const QString settingsPath = QString ( WINTERMUTE_PLUGIN_DEFINITION_DIR "/" + id.toString() + ".spec" ).replace ( "}", "" ).replace ( "{", "" );
    if ( QFile::exists ( settingsPath ) ) {
      QSettings* settings = new QSettings ( settingsPath, QSettings::IniFormat, Factory::instance() );
      winfo ( Factory::instance(), QString ( "Found configuraton for plug-in '%1' at '%2'." ).arg ( id.toString() ).arg ( settingsPath ) );
      return settings;
    }
    wdebug ( Factory::instance(), QString ( "Can't find configuration data for %1 at %2." ).arg ( id.toString() ).arg ( settingsPath ) );
    return 0;
  }

  QPluginLoader* obtainBinary ( const QUuid& id ) const {
    const QSettings* config = this->obtainConfiguration ( id );
    if ( !config ) {
      wdebug ( Factory::instance(), QString ( "Can't find meta-configuration for plugin %1." ).arg ( id.toString() ) );
      return 0;
    }
    const QString libraryName = config->value ( "Plugin/Library" ).toString();
    QPluginLoader* loader = new QPluginLoader ( QString ( WINTERMUTE_PLUGIN_LIBRARY_DIR "/lib%1.so" ).arg ( libraryName ) );
    if ( loader->fileName().isEmpty() ) {
      wdebug ( Factory::instance(), QString ( "Library not found on operating system: '%1'" ).arg ( libraryName ) );
      Q_FOREACH ( QString libraryPath, QCoreApplication::libraryPaths() ) {
        wdebug ( Factory::instance(), QString ( "Available library path: %1" ).arg ( libraryPath ) );
      }
      return 0;
    }
    return loader;
  }
};
}
