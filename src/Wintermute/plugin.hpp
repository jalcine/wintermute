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

#ifndef WINTERMUTE_CORE_PLUGIN_HPP
#define WINTERMUTE_CORE_PLUGIN_HPP

#include <Wintermute/Globals>
#include <QtCore/QUuid>
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QList>
#include <QtCore/QMap>

namespace Wintermute {
  class Version;
  class PluginPrivate;

  class Plugin : public QObject {
    // {{{ QObject-ified.
    Q_OBJECT;
    Q_ENUMS(State);
    Q_PROPERTY(QString Name          READ name);
    Q_PROPERTY(Version Version       READ version);
    Q_PROPERTY(Version SystemVersion READ systemVersion);
    Q_PROPERTY(State   State         READ state);
    // }}} QObject-ified.

    // {{{ Private data.
    Q_DECLARE_PRIVATE(Plugin);
    QScopedPointer<PluginPrivate> d_ptr;
    // }}} Private data.

    protected:
    explicit Plugin(const QString& uuid);
    bool loadBinary();
    bool unloadBinary();
    QSettings* configuration() const;

    public:

    enum State {
      Undefined = 0,
      Loading,
      Loaded,
      Unloading,
      Unloaded,
      Crashed
    };

    virtual ~Plugin();
    QUuid id() const;
    QString name() const;
    Version version() const;
    Version systemVersion() const;
    State state() const;

    Q_SIGNAL void loaded();
    Q_SIGNAL void unloaded();
  };

  typedef QList<Plugin*> PluginList;
  typedef QMap<QUuid, Plugin*> PluginMap;
}

#endif /* WINTERMUTE_CORE_PLUGIN_HPP */
