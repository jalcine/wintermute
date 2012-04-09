/**
 * @file    meanings.hpp
 * @author  Wintermute Development <wntr-devel@thesii.org>
 * @date    August 20, 2011 8:54 PM
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

#ifndef WNTRLING_MEANINGS_HPP
#define WNTRLING_MEANINGS_HPP

// Qt includes
#include <QObject>
#include <QMultiMap>

// local includes
#include "syntax/node.hpp"
#include "syntax/link.hpp"

namespace Wintermute
{
namespace Linguistics
{
struct Meaning;

/**
 * @brief Represents a QList of Meaning objects.
 * @see Meaning
 * @typedef MeaningVector
 */
typedef QList<Meaning*> MeaningList;

/**
 * @brief Represents a QMultiMap between Node objects and Link objects.
 * @typedef OntoMap
 */
typedef QMultiMap<Node*, Link*> OntoMap;

/**
 * @brief Represents the end-result of linguistics linking.
 *
 * The Meaning class does the handy-dandy work of converting a NodeVector
 * filled with flat Node objects from:
 *
 * @dot
    digraph L {
            graph [layout=dot rankdir=TB]

            {The book on the table is really red}
    }
 * @enddot
 * into a syntactical bonded sentence, as shown:
 * @dot

    digraph M {
            graph [layout=dot rankdir=LR]

            The -> book
            book -> on -> the
            the -> table
            book -> is -> red
            really -> red
    }
 * @enddot
 *
 * It uses the Binding and Rule objects to determine which Node goes with which
 * and modify the linking process. This is where the conversion from natural text
 * to an intermediate form of ontological syntax is done.
 *
 * @see Binding
 * @see Rule
 * @class Meaning parser.hpp "src/parser.hpp"
 */
class Meaning : public QObject
{
    Q_OBJECT

    Q_PROPERTY (const Link* base READ base)
    Q_PROPERTY (const LinkList* siblings READ siblings)

public:
    /**
     * @brief Empty constructor.
     * @fn Meaning
     */
    Meaning();

    /**
     * @brief Default constructor.
     * @fn Meaning
     * @param p_lnkVtr The LinkList to be incorporated.
     */
    explicit Meaning (const LinkList&);

    /**
     * @brief Copy constructor.
     * @fn Meaning
     * @param p_mng The Meaning to be copied.
     */
    Meaning (const Meaning&);

    /**
     * @brief Deconstructor.
     * @fn ~Meaning
     */
    ~Meaning();

    /**
     * @brief Obtains the base Link of this Meaning.
     * @fn base
     */
    Q_INVOKABLE const Link* base() const;

    /**
     * @brief Obtains all of the Link objects in this Meaning, excluding the base Link.
     * @fn siblings
     * @return A LinkList of every Link but the base Link.
     */
    Q_INVOKABLE const LinkList* siblings() const;

    /**
     * @brief Obtains the textual representation of a Meaning.
     * @fn toText
     */
    const QString toText() const;

    /**
     * @brief Obtains the number of levels in a Meaning.
     * @fn levels
     */
    const int levels() const;

    /**
     * @brief Obtains the Link objects at a certain level.
     * @fn linksAt
     * @param p_lvl The level to get the Link objects at.
     */
    const LinkList linksAt (const int& = 1);

    /**
     * @brief Obtains a LinkList of Link objects that of which the specified Node is the source.
     * @fn isLinkedTo
     * @param p_node The Node in question.
     * @todo Change this into 'linkedTo' and create an accompanying boolean method.
     */
    const LinkList isLinkedTo (const Node&) const;

    /**
     * @brief Obtains a LinkList of Link objects that of which the specified Node is the destination.
     * @fn isLinkedBy
     * @param p_node The Node in question.
     * @todo Change this into 'linkedBy' and create an accompanying boolean method.
     */
    const LinkList isLinkedBy (const Node&) const;

    /**
     * @brief Forms a Meaning from a LinkList and a NodeList.
     * @fn form
     * @param p_lnkLst The list of Link objects.
     * @param p_nodeLst The list of Node objects.
     */
    static const Meaning* form (LinkList* , const NodeList&);

protected:
    LinkList m_lnkVtr;
    OntoMap m_ontoMap;

private:
    void __init();
    static int s_cnt;
};
}
}

Q_DECLARE_METATYPE (Wintermute::Linguistics::Meaning)

#endif

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
