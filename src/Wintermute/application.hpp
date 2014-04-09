/**
 * @author Jacky Alciné <me@jalcine.me>
 * @copyright © 2011, 2012, 2013, 2014 Jacky Alciné <me@jalcine.me>
 * @if 0
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
 * @endif
 **/

/**
 * @headerfile Wintermute/application.hpp <Wintermute/Application>
 * @brief      Definition of Wintermute::Application class.
 */

#ifndef WINTERMUTE_APPLICATION_HPP
#define WINTERMUTE_APPLICATION_HPP

#include <QtCore/QVariant>
#include <QtCore/QList>
#include <Wintermute/Globals>

/**
 * @def   wntrApp()
 * @brief Auxillary macro to reach the Application singleton.
 */
#define wntrApp Wintermute::Application::instance()

namespace Wintermute
{
namespace Procedure
{
class Module;
}
struct Version;
class ApplicationPrivate;

/**
 * @brief Accessibility class to control this instance of `Wintermute`.
 *
 * The `Application` class in `Wintermute` serves as an object hierarchy
 * anchor. It's the first object to be created and fights to the death to be
 * the last one destroyed. If you're confused about where to anchor any of
 * your `QObject`s, wntrApp is a good place as any, if not the best.
 *
 * @bug Wintermute can't capture POSIX signals as of yet.
 */
class Application : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE ( Application )
  Q_DISABLE_COPY ( Application )

  QScopedPointer<ApplicationPrivate> d_ptr;
  static QPointer<Application> self;

  /**
   * @brief Private initialization of application.
   * @param[in] argc Argument count (native-value)
   * @param[in] argv Argument array (native-value)
   */
  explicit Application ( int& argc, char** argv );

public:
  virtual ~Application();

  /**
   * @brief Provides a pointer to an instance of Wintermute::Application.
   * @return A shared pointer to the working instance of Wintermute::Application.
   * @callergraph
   * @callgraph
   *
   * In order to keep Wintermute's instance running smoothly,
   * a shared pointer to the instance of the application that's running
   * and handling Wintermute's lower-level work. It's strongly recommended
   * that you use this method, if desired, as a parent for your QObject.
   */
  static inline QPointer<Application> instance()
  {
    return self;
  }

  /**
   * @brief Starts Wintermute up from the command line.
   * @sa    Application::start()
   *
   * This once-invokable method begins the low-level initialization of
   * Wintermute. It's invoked from Wintermute's `main()` method and cannot
   * be run a second time.
   */
  static int run ( int& argc, char** argv );

  /**
   * @brief Obtains a setting from Wintermute's local configuration.
   * @note These options are global to the Wintermute application.
   * @sa   Plugin::configuration()
   * @param[in] path The path to the setting in question.
   * @param[in] defaultValue A value to return in the event none was provided.
   * @retval QVariant The value of this setting.
   *
   * @note It's good practice to pass in a default value so that your code's
   *       more predictable and error-resistant.
   */
  static QVariant setting ( const QString& path, const QVariant defaultValue 
      = QVariant() );

  /**
   * @brief Changes a value of the setting for Wintermute
   * @note These options are global to the Wintermute application.
   * @sa   Plugin::configuration()
   * @param[in] path The path to the setting in question.
   * @param[in] value A value to update this setting with.
   */
  static void setSetting ( const QString& path, const QVariant value );

  /**
   * @brief Obtains the current version of Wintermute.
   * @sa    Version
   *
   * Extremely direct and to the point, this returns the version of Wintermute
   * running in this process. Useful for handling upgrades or compability
   * issues with different instances of Wintermute.
   */
  Version version() const;

  /**
   * @brief Defines the root module of the process.
   * @sa    Module
   *
   * @bug This always gets set to the internal `ProcessModule` used to
   * extract this process over RPC. It doesn't allow plugins to replace the
   * root module with their preference. This might be a design flaw for now.
   */
  Procedure::Module* module() const;

  /**
   * @brief Starts Wintermute's event loop.
   * @sa    Application::stop(int)
   * @sa    Application::run(int&,char**)
   *
   * Starts the initial processing required for execution in Wintermute
   * and begins the event loop.
   */
  void start();

  /**
   * @brief Ends Wintermute's event loop.
   * @see   Application::start()
   *
   * Stops the started processes in `start()` and begins the clean up for
   * the event loop.
   */
  void stop ( int exitcode = 0x0 );

  /**
   * @brief Signal when start of Wintermute has been completed.
   *
   * This signal is emitted after Wintermute's event loop has started, so
   * event signaling may be a tad bit delayed.
   */
  Q_SIGNAL void started();

  /**
   * @brief Signal when stop of Wintermute has been completed.
   *
   * This signal is emitted after Wintermute has been completed; this is
   * typically before the end of the event loop, so event signaling may not be
   * risen at all.
   */
  Q_SIGNAL void stopped();
};
}

#endif /* WINTERMUTE_APPLICATION_HPP */
