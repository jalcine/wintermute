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

#ifndef WINTERMUTE_CORE_INTERNAL_TEMPORARYPLUGIN_HPP
#define WINTERMUTE_CORE_INTERNAL_TEMPORARYPLUGIN_HPP

#include "plugin.hpp"
#include <QPluginLoader>

namespace Wintermute {
  class PluginInterfaceable;
  class TemporaryPlugin : public Plugin {
    Q_OBJECT;

    public:
      explicit TemporaryPlugin(const QUuid& id, QPluginLoader* theLoader);
      virtual ~TemporaryPlugin();
      PluginInterfaceable* tryLoad(QPluginLoader* loader);
  };
}

#endif /* WINTERMUTE_CORE_INTERNAL_TEMPORARYPLUGIN_HPP */
