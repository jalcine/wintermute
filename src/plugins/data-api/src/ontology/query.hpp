/**
 * \file query.hpp
 * \author Wintermute Developers <wintermute-devel\lists.launchpad.net>
 * \date March 29, 2011, 2:25 PM
 * \namespace Wintermute::Data::Ontology
 * \legalese
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

#ifndef QUERY_HPP
#define QUERY_HPP

// Qt
#include <QUrl>
#include <QString>
#include <QStringList>


namespace Wintermute {
namespace Data {
namespace Ontology {

/**
 * \brief SPARQL query container class
 *
 * This class contains methods for building SPARQL queries
 * which later can be extracted as a QString using \c getContents()
 * function.
 */
class Query : public QObject {
    Q_OBJECT

private:
    QString base;
    QStringList prefixes;
    QString variables;
    QStringList triples;

    QString contents;
    bool blockOpened;

public:
    explicit Query ( QObject *parent = 0 );

    /**
     * \brief Return string with a SPARQL query
     *
     * \return QString object with a SPARQL query
     */
    QString getContents();

    /**
     * \brief Set base to use for IRIs
     *
     * \param baseIRI QUrl with base's IRI
     */
    void setBase ( const QUrl& baseIRI );

    /**
     * \brief Add prefix to the query
     *
     * \param pxName name of the prefix
     * \param pxIRI IRI that the prefix substitutes
     */
    void addPrefix ( QString const& pxName, QUrl const& pxIRI );

    /**
     * \brief Add source to the query
     *
     * \param eURL URL of the ontology endpoint to use
     */
    void addEndpoint ( QUrl const& eURL );

    /**
     * \brief Add variables to the query
     *
     * \param form query form to use
     * \param vars list of variables to add
     */
    void addVariables ( QString form, QStringList const& vars );

    /**
     * \brief Add one triple to the query
     *
     * \param subject
     * \param predicate
     * \param object
     */
    void addTriple ( QString subject, QString predicate, QString object );

    /**
     * \brief Close the query
     *
     */
    void closeQuery();
};
}
}
} // namespaces
#endif // QUERY_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
