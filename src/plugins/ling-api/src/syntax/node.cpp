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

// Qt includes
#include <QDebug>
#include <QVariantMap>
#include <QDBusPendingReply>

// wntrdata includes
#include <wntrdata.hpp>

// local includes
#include "node.hpp"
#include "dbus/nodeinterface.hpp"

using Wintermute::Linguistics::Node;
using Wintermute::Linguistics::NodeList;
using Wintermute::Linguistics::NodeTree;

const QString Node::toString (const Node::FormatVerbosity& p_density) const
{
    QVariantMap::ConstIterator flgItr = m_lxdt.flags ().begin ();
    QString sig;

    switch (p_density) {
    case MINIMAL:
        sig = flgItr.value ().toString().at (0);
        break;

    case EXTRA:
        sig = flgItr.value ().toString();
        break;

    default:
    case FULL:
        sig = flgItr.value ().toString();
        sig += "[";
        sig += flgItr.key();
        sig += "]";
        break;
    }

    return sig;
}

const QString Node::toString (const Node* p_nd, const FormatVerbosity& p_density)
{
    return p_nd->toString (p_density);
}

const QString Node::toString (const NodeList& p_ndVtr, const FormatVerbosity& p_density)
{
    QString sig;

    for (NodeList::const_iterator itr = p_ndVtr.begin (); itr != p_ndVtr.end (); itr++) {
        sig += (*itr)->toString (p_density);

        if ( (itr + 1) != p_ndVtr.end ())
            sig += ">";
    }
}

Node* Node::create (const Lexical::Data& p_dt)
{
    qDebug() << "(ling) [Node] Created node for" << p_dt.id() << ".";
    Data::NodeInterface* ndInt = new Data::NodeInterface;
    QDBusPendingReply<QString> replyDt = ndInt->write (p_dt);
    replyDt.waitForFinished();
    const Lexical::Data dt = Lexical::Data::fromString (replyDt);
    return Node::obtain (dt.locale (), dt.id ());
}

Node* Node::obtain (const QString& p_lcl, const QString& p_id)
{
    Lexical::Data dt (p_id , p_lcl);

    if (exists (p_lcl , p_id)) {
        Data::NodeInterface* ndInt = new Data::NodeInterface;
        QDBusPendingReply<QString> reply = ndInt->read (dt);
        reply.waitForFinished();
        return new Node (Lexical::Data::fromString (reply));
    }

    return 0;
}

Node* Node::buildPseudo (const QString& p_lcl, const QString& p_sym)
{
    qDebug() << "(ling) [Node] Building pseudo-node for" << p_sym << "...";
    Lexical::Data dt (QString::null , p_lcl , p_sym);
    Data::NodeInterface* ndInt = new Data::NodeInterface;
    QDBusPendingReply<QString> reply = ndInt->pseudo (dt);
    reply.waitForFinished();

    if (reply.isValid()) {
        dt = Lexical::Data::fromString ( (QString) reply);
        qDebug() << "(ling) [Node] Formed psuedo:" << dt;
        return new Node (dt);
    }
    else {
        if (reply.isError())
            qDebug() << "Error building psuedo node for " << p_sym << ":"
                     << reply.error();
    }

    return 0;
}

const bool Node::exists (const QString& p_lcl, const QString& p_id)
{
    Data::System::registerDataTypes();
    Lexical::Data dt (p_id, p_lcl);
    Data::NodeInterface* ndInt = new Data::NodeInterface;
    QDBusPendingReply<bool> reply = ndInt->exists (dt);
    reply.waitForFinished();

    if (reply.isValid())
        return reply.value();
    else {
        if (reply.isError())
            qDebug() << reply.error();
    }

    return false;
}

Node* Node::form (const Lexical::Data dt)
{
    return new Node (dt);
}

NodeList Node::expand (const Node* p_nd)
{
    NodeList vtr;
    QVariantMap map;
    int indx = 0;

    map = p_nd->flags ();

    for (QVariantMap::iterator itr = map.begin (); itr != map.end (); indx++, itr++) {
        QVariantMap mp;
        mp.insert (itr.key (), itr.value ());
        Lexical::Data dt (p_nd->id (), p_nd->locale (), p_nd->symbol ());
        dt.setFlags (mp);
        vtr.push_back (const_cast<Node*> (Node::form (dt)));
    }

    qDebug() << "(ling) [Node] Expanded symbol" << p_nd->symbol () << "to spread across its" << map.size() << "variations.";

    return vtr;
}

QDebug operator<< (QDebug dbg, const Node* p_nd)
{
    Q_ASSERT (p_nd);
    dbg.nospace () << "[" << p_nd->symbol () << " (" << p_nd->toString (Node::EXTRA) << "):" << p_nd->locale ().toStdString ().c_str () << "]";
    return dbg.space();
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
