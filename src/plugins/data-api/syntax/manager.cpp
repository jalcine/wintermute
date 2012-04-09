/**
 * @file rules.hpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 * @date Sun, 30 Oct 2011 21:54:16
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

// local includes
#include "bond.hpp"
#include "model.hpp"
#include "manager.hpp"
#include "../system.hpp"

using Wintermute::Data::Linguistics::Syntax::Chain;
using Wintermute::Data::Linguistics::Syntax::Cache;
using Wintermute::Data::Linguistics::Syntax::Manager;
using Wintermute::Data::System;

Manager* Manager::s_inst = 0;

Manager::Manager() : QObject (System::instance()) { }

Manager::Manager (const Manager& p_other) : QObject (p_other.parent())
{

}

bool Manager::exists (const QString& p_1, const QString& p_2) const
{
    return Cache::exists (p_1, p_2);
}

void Manager::read (::Chain& p_chn)
{
    Cache::read (p_chn);
}

void Manager::write (::Chain& p_chn)
{
    Cache::write (p_chn);
}

Manager* Manager::instance()
{
    if (!s_inst) s_inst = new Manager;

    return s_inst;
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
