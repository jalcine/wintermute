/**
 * plugins.cpp
 * This file is part of nCurses for Wintermute.
 *
 * Copyright (C) 2011 - Wintermute Development <wntr-devel@thesii.org>
 *
 * nCurses for Wintermute is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * nCurses for Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with nCurses for Wintermute; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#include "plugin.hpp"
#include "ncurses.hpp"
#include "core.hpp"
#include <QObject>
#include <QtPlugin>
#include <QStringList>

namespace Wintermute
{
namespace Visual
{
namespace nCurses
{
Plugin::Plugin()
{
    connect (Wintermute::Core::instance(), SIGNAL (started()),
             Wintermute::Curses::Curses::instance(), SLOT (start()));
    connect (Wintermute::Core::instance(), SIGNAL (started()),
             Wintermute::Curses::Curses::instance(), SLOT (stop()));
}

Plugin::~Plugin() { }

void Plugin::start () const
{
}

void Plugin::stop () const
{
}
}
}
}

Q_EXPORT_PLUGIN2 (nCurses - wintermute, Wintermute::Visual::nCurses::Plugin)
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
