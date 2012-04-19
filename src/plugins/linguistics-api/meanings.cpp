/**
 * @file    meanings.cpp
 * @author  Wintermute Development <wntr-devel@thesii.org>
 * @date    August 20, 2011 8:58 PM
 * @license GPL3
 *
 * @section lcns Licensing
 * Copyright (c) SII 2010 - 2011
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// stdc++ includes
#include <iomanip>
#include <iostream>

// WntrData includes
#include <data-api/ontology/ontology.hpp>

// local includes
#include "syntax/node.hpp"
#include "syntax/link.hpp"
#include "parser/binding.hpp"
#include "parser/rule.hpp"
#include "meanings.hpp"

using namespace std;
using namespace Wintermute::Linguistics;

int Meaning::s_cnt = 0;
Meaning::Meaning() { }

Meaning::Meaning (const LinkList& p_lnkVtr) : m_lnkVtr (p_lnkVtr)
{
    __init();
}

Meaning::Meaning (const Meaning& p_meaning) : m_lnkVtr (p_meaning.m_lnkVtr)
{
    __init();
}

void Meaning::__init()
{
    m_ontoMap.clear();

    foreach (Link * lnk, m_lnkVtr) {
        Node* nd = const_cast<Node*> (lnk->source ());
        m_ontoMap.insert (nd, lnk);
    }

    qDebug() << "(ling) [Meaning] Encapsulates" << m_ontoMap.uniqueKeys ();
}

Meaning* Meaning::form (LinkList* p_linkLst, const NodeList& p_nodeList)
{
    Meaning::s_cnt++;

    if (p_linkLst == 0)
        p_linkLst = new LinkList;

    cout << endl << setw (6) << setfill ('=') << '='
         << " Level "
         << setw (4) << setfill ('0') << right << s_cnt << ' '
         << setw (6) << setfill ('=') << '=' << endl;

    NodeList::ConstIterator ndItr = p_nodeList.begin ();
    NodeList ndLst;
    QStringList* hideList = 0;
    bool hideOther = false;
    bool hideThis = false;

    if (p_nodeList.size () != 1) {
        Node* ndLeft = 0;
        Node* ndRight = 0;
        const NodeList::ConstIterator ndItrEnd = p_nodeList.end ();

        for (; ndItr != ndItrEnd; ndItr++) {
            ndLeft  =  *ndItr;
            ndRight = * (ndItr + 1);

            if (hideList) {
                const QString strNdLeft = ndLeft->toString (Node::EXTRA);
                bool hasMatchedHideList = false;
                foreach (const QString hiddenQualifier, *hideList)

                if (strNdLeft.contains (hiddenQualifier)) hasMatchedHideList = true;

                if (!hasMatchedHideList) {
                    qDebug() << "(ling) [Meaning] *** This node broke the filter; may appear on next round." << endl;
                    hideList = 0;
                    hideThis = false;
                }
                else {
                    hideThis = true;
                    qDebug() << "(ling) [Meaning] *** Node won't appear in next round due to filter '" << hideList->join (",") << "'." << endl;
                }
            }

            qDebug() << "(ling) [Meaning] Current node: " << ndLeft;

            const Binding* bnd = Binding::obtain (*ndLeft, *ndRight);
            const Link* lnk;

            if (bnd) {
                lnk = bnd->bind (*ndLeft, *ndRight);
                lnk->m_lvl = Meaning::s_cnt;
                p_linkLst->push_back (const_cast<Link*> (lnk));

                QString hide = bnd->getAttrValue ("hide");
                QString hideNext = bnd->getAttrValue ("hideNext");
                QString skipWord = bnd->getAttrValue ("skipWord");
                QString hideFilter = bnd->getAttrValue ("hideFilter");

                hide = (hide.isEmpty () || hide.isNull ()) ? "no" : hide;
                hideNext = (hideNext.isEmpty () || hideNext.isNull ()) ? "no" : hideNext;
                skipWord = (skipWord.isEmpty () || skipWord.isNull ()) ? "yes" : skipWord;

                // Attribute 'hide': Prevents this node (source node) from appearing on the next round of parsing. (default = 'no')
                if (!hideThis && !hideOther && hide == "no")
                    ndLst.push_back (const_cast<Node*> (dynamic_cast<const Node*> (lnk->source ())));
                else {
                    qDebug() << "(ling) [Meaning] *** Hid '" << lnk->source () << "' from appearing on the next pass of parsing.";
                }

                // Attribute 'hideNext': Prevents the next node (destination node) from appearing on its next round of parsing (2 rounds from now) (default = 'no')
                if (hideNext == "yes") {
                    hideOther = true;
                    qDebug() << "(ling) [Meaning] *** Hid '" << lnk->destination () << "' from appearing on the next pass of parsing (2 rounds from now).";
                }
                else hideOther = false;

                // Attribute 'skipWord': Doesn't allow the destination node to have a chance at being parsed. (default = yes)
                if (skipWord == "yes")
                    ndItr++;
                else {
                    qDebug() << "(ling) [Meaning] *** Skipping prevented for word-symbol '" << lnk->destination () << "'; will be parsed on next round.";
                }


                // Attribute 'hideFilter': Hides a set of words from appearing on the next round of parsing; a wrapper for the 'hide' attribute. (default = QString::null)
                if (hideFilter.length () != 0) {
                    QStringList* e = new QStringList;

                    if (hideFilter.contains (",")) {
                        QStringList d = hideFilter.split (",");
                        foreach (const QString q, d)
                        e->append (q);
                    }
                    else e->append (hideFilter);

                    hideList = e;
                    qDebug() << "(ling) [Meaning] *** Hiding any nodes that falls into the regex" << hideList->join (" 'or' ") << "on the next round.";
                }

#if 0
                qDebug() << "(ling) [Meaning] Flags> hide: (" << hide
                         << ") hideThis: ("   << ( (hideThis == true) ? "yes" : "no")
                         << ") hideOther: ("  << ( (hideOther == true) ? "yes" : "no")
                         << ") hideNext: ("   << hideNext
                         << ") hideFilter: (" << hideFilter
                         << ") skipWord: ("   << skipWord
                         << ") hideList: ("   << ( (hideList == 0) ? "0" : "*") << ")"
                         << endl << "Link sig: " << lnk->toString ().c_str () << endl;
#endif

            }
            else {
                qWarning() << "(ling) [Meaning] Linking failed ... horribly." << endl;
                lnk = 0;
            }

            qDebug() << endl << "(ling) [Meaning] Nodes to be queued:" << endl << ndLst << endl;

        }

        qDebug() << "(ling) [Meaning] Formed" << p_linkLst->size () << "links with" << ndLst.size () << "nodes left to parse.";
    }

    if (!p_linkLst->empty ()) {
        if (! (p_linkLst->size () >= 1) || ndLst.size () > 0) {
            qDebug() << "(ling) [Meaning] No links were made!";
            Q_ASSERT (Meaning::s_cnt < 5);
            return Meaning::form (&*p_linkLst, ndLst);
        }
        else {
            Meaning::s_cnt = 0;
            return new Meaning (*p_linkLst);
        }
    }
    else
        return 0;
}

const Link* Meaning::base () const
{
    return m_lnkVtr.back ();
}

const LinkList* Meaning::siblings () const
{
    return &m_lnkVtr;
}

const LinkList Meaning::linksAt (const int& p_lvl)
{
    LinkList lnkVtr;

    if (p_lvl > 1 && p_lvl <= levels()) {
        foreach (Link * lnk, m_lnkVtr) {
            if (lnk->level() == p_lvl)
                lnkVtr << lnk;
            else continue;
        }
    }
    else
        qDebug() << "(ling) [Meaning] Out of level range.";

    return lnkVtr;
}

const int Meaning::levels() const
{
    int lvl = 0;

    foreach (const Link * lnk, m_lnkVtr) {
        if (lvl < lnk->level())
            lvl = lnk->level();
        else continue;
    }

    return lvl;
}

const LinkList Meaning::isLinkedTo (const Node& p_nd) const
{
    LinkList lnkVtr;

    foreach (Link * lnk, m_lnkVtr) {
        if (lnk->source () == &p_nd)
            lnkVtr << lnk;
    }

    return lnkVtr;
}

const LinkList Meaning::isLinkedBy (const Node& p_nd) const
{
    LinkList lnkVtr;

    foreach (Link * lnk, m_lnkVtr) {
        if (lnk->destination () == &p_nd)
            lnkVtr << lnk;
    }

    return lnkVtr;
}

/// @note This method should be able to convert the Meaning back into text (equal or similar to what was given to form this Meaning).
const QString Meaning::toText () const
{
    if (levels() >= 2) {
        qDebug() << "(ling) [Meaning] Has" << m_lnkVtr.size () << "link(s).";
        foreach (const Link * lnk, m_lnkVtr)
        qDebug() << "(ling) [Meaning]" << lnk;
    }
    else {
        qDebug() << "(ling) [Meaning] Invalid meaning.";
    }

    return QString::null;
}

Meaning::~Meaning () { }


// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
