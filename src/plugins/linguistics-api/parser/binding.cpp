/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 03/04/12 5:55:12 AM
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

#include <data-api/syntax/bond.hpp>

#include "rule.hpp"
#include "binding.hpp"
#include "binding.hxx"
#include "syntax/link.hpp"
#include "syntax/node.hpp"

using namespace Wintermute::Linguistics;

Binding::Binding () : d_ptr (new BindingPrivate)
{

}

Binding::Binding (const Binding& p_other) : QObject (p_other.parent()),
    d_ptr (const_cast<BindingPrivate*> (p_other.d_ptr.data()))
{

}

Binding::Binding (const Bond& p_bond, const Rule* p_rule) : QObject(),
    d_ptr (new BindingPrivate)
{
    Q_D (Binding);

    d->m_bnd = p_bond;
    d->m_rl = *p_rule;
}

const Rule* Binding::parentRule() const
{
    Q_D (const Binding);
    return &d->m_rl;
}

const Binding* Binding::obtain (const Node& p_nd, const Node& p_nd2)
{
    const Rule* rl = Rule::obtain (p_nd);

    if (!rl) return 0;

    return rl->getBindingFor (p_nd, p_nd2);
}

const QString Binding::getAttrValue (const QString& p_attr) const
{
    Q_D (const Binding);
    return d->m_bnd.attribute (p_attr);
}

/// @todo This method needs to match each Node to the Bond with more precision.
/// @todo See if you can break down this method and allow custom attributes & custom attribute handling.
const double Binding::canBind (const Node& p_ndSrc, const Node& p_ndDst) const
{
    Q_D (const Binding);

    if (this->parentRule ()->appliesFor (p_ndSrc) == 0.0)
        return 0.0;

    double rtn = 0.0;
    const QString wh = d->m_bnd.attribute ("with");
    const QString has = d->m_bnd.attribute ("has");
    const QString hasAll = d->m_bnd.attribute ("hasAll");
    const QString ndDestStr = p_ndDst.toString (Node::EXTRA);
    const QString ndSrcStr = p_ndSrc.toString (Node::EXTRA);

    const QStringList options = wh.split (",");

    foreach (const QString s, options) {
        rtn = Syntax::Bond::matches (ndDestStr , s);
        const QString whHas = s.at (0) + has;

        if (rtn > 0.0) {
            qDebug() << endl << "(ling) [Binding] Src:" << ndSrcStr << "; Dst:" << ndDestStr << "; via:" << s << "; lvl:" << rtn;

            if (!hasAll.isEmpty ()) {
                if (!ndDestStr.contains (hasAll)) {
                    rtn = 0.0;
                    qDebug() << "(ling) [Binding] Required full destination node type:" << hasAll << "in" << ndDestStr;
                }
                else {
                    qDebug() << "(ling) [Binding] Rating up by" << ( (double) hasAll.length () / (double) ndDestStr.length ()) * 100 << "% thanks to ==" << hasAll;
                    rtn += ( (double) hasAll.length () / (double) ndDestStr.length ());
                }
            }
            else if (whHas.size () > 1) {
                const double wRtn = Syntax::Bond::matches (ndDestStr, whHas);

                if (wRtn == 0.0) {
                    rtn = 0.0;
                    qDebug() << "(ling) [Binding] Required partial destination node type:" << whHas << "in" << ndDestStr;
                }
                else {
                    qDebug() << "(ling) [Binding] Rating up by" << (wRtn / (double) ndDestStr.length ()) * 100 << "% thanks to ~=" << whHas;
                    rtn += (wRtn / (double) ndDestStr.length ());
                }
            }

            if (d->m_bnd.hasAttribute ("typeHas")) {
                const QString bindType = ndSrcStr.at (0) + this->getAttrValue ("typeHas");
                const double matchVal = Syntax::Bond::matches (ndSrcStr, bindType);
                const double min = (1.0 / (double) bindType.length ());

                //qDebug() << min << matchVal << ndSrcStr << bindType;
                if (matchVal < min) {
                    rtn = 0.0;
                    qDebug() << "(ling) [Binding] Required partial source node type:" << bindType << "in" << ndSrcStr;
                }
            }

            if (rtn > 0.0)
                break;
            else continue;
        }
    }

    if (rtn > 0.0)
        qDebug() << "(ling) [Binding] Bond:" << rtn * 100 << "% for" << p_ndSrc.symbol () << "to" << p_ndDst.symbol () << "via" << (wh + has) << endl;
    else {
        //qDebug() << "(ling) [Binding] Binding failed for (src) -> (dst) :" << p_ndSrc.toString (Node::EXTRA).c_str () << " -> " << ndDestStr.toStdString ().c_str () << " via" << wh;
    }

    return rtn;
}

/// @todo Allow the attribute to be handle certain parts.
const Link* Binding::bind (const Node& p_nd1, const Node& p_nd2) const
{
    Q_D (const Binding);

    if (!canBind (p_nd1, p_nd2)) {
        emit bindFailed (const_cast<Binding*> (this), &p_nd1, &p_nd2);
        return 0;
    }

    QString type = this->parentRule ()->type ();
    QString lcl = this->parentRule ()->locale();
    Node* nd = const_cast<Node*> (&p_nd1), *nd2 = const_cast<Node*> (&p_nd2);

    if (d->m_bnd.hasAttribute ("linkAction")) {
        const QStringList options = d->m_bnd.attribute ("linkAction").split (",");

        if (options.contains ("reverse")) {
            type = p_nd2.toString (Node::MINIMAL).at (0);
            lcl = p_nd2.locale ();
            Node* tmp = nd;
            nd = nd2;
            nd2 = tmp;
        }

        if (options.contains ("othertype"))
            type = p_nd2.toString (Node::MINIMAL).at (0);
        else if (options.contains ("thistype"))
            type = p_nd1.toString (Node::MINIMAL).at (0);
    }

    emit binded (this, &p_nd1, &p_nd2);
    qDebug() << "(ling) [Binding] Link formed: " << p_nd1.toString (Node::EXTRA) << p_nd2.toString (Node::EXTRA);
    return Link::form (*&nd, *&nd2 , type , lcl);
}

Binding::~Binding()
{

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
