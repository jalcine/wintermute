/*
 *  This file is part of Wintermute Linguistics
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

#include <QDebug>
#include <QVariantMap>
#include <QDBusPendingReply>
#include <QDBusInterface>
#include <data-api/wntrdata.hpp>
#include <data-api/lexical/data.hpp>
#include <data-api/dbus/nodeinterface.hpp>
#include "node.hxx"
#include "node.hpp"

using namespace Wintermute::Linguistics;
using namespace Wintermute::Data::Linguistics::Lexical;

Node::Node() : QObject(), d_ptr (new NodePrivate)
{

}

Node::Node (const Lexical::Data& p_lxdt) : QObject(), d_ptr (new NodePrivate)
{
    Q_D (Node);
    d->data = p_lxdt;
    setProperty ("OriginalToken", symbol ());
}

Node::Node (const Node& p_node) : QObject (p_node.parent()),
    d_ptr (const_cast<NodePrivate*> (p_node.d_ptr.data()))
{
    setProperty ("OriginalToken", symbol ());
}

Lexical::Data Node::data() const
{
    Q_D (const Node);
    return d->data;
}

QString Node::toString (const Node::FormatVerbosity& p_density) const
{
    Q_D (const Node);
    QVariantMap flags = d->data.flags();

    if (!flags.isEmpty()) {
        QVariantMap::ConstIterator flgItr = flags.begin();
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

    return "<?>";
}

const QString Node::toString (const Node* p_node, const Node::FormatVerbosity& p_verbosity)
{
    return p_node->toString (p_verbosity);
}

const QString Node::toString (const NodeList& p_nodeVector, const Node::FormatVerbosity& p_verbosity)
{
    QString sig;

    for (NodeList::const_iterator itr = p_nodeVector.begin (); itr != p_nodeVector.end (); itr++) {
        sig += Node::toString ( (*itr), p_verbosity);

        if ( (itr + 1) != p_nodeVector.end ())
            sig += ">";
    }

    return sig;
}

Node* Node::create (const Lexical::Data& p_data)
{
    qDebug() << "(ling) [Node] Created node for" << p_data.id() << ".";
    Data::NodeInterface* ndInt = new Data::NodeInterface;
    QDBusPendingReply<QString> replyDt = ndInt->write (p_data);
    replyDt.waitForFinished();
    const Lexical::Data data = Lexical::Data::fromString (replyDt);
    return Node::obtain (data.locale (), data.id ());
}

Node* Node::obtain (const QString& p_locale, const QString& p_id)
{
    Lexical::Data data (p_id , p_locale);

    if (exists (p_locale , p_id)) {
        Data::NodeInterface* ndInt = new Data::NodeInterface;
        QDBusPendingReply<QString> reply = ndInt->read (data);
        reply.waitForFinished();
        return new Node (Lexical::Data::fromString (reply));
    }

    return 0;
}

Node* Node::buildPseudo (const QString& p_locale, const QString& p_symbol)
{
    qDebug() << "(ling) [Node::buildPseudo] Building pseudo-node for" << p_symbol << "with locale" << p_locale << "...";
    Lexical::Data dt (QString::null , p_locale , p_symbol);
    qDebug() << dt.toJson();
    Data::NodeInterface* ndInt = new Data::NodeInterface;
    QDBusPendingReply<QString> reply = ndInt->pseudo (dt);
    reply.waitForFinished();

    qDebug() << "(ling) [Node::buildPseudo] Response:" << reply.value();

    if (reply.isValid() && !reply.value().isNull()) {
        dt = Lexical::Data::fromString (reply);

        if (dt.isValid()) {
            qDebug() << "(ling) [Node::buildPseudo] Formed pseudo node:" << dt.toJson();
            return new Node (dt);
        }
    }
    else {
        if (reply.isError())
            qDebug() << "Error building pseudo node for " << p_symbol << ":"
                     << reply.error();
    }

    qDebug() << "(ling) [Node] Couldn't form pseudo-node for" << p_symbol << "of locale" << p_locale;
    return 0;
}

const bool Node::exists (const QString& p_locale, const QString& p_id)
{
    Data::System::registerDataTypes();
    Lexical::Data dt;
    dt.setLocale (p_locale);
    dt.setId(p_id);
    qDebug() << "(ling) [Node::exists()] Data in question: " << dt.toJson();
    Data::NodeInterface* ndInt = new Data::NodeInterface;
    QDBusPendingReply<bool> reply = ndInt->exists (dt);
    reply.waitForFinished();
    bool result = false;

    result = reply.isValid();

    if (reply.isError())
        qDebug() << "(ling) [Node::exists()] Error determining existence of node's data:" << reply.error();

    return false;
}

Node* Node::form (const Lexical::Data& p_data)
{
    return new Node (p_data);
}

NodeList Node::expand (const Node& p_node)
{
    NodeList vtr;
    QVariantMap map;
    int indx = 0;

    map = p_node.flags ();

    for (QVariantMap::iterator itr = map.begin (); itr != map.end (); indx++, itr++) {
        QVariantMap mp;
        mp.insert (itr.key (), itr.value ());
        Lexical::Data dt = p_node.data();
        dt.setFlags (mp);
        vtr.push_back (const_cast<Node*> (Node::form (dt)));
    }

    qDebug() << "(ling) [Node] Expanded symbol" << p_node.symbol () << "to spread across its" << map.size() << "variations.";

    return vtr;
}

bool Node::operator== (const Node& p_node)
{
    return this->id () == p_node.id () &&
           this->locale() == this->locale();
}

Node::~Node()
{

}

QDebug operator<< (QDebug dbg, const Node* p_node)
{
    Q_ASSERT (p_node);
    dbg.nospace () << "Node [" << p_node->symbol () << " (" << p_node->toString (Node::FULL) << "):" << qPrintable (p_node->locale ()) << "]";
    return dbg.space();
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
