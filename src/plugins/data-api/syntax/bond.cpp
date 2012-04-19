/**
 * @file bond.cpp
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

#include <QDebug>

// QJson includes
#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <qjson/qobjecthelper.h>

// local includes
#include "bond.hpp"

using namespace Wintermute::Data::Linguistics::Syntax;

Bond::Bond() { }

Bond::Bond (const Bond& p_bnd) : QObject (p_bnd.parent()),
    m_props (p_bnd.m_props) { }

void Bond::setWith (QString& p_value)
{
    setAttribute ("with", p_value);
}

void Bond::setAttribute (const QString& p_attr, QString& p_val)
{
    m_props.insert (p_attr, p_val);
}

const QString Bond::attribute (const QString& p_attr) const
{
    return m_props.value (p_attr);
}

void Bond::setAttributes (const StringMap& p_props)
{
    m_props = p_props;
}

bool Bond::hasAttribute (const QString& p_attribute) const
{
    return m_props.contains (p_attribute);
}

const QString Bond::with() const
{
    return attribute ("with");
}

const StringMap Bond::attributes() const
{
    return m_props;
}

/// @note This might be the crowning jewel of the linking system.
double Bond::matches (const QString& p_regex, const QString& p_query)
{
    const QStringList regexList = p_regex.split (",");
    QList<double> rslts;

    foreach (const QString regex, regexList) {
        const double max = (double) regex.length ();
        double cnt = 0.0;

        if (p_query.at (0) == regex.at (0)) {
            cnt += 1.0;

            for (int i = 1; i < p_query.length (); i++) {
                QChar chr = p_query.at (i);

                if (p_regex.contains (chr, Qt::CaseSensitive))
                    cnt += 1.0;
            }
        }

        rslts.push_back ( (cnt / max));
    }

    qSort (rslts.begin (), rslts.end ());

    if (!rslts.isEmpty () && !p_regex.isEmpty ())
        return rslts.last ();
    else
        return 0.0;
}

void Bond::operator= (const Bond& p_bnd)
{
    m_props = p_bnd.m_props;
}

bool Bond::operator == (const Bond& p_bond) const
{
    return m_props == p_bond.m_props;
}

Bond* Bond::fromString (const QString& p_str)
{
    Bond* bnd = new Bond;
    QJson::Parser* parser = new QJson::Parser;
    QVariantMap map = parser->parse (p_str.toAscii()).toMap();
    QVariantMap::ConstIterator itr = map.constBegin(), end = map.constEnd();

    for (; itr != end; ++itr)
        bnd->m_props.insert (itr.key(), itr.value().toString());

    return bnd;
}

QString Bond::toString() const
{
    QJson::Serializer* serializer = new QJson::Serializer;
    QVariantMap map;
    StringMap::ConstIterator itr = m_props.constBegin(), end = m_props.constEnd();

    for (; itr != end; ++itr)
        map.insert (itr.key(), itr.value());

    QString str = serializer->serialize (map);
    qDebug() << "(data) [Bond::toString()]" << str;
    return str;
}

QDebug operator<< (QDebug p_dbg, const Bond& p_bnd)
{
    p_dbg << p_bnd.toString();
    return p_dbg;
}

Bond::~Bond () { }

#include "syntax/bond.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
