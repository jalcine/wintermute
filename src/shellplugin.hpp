/***
 *  This file is part of Wintermute.
 *
 *  Copyright (C) 2012 Jacky Alciné <me@jalcine.me>
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

#ifndef WINTERMUTE_SHELLPLUGIN_HPP
#define WINTERMUTE_SHELLPLUGIN_HPP

#include <global.hpp>
#include <plugin.hpp>

WINTER_FORWARD_DECLARE_CLASS(Factory)

WINTER_BEGIN_NAMESPACE

/**
 * @brief A helper class for plug-ins.
 * Shell plug-ins allows the Factory to learn about a plug-in configuration
 * and setup without loading all of its libraries and dependencies. It's also
 * provided since AbstractPlugin is an abstract class.
 */
class ShellPlugin : public AbstractPlugin
{
    Q_OBJECT
    friend class Factory;
    friend class AbstractPlugin;

public:
    ShellPlugin();
    ShellPlugin (const QString& p_uuid);
    virtual ~ShellPlugin();

private:
    virtual inline void start () const { }
    virtual inline void stop () const { }
};

WINTER_END_NAMESPACE

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
