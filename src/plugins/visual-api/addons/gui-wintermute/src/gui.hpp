/**
 * wntrgui.hpp
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

#ifndef GUI_HPP
#define GUI_HPP

#include <QObject>
#include <QThread>
#include <plugins.hpp>
#include "mainwindow.hpp"
#include "backend.hpp"

namespace Wintermute
{
namespace Visual
{
namespace Graphics
{
struct Core;

/**
 * @brief
 * @class Core gui.hpp "gui/gui.hpp"
 */

class Core : public QObject
{
    friend class Backend;
    Q_OBJECT
public:
    /**
     * @brief Starts the graphics core.
     * @fn Initialize
     */
    static void start (AbstractPlugin*);
    /**
     * @brief Stops the graphics core.
     * @fn Deinitialize
     */
    static void stop();

    static Backend* backend();

private:
    static MainWindow* s_wndw;
    static Backend* s_bcknd;
};
}
}
}

#endif /** GUI_HPP */

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
