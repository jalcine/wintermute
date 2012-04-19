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
 *
 */

/**
 * @file adaptors.hpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 * @date Sun, 30 Oct 2011 21:54:16
 */

#ifndef WNTRDATA_DBUS_SYNTAX_HPP
#define WNTRDATA_DBUS_SYNTAX_HPP

#include <QObject>
#include <QDebug>
#include <QDBusPendingReply>
#include <QDBusAbstractInterface>

#include <data-api/syntax/bond.hpp>
#include <data-api/syntax/chain.hpp>

namespace Wintermute
{
namespace Data
{

using namespace Wintermute::Data::Linguistics;

class SyntaxInterface: public QDBusAbstractInterface
{
    Q_OBJECT

public:
    static inline const char* staticInterfaceName() {
        return "org.thesii.Wintermute.Data.Syntax";
    }

    SyntaxInterface();
    ~SyntaxInterface();

public slots:
    inline QDBusPendingReply<void> quit();
    inline QDBusPendingReply<Syntax::Chain> read (Syntax::Chain in0);
    inline QDBusPendingReply<Syntax::Chain> write (Syntax::Chain in0);
    inline QDBusPendingReply<bool> exists (const QString& in0, const QString& in1);

signals:
    void ruleCreated (const QString& in0);
};

}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;

#endif
