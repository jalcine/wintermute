/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 03/04/12 5:56:02 AM
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
 * @endlegalese
 */

#ifndef WNTRLING_RULE_HPP
#define WNTRLING_RULE_HPP

// Qt includes
#include <QList>
#include <QObject>

// WntrData includes
#include <syntax/syntax.hpp>

// local
#include "binding.hpp"

namespace Wintermute
{
namespace Data
{
namespace Linguistics
{
namespace Syntax
{
class Chain;
}
}
}
namespace Linguistics
{
class Node;
class Link;
class Rule;
using Data::Linguistics::Syntax::Chain;
/**
 * @brief Represents a QList of Rule objects.
 * @see Rule
 * @typedef RuleList
 */
typedef QList<Rule*> RuleList;

struct RulePrivate;

/**
 * @brief Represents a set of bindings that permit linguistics links to be converted into ontological links.
 * @class Rule parser.hpp "src/parser.hpp"
 */
class Rule : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Rule)
    Q_PROPERTY (QString type READ type)
    Q_PROPERTY (QString locale READ locale)

    friend class RuleSet;
public:
    Q_DISABLE_COPY(Rule)
    virtual ~Rule();
    explicit Rule (QObject* parent = 0);

    /**
     * @brief
     *
     * @fn Rule
     * @param
     */
    Rule (const Data::Linguistics::Syntax::Chain&);
    /**
     * @brief Returns a Rule that's satisified by this Node.
     * @fn obtain
     * @param Node A qualifying Node.
     */
    static const Rule* obtain (const Node&);
    /**
     * @brief Determines if this Node can be binded with a Node that falls under this rule.
     * @fn canBind
     * @param Node The source Node in question.
     * @param Node The destination Node in question.
     */
    const bool canBind (const Node& , const Node&) const;
    /**
     * @brief Creates a Link between a qualifying source Node and a destination Node.
     * @fn bind
     * @param Node The source Node in question.
     * @param Node The destination Node in question.
     */
    const Link* bind (const Node&, const Node&) const;
    /**
     * @brief Returns a string representing the type of nodes that this Rule looks for.
     * @fn type
     */
    const QString type() const;
    /**
     * @brief
     *
     * @fn locale
     */
    const QString locale() const;
    /**
     * @brief Determines if a Node is qualified to use this Rule.
     * @fn appliesFor
     * @param Node The source node in question.
     */
    const double appliesFor (const Node&) const;
    /**
     * @brief Returns a pointer to the Binding that works for the two specified Nodes.
     * @fn getBindingFor
     * @param Node The source Node in question.
     * @param Node The destination Node in question.
     */
    const Binding* getBindingFor (const Node&, const Node&) const;

private:
    QScopedPointer<RulePrivate> d_ptr;
};

}
}

Q_DECLARE_METATYPE (Wintermute::Linguistics::Rule)
Q_DECLARE_METATYPE (Wintermute::Linguistics::Binding)

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
