/*
 *  The basis of an indefinitely expanded application framework.
 *  Copyright 2014 Jacky Alcinè <me@jalcine.me>
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation; either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WINTERMUTE_PLUGIN_HPP
#define WINTERMUTE_PLUGIN_HPP

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include "version.hpp"
#include "export.hpp"

namespace Wintermute
{
class Version;
/*
 * The basis of plugins for Wintermute.
 * This class serves as an interface for other plug-ins to work with.
 */
class Plugin : public QObject
{
  Q_OBJECT;
  Q_PROPERTY(Version version    READ version);
  Q_PROPERTY(Version abiVersion READ abiVersion);

protected:
  ///< Default constructor for plugins.
  explicit Plugin();

  /*
   * Obtains the value of the option designated by the provided path.
   * @param path The path of which the option is stored.
   * @param defaultValue The value to return in the event that the option is
   * empty.
   * @return QVariant A variant form of the value in question.
   */
  QVariant option(const QString& path, const QVariant& defaultValue = QVariant()) const;

  ///< Attempts to load this plugin.
  virtual bool load() const = 0;

  ///< Attempts to load this plugin.
  virtual bool unload() const = 0;
  
  ///< Determines if this plugin is loaded.
  virtual bool isLoaded() const = 0;

  /*
   * Sets a new option or updates an existing option with provided value.
   * @param path The slash-separated paths of the option in question.
   * @param value The new value to use in this place.
   */
  void setOption(const QString& path, const QVariant& value);

public:
  ///< Default destructor for Plugin.
  virtual ~Plugin();

  ///< The current version of the version.
  Version version() const;

  ///< The minimal required version of Wintermute's core library to operate.
  Version abiVersion() const;
};
}

#endif
