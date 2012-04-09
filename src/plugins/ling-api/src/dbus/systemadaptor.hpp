/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  Jacky Alciné <jackyalcine@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef WNTRLING_SYSTEMADAPTOR_HPP
#define WNTRLING_SYSTEMADAPTOR_HPP

// Qt includes
#include <QObject>
#include <QtDBus/QDBusAbstractAdaptor>

namespace Wintermute
{
namespace Linguistics
{
class SystemAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO ("D-Bus Interface", "org.thesii.Wintermute.Linguistics.System")
    Q_CLASSINFO ("D-Bus Introspection", ""
                 "  <interface name=\"org.thesii.Wintermute.Data.System\">\n"
                 "    <signal name=\"stopped\"/>\n"
                 "    <signal name=\"started\"/>\n"
                 "    <signal name=\"responseFormed\"/>\n"
                 "      <arg direction=\"in\" type=\"s\"/>\n"
                 "    </signal>\n"
                 "    <method name=\"quit\"/>\n"
                 "    <method name=\"stop\">\n"
                 "      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
                 "    </method>\n"
                 "    <method name=\"start\">\n"
                 "      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
                 "    </method>\n"
                 "    <method name=\"tellSystem\">\n"
                 "      <arg direction=\"in\" type=\"s\"/>\n"
                 "      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
                 "    </method>\n"
                 "  </interface>\n"
                 "")
public:
    SystemAdaptor();
    virtual ~SystemAdaptor();

public Q_SLOTS: // METHODS
    void quit();
    Q_NOREPLY void start();
    Q_NOREPLY void stop();
    Q_NOREPLY void tellSystem (const QString& in0);

Q_SIGNALS: // SIGNALS
    void started();
    void stopped();
    void responseFormed (const QString& in0);
};

}
}

#endif // SYSTEMADAPTOR_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
