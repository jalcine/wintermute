/*
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

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 03/04/12 6:31:15 AM
 */

#include <data-api/dbus/interfaces.hpp>
#include <data-api/syntax/bond.hpp>
#include <data-api/config.hpp>

#include "syntax/node.hpp"
#include "binding.hpp"
#include "rule.hxx"
#include "rule.hpp"
#include <QDBusInterface>

using namespace std;
using namespace Wintermute::Data;
using namespace Wintermute::Data::Linguistics;
using namespace Wintermute::Linguistics;

Rule::Rule() : QObject(), d_ptr (new RulePrivate)
{

}
Rule::Rule (const Chain& p_chain, QObject* parent) : QObject (parent), d_ptr (new RulePrivate)
{
    Q_D (Rule);
    d->m_chn = p_chain;
    d->__init (this);
}

Rule::Rule (const Rule& p_other) : QObject (p_other.parent()), d_ptr (const_cast<RulePrivate*> (p_other.d_ptr.data()))
{
    Q_D (Rule);
    d->__init (this);
}

const Rule* Rule::obtain (const Node& p_nd)
{
    const QString lcl = p_nd.locale ();
    const QString flg = p_nd.flags ().begin ().value ().toString();
    SyntaxInterface* intf = new SyntaxInterface;
    Syntax::Chain chn (lcl, flg);
    QDBusPendingReply<QString> reply = intf->read(chn);
    reply.waitForFinished();
    if (!reply.isError()){
        qDebug() << "(ling) [Rule::obtain()] Obtained from D-Bus:" << reply;
    } else {
        qDebug() << "(ling) [Rule::obtain()] D-Bus error:" << reply.error().errorString(reply.error().type());
    }
    chn = Syntax::Chain::fromString (reply);
    return new Rule (chn);
}

const Link* Rule::bind (const Node& p_curNode, const Node& p_nextNode) const
{
    Q_D (const Rule);

    for (BindingList::const_iterator i = d->m_bndVtr.begin (); i != d->m_bndVtr.end (); i++) {
        const Binding* bnd = *i;

        if (bnd->canBind (p_curNode, p_nextNode))
            return bnd->bind (p_curNode, p_nextNode);
    }

    return 0;
}

const bool Rule::canBind (const Node& p_nd, const Node& p_dstNd) const
{
    Q_D (const Rule);

    for (BindingList::const_iterator i = d->m_bndVtr.begin (); i != d->m_bndVtr.end (); i++) {
        const Binding* bnd = *i;

        if (bnd->canBind (p_nd, p_dstNd))
            return true;
    }

    return false;
}

const Binding* Rule::getBindingFor (const Node& p_nd, const Node& p_nd2) const
{
    Q_D (const Rule);
    typedef QMap<double, Binding*> RatedBindingMap;

    RatedBindingMap bndLevel;

    for (BindingList::const_iterator i = d->m_bndVtr.begin (); i != d->m_bndVtr.end (); i++) {
        const Binding* bnd = *i;
        double vl = bnd->canBind (p_nd, p_nd2);

        if (vl) {
            bndLevel.insert (vl, const_cast<Binding*> (bnd));
            //qDebug() << "(ling) [Rule] Valid binding for" << p_nd.symbol () << "to" << p_nd2.symbol ();
        }
    }

    if (!bndLevel.empty ()) {
        const Binding* bnd = bndLevel.values().last();
        const double rate = bndLevel.keys().last();

        if (rate > 0.0) {
            qDebug() << "(ling) [Rule] Highest binding for" << p_nd.symbol() << "at" << rate * 100 << "%";
            return bnd;
        }
    }

    qDebug() << "(ling) [Rule] No bindings found for" << p_nd.toString (Node::EXTRA) << "to" << p_nd2.toString (Node::EXTRA);
    return 0;
}

const double Rule::appliesFor (const Node& p_nd) const
{
    const QString ndStr (p_nd.toString (Node::EXTRA));
    const QString rlStr (type());
    const double rtn = Syntax::Bond::matches (ndStr , rlStr);

    return rtn;
}

const QString Rule::type() const
{
    Q_D (const Rule);
    return d->m_chn.type ();
}

const QString Rule::locale () const
{
    Q_D (const Rule);
    return d->m_chn.locale();
}

Rule Rule::operator=(Rule p_other){
    this->d_ptr.swap(p_other.d_ptr);
    return *this;
}

Rule::~Rule()
{

}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
