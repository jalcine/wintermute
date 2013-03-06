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

#ifndef WINTERMUTE_APPLICATION_HPP
#define WINTERMUTE_APPLICATION_HPP

#include <QObject>
#include <QSharedPointer>

namespace Wintermute {
  class ApplicationPrivate;
  class Application : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(Application);  

    QScopedPointer<ApplicationPrivate> d_ptr;
    static Application* self;
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
      static Application* instance();

      /**
       * @fn run
       * @brief Starts Wintermute.
       *
       * This once-invokable method begins the low-level initialization of 
       * Wintermute. It's invoked from Wintermute's `main()` method and cannot 
       * be run a second time.
       */
      static int run( int& argc, char **argv );
  };

}

#endif /* WINTERMUTE_APPLICATION_HPP */
