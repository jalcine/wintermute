/***
 *  This file is part of Wintermute.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
 *
 *  Wintermute is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  Wintermute is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with Wintermute .  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef WINTERMUTE_PLUGINPRIVATE_HPP
#define WINTERMUTE_PLUGINPRIVATE_HPP

#include <global.hpp>

class QPluginLoader;
class QSettings;

WINTER_FORWARD_DECLARE_CLASS(Factory);
WINTER_FORWARD_DECLARE_CLASS(ShellPlugin);
WINTER_FORWARD_DECLARE_CLASS(AbstractPlugin);

WINTER_BEGIN_NAMESPACE

struct AbstractPluginPrivate {
    Q_DECLARE_PUBLIC (AbstractPlugin)
    friend class Factory;
    friend class ShellPlugin;

private:
    mutable QPluginLoader* pluginLoader;
    AbstractPlugin* q_ptr;
    bool loadPlugins() const;
    bool loadPackages() const;
    void loadSettings (const QString&);
    void doStart();
    void doStop();

public:
    AbstractPluginPrivate (AbstractPlugin* p_qPtr);
    virtual ~AbstractPluginPrivate();
    QSettings* settings;
    QSettings* configuration;

};

WINTER_END_NAMESPACE

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
