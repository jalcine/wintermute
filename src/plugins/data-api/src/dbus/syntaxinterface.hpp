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

#ifndef WNTRDATA_DBUS_SYNTAX_HPP
#define WNTRDATA_DBUS_SYNTAX_HPP

// Qt includes
#include <QObject>
#include <QDebug>
#include <QDBusPendingReply>
#include <QDBusAbstractInterface>

// local includes
#include "syntax/bond.hpp"
#include "syntax/chain.hpp"

namespace Wintermute {
namespace Data {

using namespace Wintermute::Data::Linguistics;

class SyntaxInterface: public QDBusAbstractInterface {
    Q_OBJECT

public:
    static inline const char *staticInterfaceName() {
        return "org.thesii.Wintermute.Data.Rules";
    }

    SyntaxInterface();
    ~SyntaxInterface();

public slots:
    inline QDBusPendingReply<bool> exists ( const QString &in0, const QString &in1 ) {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue ( in0 ) << qVariantFromValue ( in1 );
        return asyncCallWithArgumentList ( QLatin1String ( "exists" ), argumentList );
    }

    inline QDBusPendingReply<> quit() {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList ( QLatin1String ( "quit" ), argumentList );
    }

    inline QDBusPendingReply<Syntax::Chain> read ( Syntax::Chain in0 ) {
        QList<QVariant> argumentList;
        qDebug() << in0.toString();
        argumentList << in0.toString();
        return asyncCallWithArgumentList ( QLatin1String ( "read" ), argumentList );
    }

    inline QDBusPendingReply<Syntax::Chain> write ( Syntax::Chain in0 ) {
        QList<QVariant> argumentList;
        argumentList << in0.toString();
        return asyncCallWithArgumentList ( QLatin1String ( "write" ), argumentList );
    }

signals:
    void ruleCreated ( const QString &in0 );
};

}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;

#endif