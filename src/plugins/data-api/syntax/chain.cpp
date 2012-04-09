/**
 * @file chain.cpp
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
#include "chain.hpp"

using namespace Wintermute::Data::Linguistics::Syntax;


Chain::Chain() { }

Chain::Chain (const QString& p_lcl, const QString& p_typ, const BondList& p_vtr) : m_bndVtr (p_vtr),
    m_typ (p_typ), m_lcl (p_lcl) { }

Chain::Chain (const Chain& p_chn) : m_bndVtr (p_chn.m_bndVtr), m_lcl (p_chn.m_lcl), m_typ (p_chn.m_typ) { }

void Chain::operator= (const Chain& p_chn)
{
    m_bndVtr = p_chn.m_bndVtr;
    m_typ = p_chn.m_typ;
    m_lcl = p_chn.m_lcl;
}

const BondList Chain::bonds () const
{
    return m_bndVtr;
}

const QString Chain::type () const
{
    return m_typ;
}

const QString Chain::locale() const
{
    return m_lcl;
}

void Chain::setType (const QString& p_typ)
{
    m_typ = p_typ;
}

void Chain::setBonds (const BondList& p_bnd)
{
    m_bndVtr = p_bnd;
}

QString Chain::toString() const
{
    QJson::Serializer* serializer = new QJson::Serializer;
    QVariantMap map;
    QVariantList bndLst;

    foreach (const Bond * bnd, m_bndVtr)
    bndLst << qVariantFromValue (bnd->toString());

    map["Type"] = m_typ;
    map["Bonds"] = bndLst;
    map["Locale"] = m_lcl;
    return QString (serializer->serialize (map));
}

Chain Chain::fromString (const QString& p_str)
{
    QJson::Parser* parser = new QJson::Parser;
    QVariantMap map = parser->parse (p_str.toAscii()).toMap();
    Chain chn;
    chn.m_lcl = map["Locale"].toString();
    chn.m_typ = map["Type"].toString();
    QStringList bndLst = map["Bonds"].toStringList();
    bndLst.removeAll ("{  }");
    QStringList::ConstIterator itr = bndLst.constBegin(), end = bndLst.constEnd();

    for (; itr != end; ++itr) {
        const QString str = *itr;
        chn.m_bndVtr << Bond::fromString (str);
    }

    return chn;
}

Chain::~Chain () { }
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
