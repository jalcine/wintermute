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

#ifndef WNTRLING_LINK_HPP
#define WNTRLING_LINK_HPP

// Qt includes
#include <QList>
#include <QObject>

// local includes
#include "node.hpp"

namespace Wintermute
{
namespace Linguistics
{

class Link;
/**
  * @brief Represents a @c QVector of @c Links;
  * @typedef LinkVector
  */
typedef QList<Link*> LinkList;

/**
  * @brief Represents the syntactical binding of two Node objects by a pre-defined relationship.
  *
  * The binding object that describes the relationship between two nodes can be found
  * in this class. The @c Link object demonstrates the syntactic connection between two
  * nodes. Links are considered to be the <i>predicate</i>, in <a href="http://www.w3.org/TR/REC-rdf-syntax">RDF</a>
  * syntax, and they represent a relationship between two Node objects (as shown below).
  *
  * @dot
    digraph L {
        graph [layout=dot rankdir=TB]

        {Node1 Node2} -> Link -> Relationship
    }
  * @enddot
  *
  * @class Link syntax.hpp "src/syntax.hpp"
  * @internal
  */
class Link : public QObject
{
    friend class Meaning;
    Q_OBJECT
    Q_PROPERTY (const Node* Source READ source)
    Q_PROPERTY (const Node* Destination READ destination)
    Q_PROPERTY (const QString Locale READ locale)
    Q_PROPERTY (const QString Flags READ flags)
    Q_PROPERTY (const QString Value READ toString)
    Q_PROPERTY (const int Level READ level)

public:
    /**
      * @brief Generates a Link from two Nodes, binding flags and a locale setting.
      *
      * The link formed would look something like this:
      *
      * @dot
        digraph L {
            graph [layout=dot rankdir=TB]

            {SourceNode DestinationNode} -> Link -> {Locale Flags}
        }
      * @enddot
      *
      * @fn form
      * @param p_node1 The source Node.
      * @param p_node2 The destination Node.
      * @param p_flag The flags defining the relationship between the source Node and the destination Node.
      * @param p_lcl The locale of the link.
      * @warning The locale property may become obsolete in order to ensure that we're able to support translations.
      */
    static Link* form (const Node* , const Node* , const QString&, const QString&);

    /**
      * @brief Forms a link from a QString.
      * @fn fromString
      * @param p_str The QString representing a Link.
      */
    static Link* fromString (const QString&);

    /**
      * @brief Obtains the source Node.
      * @fn source
      * @return A Node representing the source of the Link.
      */
    Q_INVOKABLE inline const Node* source() const {
        return m_src;
    }

    /**
      * @brief Obtains the destination Node.
      * @fn destination
      * @return A Node representing the destination of the Link.
      */
    Q_INVOKABLE inline const Node* destination() const {
        return m_dst;
    }

    /**
      * @brief
      *
      * @fn level
      * @return const int
      */
    Q_INVOKABLE inline const int level() const {
        return m_lvl;
    }

    /**
      * @brief Obtains the locale of the Link.
      * @fn locale
      * @return The specified locale of the Link.
      * @deprecated This method, as long with the concept of locale for Links, are phasing out.
      * @obsolete
      */
    Q_INVOKABLE inline const QString locale() const {
        return m_lcl;
    }

    /**
      * @brief Obtains the flags of the Link.
      * @fn flags
      * @return The flags describing the relationship of this Link.
      */
    Q_INVOKABLE inline const QString flags() const {
        return m_flgs;
    }

    /**
      * @brief Obtains a QString that represents this Link.
      * @fn toString
      */
    Q_INVOKABLE const QString toString() const;

    /**
      * @brief Null constructor.
      * @fn Link
      */
    Link() : m_lcl (QString::null), m_flgs (QString::null), m_src (0), m_dst (0), m_lvl (0) { }

    /**
      * @brief Copy constructor.
      * @fn Link
      * @param p_lnk The Link to be copied.
      */
    Link (const Link& p_lnk) : m_src (p_lnk.m_src), m_dst (p_lnk.m_dst), m_flgs (p_lnk.m_flgs), m_lcl (p_lnk.m_lcl), m_lvl (0) { }

protected:
    /**
      * @brief Initialization constructor.
      * @fn Link
      * @param p_src The source Node.
      * @param p_dst The destination Node.
      * @param p_flgs The relationship described by the flags of the source Node's locale.
      * @param p_lcl The locale of the flag.
      */
    Link (const Node* p_src, const Node* p_dst, const QString& p_flgs, const QString& p_lcl) :
        m_src (p_src), m_dst (p_dst), m_flgs (p_flgs), m_lcl (p_lcl), m_lvl (0) { }

private:
    mutable int m_lvl;
    const Node* m_src;
    const Node* m_dst;
    const QString m_flgs;
    const QString m_lcl;
};

}
}

/**
 * @brief Debug utility function.
 * @fn operator <<
 * @param QDebug
 * @param p_link The Link to be printed to the stream.
 */
QDebug operator<< (QDebug , const Wintermute::Linguistics::Link*);

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
