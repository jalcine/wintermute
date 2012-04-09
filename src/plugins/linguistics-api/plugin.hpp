/**
 * @file wntrling.hpp
 * This file is part of Wintermute Linguistics
 *
 * Copyright (C) 2011 - Wintermute Development <wntr-devel@thesii.org>
 *
 * Wintermute Linguistics is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute Linguistics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute Linguistics; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef WNTRLING_PLUGIN_HPP
#define WNTRLING_PLUGIN_HPP

// Qt includes
#include <QObject>

// Wintermute includes
#include <abstractplugin.hpp>

namespace Wintermute
{
namespace Linguistics
{

class Plugin : public Plugins::AbstractPlugin
{
    Q_OBJECT
    Q_DISABLE_COPY (Plugin)

public:
    Plugin() : AbstractPlugin() { }
    virtual ~Plugin();

    virtual void start() const;
    virtual void stop() const;
    virtual QObject* instance() const;
};

}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
