/***
 *  This file is part of the Wintermute project.
 *
 *  Copyright (C) 2012 Jacky Alciné <jacky.alcine@thesii.org>
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
 *  You should have received a copy of the GNU Library General Public
 *  License along with Wintermute.
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jacky.alcine@thesii.org>
 * @date 04/18/12 4:18:48 PM
 *
 * @mainpage Wintermute API Documentation
 * @section intro Introduction
 * Wintermute is written mainly in the C++ programming language using the Qt library.
 * It features a plug-in system (see Wintermute::Plugins::AbstractPlugin),
 * inter-process communication (see Wintermute::IPC::System) and a centralized system that switches
 * between everything else. This API provides a peek into the core of Wintermute and presents
 * how these three components work together in order to provide the unique and rare user
 * experience that people experience when using Wintermute. You can find more information
 * about the current development progress of Wintermute or want to join the development team,
 * visit <a href="http://www.thesii.org/wiki/Wintermute">Wintermute</a> on our Wiki and click
 * <em>Join the Team</em>. We hope to hear from you soon.
 *
 * - If you're interested in building a plug-in for Wintermute, visit Wintermute::Plugins::AbstractPlugin.
 * - Visit the <a href="http://www.thesii.org/wiki">Synthetic Intellect Institute</a>'s Wiki page for Wintermute!
 */

#ifndef WINTERMUTE_HPP
#define WINTERMUTE_HPP

#include "global.hpp"
#include "core.hpp"
#include "ipc.hpp"
#include "backend.hpp"
#include "adaptors.hpp"
#include "factory.hpp"
#include "plugin.hpp"
#include "pluginhandle.hpp"
#include "logging.hpp"

#endif /* WINTERMUTE_HPP_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
