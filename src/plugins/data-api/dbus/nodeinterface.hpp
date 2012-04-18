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
#include <data-api/lexical/data.hpp>

namespace Wintermute
{
namespace Data
{

using Linguistics::Lexical::Data;

class NodeInterface: public QDBusAbstractInterface
{
    Q_OBJECT

public:
    static const char* staticInterfaceName();
    NodeInterface();
    virtual ~NodeInterface();

public slots:
    Q_NOREPLY void generate();
    QDBusPendingReply<void> quit();
    QDBusPendingReply<bool> exists (Data in0);
    QDBusPendingReply<bool> isPseudo (Data in0);
    QDBusPendingReply<Data> pseudo (Data in0);
    QDBusPendingReply<Data> read (Data in0);
    QDBusPendingReply<Data> write (Data in0);

Q_SIGNALS: // SIGNALS
    void nodeCreated (const QString& in0);
};

}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;

#endif
