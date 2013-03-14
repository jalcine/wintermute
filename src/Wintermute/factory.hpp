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

#ifndef WINTERMUTE_CORE_FACTORY_HPP
#define WINTERMUTE_CORE_FACTORY_HPP

#include <Wintermute/Globals>
#include <Wintermute/Plugin>
#include <QtCore/QStringList>
#include <QtCore/QUuid>

namespace Wintermute {
  class FactoryPrivate;
  class Factory : public QObject {
    friend class Application;
    friend class ApplicationPrivate;

    Q_OBJECT;
    Q_DECLARE_PRIVATE(Factory);
    FactoryPrivate* d_ptr;
    static Factory* self;

    explicit Factory();

    private:
    void start();
    void stop();
    Q_SLOT void pluginLoaded(const QUuid& id);
    Q_SLOT void pluginUnloaded(const QUuid& id);
    Q_SLOT void pluginStateChange(const QUuid& id, const Plugin::State& state);

    public:
    virtual ~Factory();
    static inline Factory* instance() { return self; }
    PluginList availablePlugins() const;
    PluginList activePlugins() const;
    bool loadPlugin(const QUuid& id);
    bool unloadPlugin(const QUuid& id);
    bool autoloadPlugins();
    bool unloadAllPlugins();

    Q_SIGNAL void pluginStateChanged(const QUuid& id, const Plugin::State& state);
  };
}

#endif /* WINTERMUTE_CORE_FACTORY_HPP */
