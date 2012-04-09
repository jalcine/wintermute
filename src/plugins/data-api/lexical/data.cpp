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

// local includes
#include "data.hpp"
#include "md5.hpp"

// Qt includes
#include <QDataStream>
#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <qjson/qobjecthelper.h>

using namespace Wintermute::Data::Linguistics::Lexical;

const Data Data::Empty = Data();

Data::Data() : QObject(), m_id(), m_locale(), m_symbol(), m_flags() { }

Data::Data (const Data& p_other) : QObject (p_other.parent()), m_id (p_other.m_id),
    m_locale (p_other.m_locale), m_symbol (p_other.m_symbol),
    m_flags (p_other.m_flags) { }

Data::Data (const QString p_id, const QString p_locale,
            const QString p_symbol, const QVariantMap p_flags) : QObject(),
    m_id (p_id), m_locale (p_locale),
    m_symbol (p_symbol), m_flags (p_flags) { }

const QVariantMap Data::flags () const
{
    return m_flags;
}

const QString Data::symbol () const
{
    return m_symbol;
}

const QString Data::id () const
{
    return m_id;
}

const QString Data::locale () const
{
    return m_locale;
}

void Data::setSymbol (const QString& p_symbol)
{
    if (!p_symbol.isNull() && !p_symbol.isNull()) {
        m_symbol = p_symbol;
        m_id = Data::idFromString (m_symbol);
    }
}

void Data::setLocale (const QString& p_locale)
{
    if (!p_locale.isEmpty() && !p_locale.isNull())
        m_locale = p_locale;
}

void Data::setID (const QString& p_id)
{
    if (!p_id.isEmpty() && !p_id.isNull())
        m_id = p_id;
}

void Data::setFlags (const QVariantMap& p_flags)
{
    if (!p_flags.empty())
        m_flags = p_flags;
}

bool Data::operator== (const Data& p_otherDt) const
{
    return ( (m_flags == p_otherDt.m_flags) && (m_symbol == p_otherDt.m_symbol) &&
             (m_locale == p_otherDt.m_locale) && (m_id == p_otherDt.m_id));
}

void Data::operator= (const Data& p_otherDt)
{
    m_flags = p_otherDt.m_flags;
    m_symbol = p_otherDt.m_symbol;
    m_locale = p_otherDt.m_locale;
    m_id = p_otherDt.m_id;
}

bool Data::isValid () const
{
    return Data::Empty == *this;
}

/// @todo Use MD-5 hashing from another library (QCA has it) so we can eliminate md5.*pp.
const QString Data::idFromString (const QString p_symbol)
{
    const QString id = QString::fromStdString (md5 (p_symbol.toLower ().toStdString ()));
    //qDebug() << "(data) [Data]" << p_sym << id;

    return id;
}

Data Data::fromString (const QString& p_string)
{
    Data dt;

    if (!p_string.isEmpty() && !p_string.isNull()) {
        QJson::Parser* parser = new QJson::Parser;
        QVariantMap map = parser->parse (p_string.toAscii()).toMap();
        QJson::QObjectHelper::qvariant2qobject (map, &dt);
    }
    else
        dt = Data::Empty;

    return dt;
}

Data::operator QString() const
{
    QJson::Serializer* serializer = new QJson::Serializer;
    QVariantMap map = QJson::QObjectHelper::qobject2qvariant (this);
    return QString (serializer->serialize (map));
}

Data::~Data () { }

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
