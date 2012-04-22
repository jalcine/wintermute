/***
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 01/24/12 11:23:01 AM
 *
 * @section lcns Licensing
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
 *
 */

#include "debug.hpp"

using Wintermute::Debug;

Debug::Debug (QIODevice* p_device) : QDebug (p_device)
{

}

Debug::Debug (QtMsgType t) : QDebug (t)
{

}

Debug::Debug (const QDebug& other) : QDebug (other)
{

}

Debug::~Debug()
{

}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
