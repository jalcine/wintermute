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

#ifndef WNTRDATA_SYSTEMADAPTOR
#define WNTRDATA_SYSTEMADPATOR

// Qt includes
#include <QObject>
#include <QDBusAbstractAdaptor>

namespace Wintermute
{
namespace Data
{
class SystemAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO ("D-Bus Interface", "org.thesii.Wintermute.Data.System")
    Q_CLASSINFO ("D-Bus Introspection", ""
                 "  <interface name=\"org.thesii.Wintermute.Data.System\">\n"
                 "    <property access=\"readwrite\" type=\"s\" name=\"Directory\"/>\n"
                 "    <signal name=\"stopped\"/>\n"
                 "    <signal name=\"started\"/>\n"
                 "    <method name=\"quit\"/>\n"
                 "    <method name=\"stop\">\n"
                 "      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
                 "    </method>\n"
                 "    <method name=\"start\">\n"
                 "      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
                 "    </method>\n"
                 "    <method name=\"localeExists\">\n"
                 "      <arg direction=\"out\" type=\"b\"/>\n"
                 "      <arg direction=\"in\" type=\"s\"/>\n"
                 "    </method>\n"
                 "  </interface>\n"
                 "")
public:
    SystemAdaptor();
    virtual ~SystemAdaptor();

public: // PROPERTIES
    Q_PROPERTY (QString Directory READ directory WRITE setDirectory)
    QString directory() const;
    void setDirectory (const QString& value);

public Q_SLOTS: // METHODS
    bool localeExists (const QString& in0);
    void quit();
    Q_NOREPLY void start();
    Q_NOREPLY void stop();
Q_SIGNALS: // SIGNALS
    void started();
    void stopped();
};

}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
