/***
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 01/24/12 11:23:14 AM
 *
 * @section lcns Licensing
 * Wintermute is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#ifndef WNTR_DEBUG_HPP
#define WNTR_DEBUG_HPP

#include <QDebug>

namespace Wintermute
{
class Debug : public QDebug
{
public:
    Debug (QIODevice* device);
    Debug (QtMsgType t);
    Debug (const QDebug&);
    virtual ~Debug();
};
}

/**
 * @brief ...
 **/
typedef Wintermute::Debug wDebug;

#endif // DEBUG_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
