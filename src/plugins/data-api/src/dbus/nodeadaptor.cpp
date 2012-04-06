/**
 * @file adaptors.hpp
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

// local includes
#include "config.hpp"
#include "linguistics.hpp"
#include "nodeadaptor.hpp"
#include "lexical/manager.hpp"

using namespace Wintermute::Data::Linguistics;

using Wintermute::Data::NodeAdaptor;
using Wintermute::Data::Linguistics::Lexical::Manager;

NodeAdaptor::NodeAdaptor()
    : QDBusAbstractAdaptor ( Manager::instance() ) {
    // constructor
    setAutoRelaySignals ( true );
}

NodeAdaptor::~NodeAdaptor() { }

bool NodeAdaptor::exists ( QString in0 ) {
    return Manager::instance()->exists ( Lexical::Data::fromString ( in0 ) );
}

void NodeAdaptor::generate() {
    QMetaObject::invokeMethod ( parent(), "generate" );
}

bool NodeAdaptor::isPseudo ( QString in0 ) {
    bool out0;
    QMetaObject::invokeMethod ( parent(), "isPseudo", Q_RETURN_ARG ( bool, out0 ), Q_ARG ( Lexical::Data , Lexical::Data::fromString ( in0 ) ) );
    return out0;
}

QString NodeAdaptor::pseudo ( QString in0 ) {
    Lexical::Data  dt = Lexical::Data::fromString ( in0 );
    return Manager::instance()->pseudo ( dt );
}

void NodeAdaptor::quit() {
    QMetaObject::invokeMethod ( parent(), "quit" );
}

QString NodeAdaptor::read ( QString in0 ) {
    Lexical::Data  out0;
    QMetaObject::invokeMethod ( parent(), "read", Q_RETURN_ARG ( Lexical::Data , out0 ), Q_ARG ( Lexical::Data , Lexical::Data::fromString ( in0 ) ) );
    return out0;
}

QString NodeAdaptor::write ( QString in0 ) {
    Lexical::Data  out0;
    QMetaObject::invokeMethod ( parent(), "write", Q_RETURN_ARG ( Lexical::Data , out0 ), Q_ARG ( Lexical::Data , Lexical::Data::fromString ( in0 ) ) );
    return out0;
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
