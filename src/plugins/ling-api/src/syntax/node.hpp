/**
 * @file wntrling.hpp
 * This file is part of Wintermute Linguistics
 *
 * Copyright (C) 2011 - Wintermute Development <wntr-devel@thesii.org>
 *
 * Wintermute Linguistics is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute Linguistics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute Linguistics; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef WNTRLING_NODE_HPP
#define WNTRLING_NODE_HPP

// Qt includes
#include <QList>
#include <QString>

// WntrData includes
#include <lexical/data.hpp>
#include <lexical/model.hpp>

namespace Wintermute
{
namespace Linguistics
{

using namespace Wintermute::Data::Linguistics;

class Node;
/**
  * @brief Represents a @c QVector of @c Nodes;
  * @typedef NodeVector
  */
typedef QList<Node*> NodeList;

/**
 * @brief Represents a QList of NodeList objects.
 * @see Parser::expandNodes()
 * @typedef NodeTree
 */
typedef QList<NodeList> NodeTree;

/**
  * @brief An object representing the lexical and syntactic bindings of a word-symbol.
  *
  * Wintermute's natural language processing system uses nodes to represent individual word
  * symbols in lexical representation and in syntactical representation. Nodes are used to
  * represent either a collection or an individual set of symbols from natural text. We
  * use the term "word symbol" or just "symbol" to represent words in English or possibly
  * hieroglyphs in ancient Egypt, to ensure a transparency of meaning. Some Node objects may have
  * a single meaning, for example:
  * @dot
    digraph L {
        graph [layout=dot rankdir=LR]

        {Apple} -> ontoApple
    }
  * @enddot
  *
  * But some Node objects, like pronouns in the English and romance languages can expand to have many
  * meanings, as depicted:
  * @dot
    digraph L {
        graph [layout=dot rankdir=TB]

        {YouGroup YouAre YouSecond YouThird} ->  You
    }
  * @enddot
  *
  * We have a convience method, expand(), that splits a Node into the different meanings it represents.
  *
  * @class Node syntax.hpp "src/syntax.hpp"
  */
/// @todo Remove all of the in-line definitions of methods that _aren't_ inlined.
class Node : public QObject
{
    Q_GADGET
    Q_PROPERTY (const QString id READ id)
    Q_PROPERTY (const QString locale READ locale)
    Q_PROPERTY (const QString symbol READ symbol)
    Q_PROPERTY (const QString value READ toString)
    Q_PROPERTY (const QVariantMap flags READ flags)
    Q_PROPERTY (const Data* data READ data)
    Q_ENUMS (FormatVerbosity)

    friend class Link;

protected:
    Lexical::Data m_lxdt;

public:
    /**
    * @brief The format verbosity of a Node in text.
    *
    * The enumeration shown here represents the verbosity of the information
    * generated when representing a @c Node as a string.
    *
    * @enum FormatVerbosity.
    */
    enum FormatVerbosity {
        FULL = 0, /**< Represents a full dump of the node's first flag, ontological mapping, and it's ID. */
        MINIMAL, /**< Represents only the first flag's first letter. */
        EXTRA /**< Represents the first flag.  */
    };

    /**
    * @brief Null constructor.
    * @fn Node
    */
    Node() : m_lxdt() { }

    /**
    * @brief Default constructor.
    * @fn Node
    * @param p_lxdt The Lexical::Data representing the internal data of the Node.
    */
    explicit Node (Lexical::Data p_lxdt) : m_lxdt (p_lxdt) {
        this->setProperty ("OriginalToken", symbol ());
    }

    /**
    * @brief Copy constructor.
    * @fn Node
    * @param p_nd The node being copied.
    */
    Node (const Node& p_nd) : m_lxdt (p_nd.m_lxdt) {
        this->setProperty ("OriginalToken", symbol ());
    }

    /**
    * @brief Deconstructor.
    * @fn ~Node
    */
    ~Node() { }

    /**
    * @brief Obtains the ID of the Node.
    * @fn id
    * @return The internal ID of the node, or an empty QString.
    * @note It's very unlikely that you'd get an empty QString, as an update to the symbol would
    *       generate an ID, and in order to obtain the internal Lexical::Data, the ID is required.
    * @see Node(Lexical::Data p_lxdt)
    * @see Lexical::Data
    */
    Q_INVOKABLE inline const QString id() const {
        return m_lxdt.id ();
    }

    /**
    * @brief Obtains the locale of the Node.
    * @fn locale
    * @return The locale of the Node, or an empty QString.
    * @note It's very unlikely that you'd get an empty QString, due to the fact
    *        the internal Lexical::Data requires a value for the locale.
    * @see Node(Lexical::Data p_lxdt)
    * @see Lexical::Data
    */
    Q_INVOKABLE inline const QString locale() const {
        return m_lxdt.locale ();
    }

    /**
    * @brief Obtains the symbol of the Node.
    * @fn symbol
    * @return The symbol of the Node, or an empty QString.
    */
    Q_INVOKABLE inline const QString symbol() const {
        return m_lxdt.symbol ();
    }

    /**
    * @brief Obtains the flags of the Node.
    * @fn flags
    * @return The flags of the Node.
    */
    Q_INVOKABLE inline const QVariantMap flags() const {
        return m_lxdt.flags ();
    }

