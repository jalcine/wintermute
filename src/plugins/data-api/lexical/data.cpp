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
 */

#include <QDataStream>
#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <qjson/qobjecthelper.h>
#include "data.hpp"
#include "dataprivate.hpp"
#include "md5.hpp"

using namespace Wintermute::Data::Linguistics::Lexical;

const Data Data::Empty = Data();

DataPrivate::DataPrivate() : id (QString::null),
    locale (QString::null),
    symbol (QString::null),
    flags (QVariantMap())
{

}

DataPrivate::~DataPrivate() {
}

Data::Data() : QObject(), d_ptr (new DataPrivate)
{

}

Data::Data (const Data& p_other) : QObject (p_other.parent()),
    d_ptr (p_other.d_ptr.data())
{

}

Data::Data (const QString p_id, const QString p_locale,
            const QString p_symbol, const QVariantMap p_flags) : QObject(),
    d_ptr (new DataPrivate)
{
    Q_D (Data);
    d->id = p_id;
    d->locale = p_locale;
    d->symbol = p_symbol;
    d->flags = p_flags;
}

Data::Data (DataPrivate* dd) : QObject(), d_ptr (dd)
{

}

QVariantMap Data::flags () const
{
    Q_D (const Data);
    return d->flags;
}

QString Data::symbol () const
{
    Q_D (const Data);
    return d->symbol;
}

QString Data::id () const
{
    Q_D (const Data);
    return d->id;
}

QString Data::locale () const
{
    Q_D (const Data);
    return d->locale;
}

void Data::setSymbol (const QString& p_symbol)
{
    Q_D (Data);

    if (!p_symbol.isNull() && !p_symbol.isNull()) {
        d->symbol = p_symbol;
        d->id = Data::idFromString (d->symbol);
    }
}

void Data::setLocale (const QString& p_locale)
{
    Q_D (Data);

    if (!p_locale.isEmpty() && !p_locale.isNull())
        d->locale = p_locale;
}

void Data::setId (const QString& p_id)
{
    Q_D (Data);

    if (!p_id.isEmpty() && !p_id.isNull())
        d->id = p_id;
}

void Data::setFlags (const QVariantMap& p_flags)
{
    Q_D (Data);

    if (!p_flags.empty())
        d->flags = p_flags;
}

bool Data::operator== (const Data& p_other) const
{
    return ( (flags() == p_other.flags()) && (symbol() == p_other.symbol()) &&
             (locale() == p_other.locale()) && (id() == p_other.id()));
}

void Data::operator= (const Data& p_other)
{
    setFlags (p_other.flags());
    setLocale (p_other.locale());
    setSymbol (p_other.symbol());
    setId (p_other.id());
}

Data Data::clone() const
{
    return * (new Data (d_ptr.data()));
}

bool Data::isValid () const
{
    return Data::Empty.d_ptr != this->d_ptr ||
           ! (flags().isEmpty() && (symbol().isNull() && symbol().isEmpty()));
}

/// @todo Use MD-5 hashing from another library (QCA has it) so we can eliminate md5.*pp.
const QString Data::idFromString (const QString p_symbol)
{
    const QString id = QString::fromStdString (md5 (p_symbol.toLower ().toStdString ()));
    qDebug() << "(data) [Data::idFromString()]" << p_symbol << id;

    return id;
}

Data Data::fromString (const QString& p_string)
{
    Data dt = Data::Empty;

    if (!p_string.isEmpty() && !p_string.isNull()) {
        QJson::Parser* parser = new QJson::Parser;
        bool success = false;
        QVariantMap map = parser->parse (p_string.toStdString().c_str(), &success).toMap();

        qDebug() << "(data) [Data::fromString()] Result from string:" << map;

        if (success) {
            QJson::QObjectHelper::qvariant2qobject (map, &dt);
        }
    }

    return dt;
}

QString Data::toJson() const
{
    QJson::Serializer* serializer = new QJson::Serializer;
    QVariantMap map = QJson::QObjectHelper::qobject2qvariant (this);
    const QString str = QString (serializer->serialize (map));
    qDebug() << "(data) [Data::cast_operation]" << str;
    return str;
}

Data::~Data()
{
    d_ptr.reset();
}

#include "lexical/data.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
