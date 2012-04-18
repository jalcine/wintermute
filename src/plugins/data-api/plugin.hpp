/**
 * @file linguistics.hpp
 * @author: Wintermute Development <wntr-devel@thesii.org>
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
 * @date March 29, 2011, 3:08 PM
 */

#ifndef WNTRDATA_PLUGIN_HPP
#define WNTRDATA_PLUGIN_HPP

// Wintermute includes
#include <plugins.hpp>

namespace Wintermute
{
namespace Data
{
class Plugin : public Plugins::AbstractPlugin
{
    Q_OBJECT
public:
    Plugin();
    virtual ~Plugin();
    virtual void start() const;
    virtual void stop() const;
};

}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
