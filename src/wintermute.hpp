/**
 * @file wintermute.hpp
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
 *
 * @legalese
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * @endlegalese
 */

/**
 * @mainpage Wintermute API Documentation
 * <img src="wintermute-profile.png" align="right" title="Wintermute logo"/>
 * @section intro Introduction
 * Wintermute is written mainly in C++. It features a plug-in system (see Wintermute::Plugins::PluginBase),
 * inter-process communication (see Wintermute::IPC) and a centralized system that switches
 * between everything else. This API provides a peek into the core of Wintermute and presents
 * how these three components work together in order to provide the unique and rare user
 * experience that people experience when using Wintermute. You can find more information
 * about the current development progress of Wintermute or want to join the development team,
 * visit <a href="http://www.thesii.org/wiki/Wintermute">Wintermute</a> on our Wiki and click
 * <b>Join the Team</b>. We hope to hear from you soon.
 *
 * - If you're interested in building a plug-in for Wintermute, visit Wintermute::Plugins::PluginBase.
 * - Visit the <a href="http://www.thesii.org">Synthetic Intellect Institute</a>'s Wiki page for Wintermute!
 */
#ifndef WINTERMUTE_HPP
#define WINTERMUTE_HPP

// Joint header
#include "config.hpp"
#include "core.hpp"
#include "ipc.hpp"
#include "plugins/plugins.hpp"

#endif /* WINTERMUTE_HPP */
// kate: indent-mode cstyle; space-indent on; indent-width 4;
