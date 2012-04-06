/**
 * @file bond.hpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 * @date Sun, 30 Oct 2011 21:54:16
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

#ifndef WNTRDATA_SYNTAX_BOND_HPP
#define WNTRDATA_SYNTAX_BOND_HPP

// Qt includes
#include <QMap>
#include <QString>
#include <QObject>
#include <QMetaType>
#include <QDBusArgument>


namespace Wintermute {
namespace Data {
namespace Linguistics {
namespace Syntax {

class Bond;
class Chain;

/**
 * @brief Represents a key-value list of strings.
 * @typedef StringMap
 */
typedef QMap<QString, QString> StringMap;

/**
 * @brief Represents a list of Bonds.
 * @typedef BondVector
 */
typedef QList<Bond*> BondList;

/**
 * @brief Represents the syntactical data and rules needed to form a syntactic link.
 *
 * Bonds are used by the Parser to determine whether or not a word (of a specified parent rule)
 * can efficiently and properly bond with other words. Specificially, the Binding object uses
 * the Bond object to form specialized links:
 *
 * @code
 * const Link* l_lnk = Binding::obtain(p_nd, p_nd2);
 * @endcode
 *
 * @see Binding
 * @see Cache
 * @class Bond models.hpp "src/models.hpp"
 */
class Bond : public QObject {
    Q_OBJECT
    Q_PROPERTY ( QString With READ with WRITE setWith )
    Q_PROPERTY ( StringMap Attributes READ attributes WRITE setAttributes )
    friend QDebug operator<< ( QDebug , const Bond& );
    friend QDBusArgument& operator<< ( QDBusArgument& p_arg, const Bond& p_bnd){
        p_arg.beginStructure();
        p_arg << p_bnd.m_props;
        p_arg.endStructure();
        return p_arg;
    }
    friend const QDBusArgument& operator>> ( const QDBusArgument& p_arg, Bond& p_bnd){
        p_arg.beginStructure();
        p_arg >> p_bnd.m_props;
        p_arg.endStructure();
        return p_arg;

    }

public:
    /**
     * @brief Null constructor.
     * @fn Bond
     */
    explicit Bond();

    /**
     * @brief Copy constructor.
     * @fn Bond
     * @param Bond The source Bond to be copied.
     */
    Bond ( const Bond& );

    /**
     * @brief Assignment operator.
     * @fn operator =
     * @param Bond The source Bond to be copied.
     */
    void operator= ( const Bond& );

    /**
     * @brief Equality operator.
     * @fn operator ==
     * @param Bond The source Bond to be compared.
     */
    const bool operator== ( const Bond& ) const;
    /**
     * @brief Destructor.
     * @fn ~Bond
     */
    virtual ~Bond();
    /**
     * @brief
     * @fn with
     */
    const QString with() const;
    /**
     * @brief
     * @fn attribute
     * @param
     */
    const QString attribute ( const QString& ) const;
    /**
     * @brief
     * @fn attributes
     */
    const StringMap attributes() const;
    /**
     * @brief
     * @fn hasAttribute
     * @param
     */
    const bool hasAttribute ( const QString& ) const;
    /**
     * @brief
     * @fn setWith
     * @param
     */
    void setWith ( QString& );
    /**
     * @brief
     * @fn setAttribute
     * @param
     * @param
     */
    void setAttribute ( const QString& , QString& );
    /**
     * @brief
     * @fn setAttributes
     * @param
     */
    void setAttributes ( const StringMap& );
    /**
     * @brief
     * @fn matches
     * @param
     * @param
     */
    static const double matches ( const QString& , const QString& );

    QString toString() const;

    static Bond* fromString ( const QString& );

private:
    StringMap m_props; /**< Holds all of the attributes. */
};


}
}
}
}

Q_DECLARE_METATYPE ( Wintermute::Data::Linguistics::Syntax::Bond );

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
