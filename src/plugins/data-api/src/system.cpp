/**
 * @file rules.hpp
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

// Qt includes
#include <QDBusMetaType>

// Wintermute includes
#include <core.hpp>

// local includes
#include "config.hpp"
#include "system.hpp"
#include "linguistics.hpp"
#include "ontology/ontology.hpp"
#include "lexical/data.hpp"
#include "syntax/bond.hpp"

using namespace Wintermute;
using namespace Wintermute::Data;
using Wintermute::Data::System;
using Wintermute::Data::Linguistics::Lexical::Data;
using Wintermute::Data::Linguistics::Syntax::Bond;
using Wintermute::Data::Linguistics::Syntax::Chain;

System* System::s_inst = 0;

System::System() : m_dir ( WNTRDATA_DATA_DIR ) {
    connect ( this,SIGNAL ( started() ),this,SLOT ( registerDataTypes() ) );
}

void System::registerDataTypes() {
    qDBusRegisterMetaType<QVariantMap>();
    qDBusRegisterMetaType<Wintermute::Data::Linguistics::Lexical::Data>();
    qDBusRegisterMetaType<Bond>();
    qDBusRegisterMetaType<Chain>();
}

void System::start ( ) {
    Linguistics::System::setLocale ( Core::arguments ()->value ( "locale" ).toString () );
    Linguistics::System::load ( System::directory() + QString ( "/" ) + QString ( WNTRDATA_LING_DIR ) );
    Ontology::System::load();
    emit s_inst->started();
}

void System::stop ( ) {
    Wintermute::Data::Ontology::System::unload();
    Wintermute::Data::Linguistics::System::unload();
    emit s_inst->stopped();
}

const QString System::directory () {
    return s_inst->m_dir;
}

void System::setDirectory ( const QString& p_dir ) {
    stop();
    s_inst->m_dir = p_dir;
    start();
}

System* System::instance () {
    if ( !s_inst ) s_inst = new System;
    return s_inst;
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
