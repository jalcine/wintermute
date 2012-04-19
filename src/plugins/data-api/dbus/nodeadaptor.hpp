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

#ifndef WNTRDATA_NODEADAPTOR_HPP
#define WNTRDATA_NODEADAPTOR_HPP

#include <app/adaptors.hpp>

namespace Wintermute
{
namespace Data
{

class NodeAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO ("D-Bus Interface", "org.thesii.Wintermute.Data.Nodes")
    Q_CLASSINFO ("D-Bus Introspection", ""
                 "  <interface name=\"org.thesii.Wintermute.Data.Nodes\">\n"
                 "    <signal name=\"nodeCreated\">\n"
                 "      <arg direction=\"out\" type=\"s\"/>\n"
                 "    </signal>\n"
                 "    <method name=\"generate\">\n"
                 "      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
                 "    </method>\n"
                 "    <method name=\"quit\"/>\n"
                 "    <method name=\"read\">\n"
                 "      <arg direction=\"out\" type=\"s\"/>\n"
                 "      <arg direction=\"in\" type=\"s\"/>\n"
                 "    </method>\n"
                 "    <method name=\"write\">\n"
                 "      <arg direction=\"out\" type=\"s\"/>\n"
                 "      <arg direction=\"in\" type=\"s\"/>\n"
                 "    </method>\n"
                 "    <method name=\"pseudo\">\n"
                 "      <arg direction=\"out\" type=\"s\"/>\n"
                 "      <arg direction=\"in\" type=\"s\"/>\n"
                 "    </method>\n"
                 "    <method name=\"exists\">\n"
                 "      <arg direction=\"out\" type=\"b\"/>\n"
                 "      <arg direction=\"in\" type=\"s\"/>\n"
                 "    </method>\n"
                 "    <method name=\"isPseudo\">\n"
                 "      <arg direction=\"out\" type=\"b\"/>\n"
                 "      <arg direction=\"in\" type=\"s\"/>\n"
                 "    </method>\n"
                 "  </interface>\n"
                 "")
public:
    NodeAdaptor();
    virtual ~NodeAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    bool exists (QString in0);
    void generate();
    bool isPseudo (QString in0);
    QString pseudo (QString in0);
    void quit();
    QString read (QString in0);
    QString write (QString in0);
Q_SIGNALS: // SIGNALS
    void nodeCreated (const QString& in0);
};

}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
