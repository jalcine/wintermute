/**
 * \file ontology.hpp
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

#ifndef ONTOLOGY_HPP
#define	ONTOLOGY_HPP

// Qt
#include <QUrl>
#include <QString>
#include <QStringList>

// Soprano
#include <Soprano/Model>
#include <Soprano/Node>
#include <Soprano/Client/SparqlModel>

// Local
#include "query.hpp"

using namespace std;


namespace Wintermute {
namespace Data {
namespace Ontology {
struct System;
struct Resource;
struct Repository;

typedef QString Concept;

/**
 * \brief Configuration class for the ontology section.
 */
class System : public QObject {
    Q_OBJECT

private:
    QUrl systemOntology;

public:
    /**
     * \brief Return URL of the system ontology
     *
     * \return URL of the system ontology
     */
    static QUrl getSystemOntology();

    /**
     * \brief Initializes the ontology system.
     *
     * Runs all of the necessary initialization code to get the ontology system on its toes.
     */
    Q_INVOKABLE static void load();

    /**
     * \brief Deinitializes the ontology system.
     *
     * Runs all of the necessary deinitialization code to have the ontology system be safely shut down.
     */
    Q_INVOKABLE static void unload();
};

/**
 * \brief Represents an object being described.
 *
 * According to the definition of resources in the RDF syntax documentation
 * <http://www.w3.org/TR/REC-rdf-syntax> it's defined as,
 * <blockquote>
 * All things being described by RDF expressions are called <b>resources</b>.
 * A resource may be an entire Web page; such as the HTML document "http://www.w3.org/Overview.html"
 * for example. A resource may be a part of a Web page; e.g. a specific HTML or XML element within
 * the document source. A resource may also be a whole collection of pages; e.g. an entire Web site.
 * A resource may also be an object that is not directly accessible via the Web; e.g. a printed book.
 * Resources are always named by URIs plus optional anchor ids. Anything can have a URI; the extensibility
 * of URIs allows the introduction of identifiers for any entity imaginable.
 * </blockquote>
 *
 * This class provides a means of convience over RDF-XML parsing languages (mainly SPARQL and RDFQL) so that
 * properties of resources can be properly queried and modified.
 *
 * \class Resource ontology.hpp "src/ontology.hpp"
 * \todo Add 'operator==()', 'operator!=()', and 'operator=()' to this method for convience.
 */
class Resource : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( Resource )

public:
    /**
     * \brief Default constructor.
     *
     * \fn Resource
     * \param parent The parent object.
     */
    explicit Resource ( QObject* = 0 );

    /**
     * \brief Initializating constructor.
     *
     * Creates a new Resource from its parent Repository and
     * its corresponding Node within the ontology.
     *
     * \fn Resource
     * \param node The Soprano::Node that this Resource represents.
     * \param parent The parent repository.
     */
    Resource ( const Soprano::Node& , const Repository* , QObject* = 0 );

    /**
     * \brief Deconstructor.
     *
     * \fn ~Resource
     */
    virtual ~Resource();

    /**
     * \brief
     *
     * \fn countConcepts
     */
    static const int countConcepts();

private:
    const Repository* m_repo;
    Soprano::Node m_node;

};

/**
 * \brief Represents an entire ontological store.
 *
 * Repostiories represent the entire encompassing set of Resources
 * in their native format (that being RDF-XML, for now).
 *
 * \todo Obtain the specified Resource from the internal ontology when requested.
 * \todo Document methods.
 *
 * \class Repository ontology.hpp "src/ontology.hpp"
 */
class Repository : public QObject {
    Q_OBJECT
signals:
    void loaded() const;

public:
    explicit Repository ( const QObject* parent = 0 );
    Repository ( const QString& p_str );
    Repository ( const Repository& p_repo );
    virtual ~Repository();

    const QUrl url() const;

    /**
     * \brief Returns a pointer to the Resource object representing given Concept.
     *
     * \param concept concept to search for
     * \return pointer to the object representing the concept
     */
    Resource* obtainResource ( Concept const& concept ) const;
    static Resource* obtainResource ( const QString&, const QString& );

    static Repository* obtainRepository ( const QString& );
    static const int countOntologies();

private:
    static QMap<QString, Repository*> s_repos;
    void load ( const QString& = QString::null ) const;
    mutable QString m_repoName;
    mutable Soprano::Client::SparqlModel* m_model;
};

}
}
}

#endif	/* ONTOLOGY_HPP */

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
