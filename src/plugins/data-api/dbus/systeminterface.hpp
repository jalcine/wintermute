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

#ifndef WNTRDATA_DBUS_SYSTEM_HPP
#define WNTRDATA_DBUS_SYSTEM_HPP

// Qt includes
#include <QObject>
#include <QDBusPendingReply>
#include <QDBusAbstractInterface>

namespace Wintermute
{
namespace Data
{

class SystemInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char* staticInterfaceName() {
        return "org.thesii.Wintermute.Data.System";
    }

    SystemInterface();
    ~SystemInterface();

    Q_PROPERTY (QString Directory READ directory WRITE setDirectory)

    inline QString directory() const {
        return qvariant_cast< QString > (property ("Directory"));
    }
    inline void setDirectory (const QString& value) {
        setProperty ("Directory", qVariantFromValue (value));
    }

public slots:
    inline QDBusPendingReply<bool> localeExists (const QString& in0) {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue (in0);
        return asyncCallWithArgumentList (QLatin1String ("localeExists"), argumentList);
    }

    inline QDBusPendingReply<> quit() {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList (QLatin1String ("quit"), argumentList);
    }

    inline Q_NOREPLY void start() {
        QList<QVariant> argumentList;
        callWithArgumentList (QDBus::NoBlock, QLatin1String ("start"), argumentList);
    }

    inline Q_NOREPLY void stop() {
        QList<QVariant> argumentList;
        callWithArgumentList (QDBus::NoBlock, QLatin1String ("stop"), argumentList);
    }

signals:
    void started();
    void stopped();
};

}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 

#endif
