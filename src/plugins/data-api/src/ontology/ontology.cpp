/**
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
 * @file ontology.cpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 * @date March 29, 2011, 2:25 PM
 * @todo Determine a means of using Soprano to interpret RDF/XML (OWL) files into semantic information.
 * @todo Determine how and where UUIDs will be obtained. (Boost::UUID anyone? :])
 */

// Boost includes
#include <boost/progress.hpp>

// Qt includes
#include <QDebug>

// Soprano includes
#include <Soprano/Soprano>

// local includes
#include "config.hpp"
#include "ontology.hpp"
#include "query.hpp"
#include "wntrdata.hpp"

using namespace Soprano;
using namespace Wintermute::Data::Ontology;

QMap<QString, Repository*> Repository::s_repos;

QUrl System::getSystemOntology() {
    return QUrl ( QString ( WNTRDATA_ONTO_DIR ) +QString ( "/COSMO.owl" ) );
}

void System::load() {
    const Repository* repo = Repository::obtainRepository ( "COSMO" );
    qDebug() << "(data) [System] # ontology # Loaded.";
}

void System::unload() {
    qDebug() << "(data) [System] # ontology # Unloaded.";
}

Resource::Resource ( const Soprano::Node &node, const Repository* repo, QObject* parent ) : QObject ( parent ),
    m_node ( node ), m_repo ( repo ) {
}

Resource::Resource ( const Resource &resource ) : QObject ( resource.parent() ), m_node ( resource.m_node ), m_repo ( resource.m_repo ) {
}

Resource::Resource ( QObject* parent ) : QObject ( parent ) {

}

const int Resource::countConcepts () {
    return 0;
}

Resource::~Resource() {

}

Repository::Repository ( const QString &p_str ) : m_repoName ( p_str ) {
    load();
}

Repository::Repository ( const Repository &p_repo ) : m_repoName ( p_repo.m_repoName ), m_model ( p_repo.m_model ) { }

Repository* Repository::obtainRepository ( const QString& p_repoName ) {
    Repository* repo = 0;

    if ( Repository::s_repos.count ( p_repoName ) == 0 ) {
        repo = new Repository ( p_repoName );
        Repository::s_repos.insert ( p_repoName,repo );
    } else
        repo = Repository::s_repos.value ( p_repoName );

    return repo;
}

const QUrl Repository::url() const {
    return QUrl::fromLocalFile ( Linguistics::System::directory () + QString ( "/" )
                                 + QString ( WNTRDATA_ONTO_DIR ) + QString ( "/" ) + m_repoName + QString ( ".owl" ) );
}

const int Repository::countOntologies () {
    return 0;
}

void Repository::load ( const QString& p_repoName ) const {
    qDebug() << "(data) [Repository] Loading ontology" << m_repoName << "...";
    if ( !p_repoName.isEmpty () )
        m_repoName = p_repoName;
}

Resource* Repository::obtainResource ( const Concept& concept ) const {
    Query query;
    query.setBase ( this->url() );
    query.addPrefix ( "rdf", QUrl ( "http://www.w3.org/1999/02/22-rdf-syntax-ns#" ) );
    query.addPrefix ( "owl", QUrl ( "http://www.w3.org/2002/07/owl#" ) );

    QStringList variables;
    variables.append ( "x" );
    query.addVariables ( "SELECT", variables );

    query.addTriple ( "?x", "owl:Class", "?res" );
    query.addTriple ( "?res", "rdf:ID", concept );

    QueryResultIterator it = m_model->executeQuery ( query.getContents() );
    QList<BindingSet> bindingSets = it.allBindings();

    foreach ( const BindingSet bndSt, bindingSets )
    qDebug() << bndSt.bindingNames();

    return new Resource ( bindingSets[0]["x"], this );
}

Resource* Repository::obtainResource ( const QString& p_repository, const QString& p_res ) {
    Repository* repo = Repository::obtainRepository ( p_repository );
    if ( repo == 0 ) return 0;
    else return repo->obtainResource ( p_res );
}

Repository::~Repository() {
    qDebug() << "Destroyed repository" << m_repoName << ".";
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
