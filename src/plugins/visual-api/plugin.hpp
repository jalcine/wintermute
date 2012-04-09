/**
 * @file plugin.hpp
 * Copyright (C) 2011 - Wintermute Development <wntr-devel@thesii.org>
 *
 * Wintermute Visual is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute Visual is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute Visual; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef _WNTRVISUAL_PLUGIN_HPP_
#define _WNTRVISUAL_PLUGIN_HPP_

#include <QtPlugin>
#include <QString>
#include <plugins.hpp>
#include "config.hpp"

using Wintermute::Plugins::AbstractPlugin;

/// See plugin.cpp for more information.
namespace Wintermute
{
namespace Visual
{
struct Plugin;

/**
* @brief
* @class Plugin plugin.hpp "src/plugin.hpp"
*/
class Plugin : public Wintermute::Plugins::AbstractPlugin
{
    Q_OBJECT
    Q_DISABLE_COPY (Plugin)

public:
    Plugin();
    virtual ~Plugin();
    virtual void start() const;
    virtual void stop() const;
};
}
}

#endif /** _PLUGIN_HPP */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
