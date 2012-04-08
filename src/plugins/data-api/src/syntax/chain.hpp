/**
 * @file chain.hpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 * @date Sun, 04 Mar 2012 10:24:23 UTC
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
 *
 */

#ifndef WNTRDATA_SYNTAX_CHAIN_HPP
#define WNTRDATA_SYNTAX_CHAIN_HPP

#include <QMap>
#include <QString>
#include <QObject>
#include <QMetaType>
#include <QDBusArgument>

#include "bond.hpp"

namespace Wintermute
{
namespace Data
{
namespace Linguistics
{
namespace Syntax
{

class Chain;

/**
* @brief
*/
class Chain : public QObject
{
    Q_OBJECT
    Q_PROPERTY (BondList Bonds READ bonds WRITE setBonds)
    Q_PROPERTY (QString Type READ type WRITE setType)
    Q_PROPERTY (QString Locale READ locale)
    friend QDBusArgument& operator<< (QDBusArgument& p_arg, const Chain& p_chn) {
        p_arg.beginStructure();
        p_arg << p_chn.m_lcl << p_chn.m_typ;
        p_arg.beginArray (qMetaTypeId<Bond>());
        Q_FOREACH (const Bond * l_bnd, p_chn.m_bndVtr) {
            p_arg << *l_bnd;
        }
        p_arg.endArray();
        p_arg.endStructure();
        return p_arg;
    }

    friend const QDBusArgument& operator>> (const QDBusArgument& p_arg, Chain& p_chn) {
        p_arg.beginStructure();
        p_arg >> p_chn.m_lcl >> p_chn.m_typ;
        p_arg.beginArray();

        while (!p_arg.atEnd()) {
            Bond* l_bnd = new Bond;
            p_arg >> *l_bnd;
            p_chn.m_bndVtr << l_bnd;
        }

        p_arg.endArray();
        p_arg.endStructure();
        return p_arg;
    }

public:
    /**
     * @brief Null constructor.
     * @fn Chain
     */
    Chain();

    /**
     * @brief Copy constructor.
     * @fn Chain
     * @param p_chn The Chain to be copied.
     */
    Chain (const Chain&);

    /**
     * @brief Default construcotr.
     * @fn Chain
     * @param
     * @param
     */
    explicit Chain (const QString&, const QString& = QString::null, const BondList& = BondList());

    /**
     * @brief Assignment operator.
     * @fn operator =
     * @param p_chn The Chain to be copied.
     */
    void operator= (const Chain&);

    /**
     * @brief Deconstructor.
     * @fn ~Chain
     */
    virtual ~Chain();

    /**
     * @brief
     * @fn setBonds
     * @param
     */
    void setBonds (const BondList&);

    /**
     * @brief
     * @fn setType
     */
    void setType (const QString&);

    /**
     * @brief
     * @fn bonds
     */
    const BondList bonds() const;

    /**
     * @brief
     * @fn locale
     */
    const QString locale() const;

    /**
     * @brief
     * @fn type
     */
    const QString type() const;

    QString toString() const;

    static Chain fromString (const QString&);

    /**
     * @brief
     * @fn operator []
     * @param
     */
    Bond operator[] (const int&) const;
private:
    BondList m_bndVtr; /**< Holds the bonds */
    QString m_typ;
    QString m_lcl;
};


}
}
}
}

Q_DECLARE_METATYPE (Wintermute::Data::Linguistics::Syntax::Chain);
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;

#endif
