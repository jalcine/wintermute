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

#ifndef WINTERMUTE_VERSION_HPP
#define WINTERMUTE_VERSION_HPP

#include <QtCore/QString>
#include <QtCore/QVariant>

namespace Wintermute
{
struct Version
{
  quint16 major;
  quint16 minor;
  quint16 patch;

  QString toString() const;
  static const Version fromString(const QString& versionString);
  // TODO Implement equality operator !=.
  // TODO Implement equality operator ==.
  // TODO Implement equality operator >.
  // TODO Implement equality operator <.
  // TODO Implement cast operator to QString.
};
};

#endif
