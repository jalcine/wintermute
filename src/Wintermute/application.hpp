/**
 *
 * Copyright (C) 2013 Jacky Alcine <me@jalcine.me>
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

#ifndef WINTERMUTE_CORE_APPLICATION_HPP
#define WINTERMUTE_CORE_APPLICATION_HPP

#include <QtCore/QVariant>
#include <QtCore/QList>
#include <Wintermute/Globals>

namespace Wintermute {
  namespace Procedure { class Module; }
  class Version;
  class ApplicationPrivate;

  /**
   * @class Application
   *
   * The `Application` class in Wintermute serves as an object hierarchy
   * anchor. It's the first object to be created and fights to the death to be
   * the last one destroyed.
   */
  class Application : public QObject {
    Q_OBJECT;
    Q_DECLARE_PRIVATE(Application);
    Q_DISABLE_COPY(Application);

    QScopedPointer<ApplicationPrivate> d_ptr;
    static Application* self;

    /**
     * @fn Application
     * @brief Private initialization of application.
     * @constructor
     *
     * @param argc Argument count (native-value)
     * @param argv Arguemnt array (native-value)
     */
    Application(int &argc, char **argv);

    public:
    virtual ~Application();
    /**
     * @fn instance
     * @brief Provides a pointer to an instance of Wintermute::Application.
     *
     * In order to keep Wintermute's instance running smoothly,
     * a shared pointer to the instance of the application that's running
     * and handling Wintermute's lower-level work. It's strongly recommended
     * that you use this method if desired.
     *
     * @return A shared pointer to the working instance of Wintermute::Application.
     */
    static inline Application* instance() { return self; }

    /**
     * @fn run
     * @brief Starts Wintermute.
     *
     * This once-invokable method begins the low-level initialization of
     * Wintermute. It's invoked from Wintermute's `main()` method and cannot
     * be run a second time.
     */
    static int run( int& argc, char **argv );

    /**
     * @fn setting
     * @brief Obtains a setting from Wintermute's local configuration.
     *
     * @note These options are global to the Wintermute application.
     */
    static QVariant setting(const QString& path, const QVariant defaultValue = QVariant());

    /** @fn setSetting
     * @brief Changes a value of the setting for Wintermute
     *
     * @note These options are global to the Wintermute application.
     */
    static void setSetting(const QString& path, const QVariant value);

    /**
     * @fn version
     * @brief Obtains the current version of Wintermute.
     */
    Version version() const;

    /**
     * @fn processName
     * @brief Obtains the stringifed process name used by Wintermute.
     *
     * Obtains the process name of this running instance of Wintermute. This
     * is typically in the form of `domain.application:$PID`, a bit similar
     * to package paths in Java. The :$PID portion is added to help increase
     * the precision of finding specific processes. Process names are formed
     * from the formulated name of the 'root module'.
     *
     * @see setModule()
     * @see module()
     */
    QString processName() const;

    /**
     * @fn module()
     * @brief Defines the root module of the process.
     */
    Procedure::Module* module() const;

    /**
     * @fn findModule
     * @brief Finds a method by its specified module.
     */
    Procedure::Module* findModule(const QString& name) const;

    /**
     * @fn modules
     * @brief Obtains a list of the modules registered locally.
     */
    QList<Procedure::Module*> modules() const;

    /**
     * @fn start
     * @brief
     *
     * Starts the initial processing required for execution in Wintermute
     * and begins the event loop.
     */
    void start();

    /**
     * @fn stop
     * @brief
     *
     * Stops the started processes in `start()` and begins the clean up for
     * the event loop.
     */
    void stop();
  };
}

#define wApp Wintermute::Application::instance()

#endif /* WINTERMUTE_CORE_APPLICATION_HPP */
