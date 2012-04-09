/**
 * @file data.hpp
 * @author Jacky Alcine <jacky.alcine@thesii.org>
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

#ifndef WNTRDATA_LEXICAL_DATA_HPP
#define WNTRDATA_LEXICAL_DATA_HPP

#include <QDebug>
#include <QMap>
#include <QObject>
#include <QVariant>
#include <QMetaType>
#include <QDBusArgument>

namespace Wintermute
{
namespace Data
{
namespace Linguistics
{
namespace Lexical
{

/**
 * @brief The lexical POD (plain ol' data) format of linguistics parsing.
 *
 * The Data object represents the internal workings of the lexical
 * bindings. Data objects hold information about the locale of the
 * potential parent node, the ID of the node, the symbol and the
 * flags of the node. All of this information can be used to assist
 * interoperability in moving it from a literal symbol ("you") to
 * its internal, workable type ("Aeon1#~" [en]).
 *
 * @note This class can be considered this a POD (<b>p</b>lain <b>o</b>l' <b>data format) of Wintermute.
 * @class Data models.hpp "models.hpp"
 * @see QVariantMap
 */
class Data : public QObject
{
    Q_OBJECT
    Q_PROPERTY (QString ID READ id WRITE setID)
    friend const QDBusArgument& operator>> (const QDBusArgument& argument, Wintermute::Data::Linguistics::Lexical::Data& structure) {
        argument.beginStructure();
        argument >> structure.m_id >> structure.m_locale >> structure.m_symbol;
        argument >> structure.m_flags;
        argument.endStructure();
        return argument;
    }

    friend QDBusArgument& operator<< (QDBusArgument& argument, const Data& structure) {
        argument.beginStructure();
        argument << structure.m_id << structure.m_locale << structure.m_locale << structure.m_flags;
        argument.endStructure();
        return argument;
    }

public:
    /**
     * @brief Default constructor.
     * @fn Data
     * @param p_id The ID of the Data.
     * @param p_locale The locale of the Data.
     * @param p_symbol The symbol of the Data.
     * @param p_flags The flags of the Data.
     */
    Data (const QString p_id , const QString p_locale , const QString p_symbol = QString::null , const QVariantMap p_flags = QVariantMap());

    /**
     * @brief Null constructor.
     * @fn Data
     */
    explicit Data();

    /**
     * @brief Copy constructor.
     * @fn Data
     * @param Data The Data to be copied.
     */
    Data (const Data& p_other);

    /**
     * @brief Deconstructor.
     * @fn ~Data
     */
    virtual ~Data();

    static Data fromString (const QString& p_string);

    /**
     * @brief Returns the ID of the node.
     * @fn id
     */
    const QString id() const;

    /**
     * @brief Returns the locale of the Data.
     * @fn locale
     */
    const QString locale() const;

    /**
     * @brief Returns the symbol of the Data.
     * @fn symbol
     */
    const QString symbol() const;

    /**
     * @brief Returns the flags of the Data.
     * @fn flags
     */
    const QVariantMap flags() const;

    /**
     * @brief Changes the symbol of the Data to p_dt.
     * @fn setSymbol
     * @param p_dt The symbol for the Data to hold now.
     * @note When the symbol is changed, the ID is changed as well. This
     *       is why there's no setID() method. The ID value is actually a
     *       MD5 hash of a lower-case representation of the symbol string.
     * @see idFromString(const QString)
     */
    void setSymbol (const QString& p_symbol);

    /**
     * @brief Sets the flags of the Data.
     * @fn setFlags
     * @param p_flags The flags for the Data to hold now.
     */
    void setFlags (const QVariantMap& p_flags);

    /**
     * @brief ...
     *
     * @param p_locale ...
     * @return void
     **/
    void setLocale (const QString& p_locale);
    /**
     * @brief ...
     *
     * @param p_id ...
     * @return void
     **/

    void setID (const QString& p_id);

    /**
     * @brief Determines if this Data is equivalent to a null Data object.
     * @fn isNull
     */
    bool isValid() const;

    /**
     * @brief Obtains the ID from a said QString.
     * @fn idFromString
     * @param QString The text to be transformed into its proper ID.
     */
    static const QString idFromString (const QString);

    static const Data Empty; /**< Represents an empty set of data. */
    /**
     * @brief Equality operator.
     * @fn operator==
     * @param The Data to be equated against.
     */
    bool operator== (const Data&) const;

    /**
     * @brief Assignment operator.
     * @fn operator=
     * @param The Data to be copied.
     */
    void operator= (const Data&);

    operator QString() const;

private:
    QString m_id;
    QString m_locale;
    QString m_symbol;
    QVariantMap m_flags;
};

}
}
}
}

Q_DECLARE_TYPEINFO (Wintermute::Data::Linguistics::Lexical::Data, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE (Wintermute::Data::Linguistics::Lexical::Data)

#endif /* WNTRDATA_LEXICAL_DATA_HPP */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
