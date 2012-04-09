/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 03/04/12 5:56:16 AM
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
#ifndef WNTRLING_BINDING_HPP
#define WNTRLING_BINDING_HPP

#include <QObject>

namespace Wintermute
{namespace Data
{
    namespace Linguistics
    {
        namespace Syntax
        {
            class Bond;
        }
    }
}
namespace Linguistics
{
using Wintermute::Data::Linguistics::Syntax::Bond;
class Rule;
class Node;
class Link;
class Binding;
/**
 * @brief Represents a QList of Binding objects.
 * @see Binding
 * @typedef BindingVector
 */
typedef QList<Binding*> BindingList;

/**
 * @brief Represents the potential connection of words by a specified rule as defined by its parent rule.
 *
 * Binding objects are used extenstively by Meaning objects in the formulation
 * process of parsing,
 *
 * @class Binding parser.hpp "src/parser.hpp"
 */
struct BindingPrivate;
class Binding : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE (Binding)
    friend class RulePrivate;
signals:
    /**
     * @brief
     * @fn binded
     * @param
     * @param
     * @param
     */
    void binded (const Binding* = 0, const Node* = 0, const Node* = 0) const;
    /**
     * @brief
     * @fn bindFailed
     * @param
     * @param
     * @param
     */
    void bindFailed (const Binding* = 0, const Node* = 0, const Node* = 0) const;

public:
    Q_DISABLE_COPY(Binding)
    explicit Binding ();
    /**
     * @brief
     * @fn ~Binding
     */
    virtual ~Binding();
    /**
     * @brief
     *
     * @fn obtain
     * @param
     * @param
     */
    static const Binding* obtain (const Node&, const Node&);
    /**
     * @brief
     * @fn parentRule
     */
    const Rule* parentRule() const;
    /**
     * @brief The ability of binding is measured on a scale from 0.0 to 1.0; where 0.0 is no chance at all and 1.0 is equality.
     * @fn canBind
     * @param
     * @param
     */
    const double canBind (const Node&, const Node&) const;
    /**
     * @brief
     * @fn getAttrValue
     * @param
     */
    const QString getAttrValue (const QString&) const;
    /**
     * @brief
     * @fn bind
     * @param
     * @param
     */
    const Link* bind (const Node&, const Node&) const;

protected:
    /**
     * @brief
     * @fn Binding
     * @param QDomElement
     * @param
     */
    Binding (const Bond& , const Rule*);

private:
    QScopedPointer<BindingPrivate> d_ptr;
};

}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
