/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2014 Jacky Alciné <me@jalcine.me>
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

#ifndef WINTERMUTE_PLUGIN_PROCESS_HPP
#define WINTERMUTE_PLUGIN_PROCESS_HPP

#include <QtCore/QObject>
#include <Wintermute/Plugin>

namespace Wintermute
{
  class PluginProcessPrivate;
  class PluginProcess : public QObject {
    Q_OBJECT;
    QScopedPointer<PluginProcessPrivate> d_ptr;
    Q_DECLARE_PRIVATE ( PluginProcess );

    public:
      explicit PluginProcess(const QString& pluginName);
      virtual ~PluginProcess();
      void start();
      void stop();
      QString pluginName() const;
  };
}

#endif
