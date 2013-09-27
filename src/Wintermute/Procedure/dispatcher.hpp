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

#ifndef WINTERMUTE_PROCEDURE_DISPATCHER_HPP
#define WINTERMUTE_PROCEDURE_DISPATCHER_HPP

#include <QtCore/QObject>
#include <Wintermute/Procedure/Module>

namespace Wintermute {
  namespace Procedure {
    class Module;
    class DispatcherPrivate;
    class Dispatcher : public QObject {
      Q_OBJECT;
      Q_DECLARE_PRIVATE(Dispatcher);
      friend class ModulePrivate;

      public:
        Q_SIGNAL void messageDispatched(Module* dispatchingModule, QString& data);

      protected:
        Dispatcher();
        virtual ~Dispatcher();
        virtual void sendMessage(const QString& data) = 0;
        static void dispatch(const QString& data);
    };
  }
}

#endif
