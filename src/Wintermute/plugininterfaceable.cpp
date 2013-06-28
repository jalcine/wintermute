/**
 *
 * Copyright (C) 2013 Jacky Alcine <jacky.alcine@thesii.org>
 *
 * This file is part of Wintermute, the extensible AI platform.
 *
 *
 * Wintermute is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
**/

#include "plugininterfaceable.hpp"
#include "private/plugin.hpp"
#include "private/plugininterfaceable.hpp"
#include "factory.hpp"
#include "plugin.hpp"

using Wintermute::PluginInterfaceablePrivate;
using Wintermute::PluginInterfaceable;
using Wintermute::Factory;

PluginInterfaceable::PluginInterfaceable() : QObject(Factory::instance()), d_ptr(this) {
}

PluginInterfaceable::~PluginInterfaceable() {
}

#include "Wintermute/plugininterfaceable.moc"
