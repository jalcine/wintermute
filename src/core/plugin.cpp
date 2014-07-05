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

#include "plugin.hpp"

using Wintermute::Plugin;
using Wintermute::Version;

Plugin::Plugin()
{
}

QVariant Plugin::option(const QString& path, const QVariant& defaultValue) const
{
  /// TODO Load options from the settings engine.
  return defaultValue;
}

void Plugin::setOption(const QString& path, const QVariant& value)
{
  /// TODO Set options into the settings engine.
}

Version Plugin::version() const
{
  QString versionString = option("Version/Plugin").toString();
  return Version::fromString(versionString);
}

Version Plugin::abiVersion() const
{
  QString versionString = option("Version/ABI").toString();
  return Version::fromString(versionString);
}
