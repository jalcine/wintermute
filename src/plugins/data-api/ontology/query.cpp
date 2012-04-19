/**
 * \file query.cpp
 * \author Wintermute Developers <wintermute-devel\lists.launchpad.net>
 * \date March 29, 2011, 2:25 PM
 * \namespace Wintermute::Data::Ontology
 */
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
 * \endlegalese
 *
 */

#include "query.hpp"
#include <QStringList>

namespace Wintermute
{
namespace Data
{
namespace Ontology
{

Query::Query (QObject* parent) :
    QObject (parent), blockOpened (false), base (QString::null)
{
}

QString Query::getContents()
{
    return base + contents;
}

void Query::setBase (const QUrl& baseIRI)
{
    base = baseIRI.toString();
}

void Query::addPrefix (const QString& pxName, const QUrl& pxIRI)
{
    contents += "PREFIX " + pxName + ": <" + pxIRI.toString() + ">\n";
}

void Query::addEndpoint (const QUrl& eURL)
{
    contents += "FROM <" + eURL.toString() + ">\n";
}

void Query::addVariables (QString form, const QStringList& vars)
{
    contents += form + QString (" ");
    foreach (QString var, vars) {
        contents += "?" + var + " ";
    }
    contents += "\n";
}

void Query::addTriple (QString subject, QString predicate, QString object)
{
    if (!blockOpened) {
        contents += "WHERE {\n";
        blockOpened = true;
    }

    contents += subject + " ";
    contents += predicate + " ";
    contents += object + " .\n";
}

void Query::closeQuery()
{
    if (blockOpened) {
        contents += "}\n";
    }
}

}
}
} // namespaces

#include "ontology/query.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