    /**
    * @brief Obtains the internal Lexical::Data object.
    * @fn data
    * @return The Lexical::Data defining this node.
    */
    Q_INVOKABLE inline const Lexical::Data* data() const {
        return &m_lxdt;
    }

    /**
    * @brief Determines if the Node is 'flat'.
    *
    * A Node is considered 'flat' if and only if the size of the QVariantMap returned
    * by flags() is 1.
    *
    * @fn isFlat
    * @return 'true' if flags().size == 1, returns 'false' otherwise.
    */
    Q_INVOKABLE inline const bool isFlat() const {
        return this->flags ().size () == 1;
    }

    /**
    * @brief Determines if the Node is 'pseudo'.
    *
    * Some nodes can be pseudo-nodes, meaning that their real meaning is unknown
    * to a Parser. Pseudo nodes contain the locale it was detected in, and its
    * symbol. Typically, the ID and its flags are internally reserved for such a Node.
    *
    * @fn isPseudo
    * @return 'true' if the flags and ID are pseudo-worthy, returns 'false' otherwise.
    */
    Q_INVOKABLE inline const bool isPseudo() const {
        return Lexical::Cache::isPseudo (m_lxdt);
    }

    /**
    * @brief Prints out a QString representing this Node.
    * @fn toString
    * @param p_verbosity The verbosity of the printing of the Node.
    */
    Q_INVOKABLE const QString toString (const FormatVerbosity& = FULL) const;

    /**
    * @brief Prints out a QString representing a specific Node.
    * @fn toString
    * @param p_node The Node to be printed.
    * @param p_verbosity The verbosity of the printing of the Node.
    */
    static const QString toString (const Node* , const FormatVerbosity& = FULL);

    /**
    * @brief Prints out an entire NodeVector using a specific FormatVerbosity.
    * @fn toString
    * @param p_ndVtr The vector to be printed out.
    * @param p_verbosity The verbosity for each Node in the NodeVector to be printed with.
    */
    static const QString toString (const NodeList& , const FormatVerbosity& = FULL);

    /**
    * @brief Determines if a specific Node exists.
    * @fn exists
    * @param p_lcl The locale of the potential Node.
    * @param p_sym The symbol of the potential Node.
    */
    static const bool exists (const QString&, const QString&);

    /**
    * @brief Obtains a Node from a specific locale with a specific symbol.
    * @fn obtain
    * @param p_lcl The locale of the potential Node.
    * @param p_sym The symbol of the potential Node.
    * @return A Node if the Node exists, or 0 if it doesn't.
    */
    static Node* obtain (const QString&, const QString&);

    /**
    * @brief Creates a new Node based on a Lexical::Data.
    * @fn create
    * @param p_data The Lexical::Data to generate a Node with.
    * @return The Node generated from the Lexical::Data or 0 if the operation failed.
    * @warning This method may fail quietly, if the Lexical::Cache is unable to write the Lexical::Data to disk.
    *          In a future implementation that uses error handling via exceptions, you'll be able to catch such a failure.
    */
    static Node* create (const Lexical::Data&);

    /**
    * @brief Generates a Node from a QString.
    * @fn fromString
    * @param p_str The QString to generate the Node from.
    * @see obtain()
    * @deprecated Use the obtain() method to obtain a Node from a QString.
    * @obsolete This method will be removed in a later micro-version of WntrLing.
    */
    static Node* fromString (const QString&);

    /**
    * @brief Builds a pseudo-Node with the specified locale and symbol.
    * @fn buildPseudo
    * @param p_lcl The locale to use.
    * @param p_sym The symbol of the Node.
    * @return A Node representing the pseudo-Node, or 0 if no pseudo-Nodes could be formed.
    * @todo Check if this Node exists. If it does, return that instead of forming a pseudo-Node.
    */
    static Node* buildPseudo (const QString&, const QString&);

    /**
    * @brief Creates a Node based on a specific Lexical::Data.
    * @fn form
    * @param p_lxdt The Lexical::Data representing the internal data of the Node.
    * @return A new Node with @var p_lxdt at its core.
    * @see Node(Lexical::Data p_lxdt)
    */
    static Node* form (Lexical::Data);

    /**
    * @brief Splits a Node into many Nodes.
    *
    * As mentioned in the preface, Nodes can represent a single symbol
    * but contain many different meanings. This method returns a list of
    * Nodes in a NodeList that each have only one of the flags defined in
    * their originating Node.
    *
    * @fn expand
    * @param p_node The Node to split.
    * @return A NodeList with a Node for each flag defind in p_node.
    */
    static NodeList expand (const Node*);

    /**
    * @brief Equality operator.
    * @fn operator==
    * @param p_nd
    * @return bool
    */
    bool operator== (const Node& p_nd) {
        return this->id () == p_nd.id () &&
               this->m_lxdt.locale () == this->m_lxdt.locale ();
    }

};

}
}

/**
 * @brief Debug utility function.
 * @fn operator <<
 * @param QDebug
 * @param p_node The Node to be printed to the stream.
 */
QDebug operator<< (QDebug , const Wintermute::Linguistics::Node*);

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
