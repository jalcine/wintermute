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

#ifndef WNTRLING_DBUS_NODEINTERFACE_HPP
#define WNTRLING_DBUS_NODEINTERFACE_HPP

// Qt includes
#include <QObject>
#include <QDBusPendingReply>
#include <QDBusAbstractInterface>

// local includes
#include "lexical/data.hpp"

namespace Wintermute {
    namespace Data {

        using Linguistics::Lexical::Data;

        class NodeInterface: public QDBusAbstractInterface {
            Q_OBJECT

        public:
            static inline const char *staticInterfaceName() {
                return "org.thesii.Wintermute.Data.Nodes";
            }
            NodeInterface();

            ~NodeInterface();

        public slots:
            inline QDBusPendingReply<bool> exists ( Data in0 ) {
                QList<QVariant> argumentList;
                argumentList << qVariantFromValue(in0);
                if ( in0.id() == "d41d8cd98f00b204e9800998ecf8427e" )
                    qFatal ( "WOAH BUDDY!" );
                return asyncCallWithArgumentList ( QLatin1String ( "exists" ), argumentList );
            }

            inline Q_NOREPLY void generate() {
                QList<QVariant> argumentList;
                callWithArgumentList ( QDBus::NoBlock, QLatin1String ( "generate" ), argumentList );
            }

            inline QDBusPendingReply<bool> isPseudo ( Data in0 ) {
                QList<QVariant> argumentList;
                argumentList << qVariantFromValue(in0);
                return asyncCallWithArgumentList ( QLatin1String ( "isPseudo" ), argumentList );
            }

            inline QDBusPendingReply<Data> pseudo ( Data in0 ) {
                QList<QVariant> argumentList;
                argumentList << qVariantFromValue ( in0 );
                return asyncCallWithArgumentList ( QLatin1String ( "pseudo" ), argumentList );
            }

            inline QDBusPendingReply<> quit() {
                QList<QVariant> argumentList;
                return asyncCallWithArgumentList ( QLatin1String ( "quit" ), argumentList );
            }

            inline QDBusPendingReply<Data> read ( Data in0 ) {
                QList<QVariant> argumentList;
                argumentList << qVariantFromValue ( in0 );
                return asyncCallWithArgumentList ( QLatin1String ( "read" ), argumentList );
            }

            inline QDBusPendingReply<Data> write ( Data in0 ) {
                QList<QVariant> argumentList;
                argumentList << qVariantFromValue ( in0 );
                return asyncCallWithArgumentList ( QLatin1String ( "write" ), argumentList );
            }

            Q_SIGNALS: // SIGNALS
    void nodeCreated ( const QString &in0 );
        };

    }
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;

#endif