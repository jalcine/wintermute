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

#ifndef WINTERMUTE_PLUGIN_HXX_
#define WINTERMUTE_PLUGIN_HXX_

#include <QtGlobal>

class QPluginLoader;
class QSettings;

namespace Wintermute
{
namespace Plugins
{

class Factory;
class ShellPlugin;

struct AbstractPluginPrivate {
    Q_DECLARE_PUBLIC (AbstractPlugin)
    friend class Factory;
    friend class ShellPlugin;

private:
    mutable QPluginLoader* m_plgnLdr;
    AbstractPlugin* q_ptr;
    bool loadPlugins() const;
    bool loadPackages() const;
    void loadSettings (const QString&);
    void doStart();
    void doStop();

public:
    AbstractPluginPrivate (AbstractPlugin* p_qPtr);
    virtual ~AbstractPluginPrivate();
    QSettings* m_sttngs;
    QSettings* m_cnfg;

};
}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
