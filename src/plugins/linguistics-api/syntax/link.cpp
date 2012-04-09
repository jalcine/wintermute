/**  This file is part of Wintermute Linguistics
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

// Boost includes
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>
#include <boost/token_iterator.hpp>

// local includes
#include "link.hpp"

using namespace boost;
using namespace Wintermute::Linguistics;

/// @todo Create a serialized version of the Link.
const QString Link::toString() const
{
    return QString::null;
}

Link* Link::form (const Node* p_src, const Node* p_dst, const QString& p_flgs, const QString& p_lcl)
{
    return new Link (p_src , p_dst , p_flgs , p_lcl);
}

Link* Link::fromString (const QString& p_data)
{
    char_separator<char> frstLvl (":");
    char_separator<char> scndLvl (",");

    typedef tokenizer<char_separator<char> > Tokenizer;
    Tokenizer toks (p_data.toStdString() , frstLvl);

    Tokenizer::const_iterator itr = toks.begin();
    const std::string node1 = *itr,
                      node2 = * (itr++);

    Tokenizer toks2 (node1, scndLvl);
    Tokenizer toks3 (node2, scndLvl);

    const QString node1_id = QString::fromStdString (* (++ (toks2.begin()))),
                  node2_id = QString::fromStdString (* (++ (toks3.begin()))),
                  flags    = QString::fromStdString (* (itr++)),
                  lcl      = QString::fromStdString (node1);

    return new Link (Node::obtain (lcl , node1_id), Node::obtain (lcl, node2_id),
                     flags, lcl);
}
QDebug operator<< (QDebug dbg, const Link* p_lnk)
{
    if (p_lnk)
        dbg.nospace () << "[Level " << p_lnk->level() << "](type:" << p_lnk->flags () << ")"
                       << p_lnk->source () << "->" << p_lnk->destination ();
    else
        dbg.nospace() << "(null link)";

    return dbg.space();
}


// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
