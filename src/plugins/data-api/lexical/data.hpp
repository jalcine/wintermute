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
 *
 */

/**
 * @file data.hpp
 * @author Jacky Alcine <jacky.alcine@thesii.org>
 * @date Sun, 30 Oct 2011 21:54:16
 *
 */

#ifndef WNTRDATA_LEXICAL_DATA_HPP
#define WNTRDATA_LEXICAL_DATA_HPP

#include <QDebug>
#include <QMap>
#include <QObject>
#include <QVariant>
#include <QDBusArgument>

namespace Wintermute
{
namespace Data
{
namespace Linguistics
{
namespace Lexical
{
struct DataPrivate;
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
    Q_GADGET
    Q_DECLARE_PRIVATE (Data)
    Q_PROPERTY (QString ID READ id WRITE setId)              /// Represents the ID of the Data.
    Q_PROPERTY (QString Locale READ locale WRITE setLocale)  /// Represents the locale of the Data.
    Q_PROPERTY (QString Symbol READ symbol WRITE setSymbol)  /// Represents the symbol of the Data.
    Q_PROPERTY (QVariantMap Flags READ flags WRITE setFlags) /// Represents the flags of the Data.

    friend const QDBusArgument& operator>> (const QDBusArgument& argument, Lexical::Data& structure) {
        QString id, locale, symbol;
        QVariantMap flags;
        argument.beginStructure();
        argument >> id >> locale >> symbol;
        argument >> flags;
        argument.endStructure();

        structure.setFlags (flags);
        structure.setLocale (locale);
        structure.setSymbol (symbol);
        structure.setId (id);
        return argument;
    }

    friend QDBusArgument& operator<< (QDBusArgument& argument, const Lexical::Data& structure) {
        argument.beginStructure();
        argument << structure.id() << structure.locale() << structure.symbol();
        argument << structure.flags();
        argument.endStructure();
        return argument;
    }

    Data (DataPrivate* dd);

public:
    /**
     * @brief Default constructor.
     * @param p_id     The ID of the Data.
     * @param p_locale The locale of the Data.
     * @param p_symbol The symbol of the Data.
     * @param p_flags  The flags of the Data.
     */
    Data (const QString     p_id,
          const QString     p_locale,
          const QString     p_symbol = QString::null,
          const QVariantMap p_flags  = QVariantMap());

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
     * @brief Destructor.
     **/
    virtual ~Data();

    /**
     * @brief Returns the ID of the node.
     * @fn id
     */
    QString id() const;

    /**
     * @brief Returns the locale of the Data.
     * @fn locale
     */
    QString locale() const;

    /**
     * @brief Returns the symbol of the Data.
     * @fn symbol
     */
    QString symbol() const;

    /**
     * @brief Returns the flags of the Data.
     * @fn flags
     */
    QVariantMap flags() const;

    /**
     * @brief Returns a clone of this object.
     * @fn clone
     * @return A object with the same exact data as this Data.
     **/
    Data clone() const;

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

    void setLocale (const QString& p_locale);

    void setId (const QString& p_id);

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

    static Data fromString (const QString& p_string);

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
    void operator= (const Data& p_other);

    QString toJson() const;

private:
    QScopedPointer<DataPrivate> d_ptr;
};

}
}
}
}

Q_DECLARE_METATYPE (Wintermute::Data::Linguistics::Lexical::Data)

#endif /* WNTRDATA_LEXICAL_DATA_HPP */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;

