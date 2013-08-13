/**
 *
 * Copyright (C) 2013 Jacky Alciné <jacky.alcine@thesii.org>
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
#include <Wintermute/Globals>

namespace Wintermute {
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
       * @fn start
       * @brief
       *
       * Starts the initial processing required for execution in Wintermute
       * and begins the event loop.
       */
      Q_SLOT void start();

      /**
       * @fn stop
       * @brief
       *
       * Stops the started processes in `start()` and begins the clean up for
       * the event loop.
       */
      Q_SLOT void stop();
  };

}

#endif /* WINTERMUTE_CORE_APPLICATION_HPP */
