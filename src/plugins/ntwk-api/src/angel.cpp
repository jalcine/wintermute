/**
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

 * @file angel.cpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 * @date October 29, 2011 3:53 PM
 */

#include "angel.hpp"

namespace Wintermute
{
namespace Network
{
Instance::Instance (QObject* p_obj) : QObject (p_obj) { }

Instance::Instance (const Instance& p_inst) : QObject (p_inst.parent()) { }

Instance::~Instance() { }
}
}

#include "angel.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
