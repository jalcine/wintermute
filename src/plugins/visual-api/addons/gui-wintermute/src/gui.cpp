/**
 * wntrgui.cpp
 * This file is part of Wintermute Graphics.
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

#include <wntr/core.hpp>
#include <wntr/config.hpp>
#include <QMessageBox>
#include <QtDebug>
#include "gui.hpp"

namespace Wintermute
{
namespace Visual
{
namespace Graphics
{
MainWindow* Core::s_wndw = 0;
Backend* Core::s_bcknd = 0;

void Core::start (AbstractPlugin* p_plgn)
{
    qDebug() << "(gui) [Core] Initialized.";
    s_wndw = new MainWindow;
    s_bcknd = new Backend (p_plgn);
}

/// @todo Should this kill the entire system?
void Core::stop ()
{
    qDebug() << "(gui) [Core] Deinitialized.";
    s_wndw->deleteLater ();
    Wintermute::Core::exit (0, false);
}

Backend* Core::backend()
{
    return s_bcknd;
}
}
}
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
