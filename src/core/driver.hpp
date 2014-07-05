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

#ifndef WINTERMUTE_DRIVER_HPP
#define WINTERMUTE_DRIVER_HPP

#include <QtCore/QObject>
#include "export.hpp"

namespace Wintermute
{
/*
 * The focal point of activity for Wintermute as a library.
 * This library serves as the primary driver into the operation of
 * Wintermute. The idea is that this 'static' class would handle the startng up
 * and the shutting down of things pertaining to Wintermute. At the most, this
 * serves as an utility to drive the rest of Wintermute. It also serves as an
 * anchor: this class is at the top of the object hierarchy when it comes to
 * things made in Wintermute land.
 */
class Driver
{
};
}

#endif
