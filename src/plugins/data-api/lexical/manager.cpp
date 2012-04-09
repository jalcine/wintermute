/**
 * @file manager.cpp
 * @author Jacky Alcine <jacky.alcine@thesii.org>
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
#include "data.hpp"
#include "model.hpp"
#include "manager.hpp"
#include "system.hpp"

using namespace Wintermute::Data::Linguistics;
using Lexical::Data;
using Lexical::Manager;

Manager* Manager::s_inst = 0;

Manager::Manager (const Manager& p_other) : QObject (p_other.parent())
{

}

Manager::Manager() : QObject (Wintermute::Data::System::instance())
{
}

void Manager::generate()
{
    Lexical::Cache::generate();
}

Data& Manager::pseudo (Data& p_dt) const
{
    Lexical::Cache::pseudo (p_dt);
    return p_dt;
}

/// @todo Should this return a pseudo node of the passed data if the said node doesn't exist?
Data& Manager::read (Data& p_dt) const
{
    if (!Lexical::Cache::read (p_dt))
        Lexical::Cache::pseudo (p_dt);

    return p_dt;
}

const Data& Manager::write (const Data& p_dt)
{
    Lexical::Cache::write (p_dt);
    return p_dt;
}

bool Manager::exists (const Data& p_dt) const
{
    qDebug() << "(data) [Manager] Exists? " << p_dt.id() << Lexical::Cache::exists (p_dt);
    return Lexical::Cache::exists (p_dt);
}

bool Manager::isPseudo (const Data& p_dt) const
{
    return Lexical::Cache::isPseudo (p_dt);
}

Manager* Manager::instance()
{
    if (!s_inst) s_inst = new Manager;

    return s_inst;
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
