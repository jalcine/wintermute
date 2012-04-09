/*
 * backend.cpp
 * This file is part of Wintermute Graphical User Interface
 *
 * Copyright (C) 2011 - Jacky Alcine
 *
 * Wintermute Graphical User Interface is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Wintermute Graphical User Interface is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "backend.hpp"
#include "config.hpp"
#include "gui.hpp"
#include <plugins.hpp>
#include <core.hpp>
#include <QDebug>

using Wintermute::Plugins::AbstractPlugin;
using namespace Wintermute::Visual;

namespace Wintermute
{
namespace Visual
{
namespace Graphics
{
Backend::Backend (AbstractPlugin* p_plgn) : AbstractBackend (p_plgn)
{
    connect (Framework::instance(), SIGNAL (alertDisplayed (AlertLevel, QString)), this, SLOT (alertDisplayed (AlertLevel, QString)));
}

void Backend::alertDisplayed (const AlertLevel& alertLevel, const QString& message)
{
}

void Backend::initialize()
{
    qDebug() << "(gui) [Backend] Initializing...";
    Core::s_wndw->show();
    qDebug() << "(gui) [Backend] Initializied.";
}

void Backend::deinitialize()
{
    qDebug() << "(gui) [Backend] Deinitializing...";
    Core::s_wndw->close();
    qDebug() << "(gui) [Backend] Deinitializied.";
}

const QString Backend::id() const
{
    return WNTRGUI_UUID;
}

const bool Backend::isActive() const
{
    return Core::s_wndw != 0;
}

Backend::~Backend() { }
}
}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
