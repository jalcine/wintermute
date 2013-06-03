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

#ifndef WINTERMUTE_CORE_PLUGIN_INTERFACEABLE_HPP
#define WINTERMUTE_CORE_PLUGIN_INTERFACEABLE_HPP

#include <Wintermute/Globals>
#include <QtCore/QObject>

namespace Wintermute {

  /**
   * @class Wintermute::PluginInterface
   *
   * This interface provides libraries that wish to be manipulated by 
   * Wintermute to have a direct proxy into the application. As opposed to the 
   * classical approach of having libraries define the Wintermute::Plugin 
   * directly and resolve its symbol, we require this to be the object in 
   * question for resolution and implementation.
   *
   * This object serves as a proxy between the library defined and the 
   * Wintermute application running this application. It provides all of the 
   * necessary aspects of manipulating and toggling the library and is meant 
   * to be small in feature set.
   *
   * @todo Update with some sensable docs.
   */
  class PluginInterface : public QObject {
    /// {{{ QObject-ified.
    Q_OBJECT;

    public:
    explicit PluginInterface();
  };
}

#endif /* WINTERMUTE_CORE_PLUGIN_INTERFACEABLE_HPP */
