/**
 * @file    adaptors.hpp
 * @author  Wintermute Development <wntr-devel@thesii.org>
 *
 *
 *
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute Linguistics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute Linguistics; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef ADAPTORS_HPP
#define ADAPTORS_HPP

#include <wintermute/adaptors.hpp>
#include <QtDBus/QDBusMessage>

using Wintermute::AbstractAdaptor;

namespace Wintermute {
namespace Network {
struct Message;
struct BroadcastAdaptor;
struct SystemAdaptor;

class BroadcastAdaptor : public AbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO ( "D-Bus Interface","org.thesii.Wintermute.Network.Broadcast" )

public:
    explicit BroadcastAdaptor();

signals:
    void started();
    void stopped();

public slots:
    virtual void quit() const;
    const bool isBroadcasting() const;
    void sendBroadcast();
    void stopBroadcasting();
    void startBroadcasting();
};

class SystemAdaptor : public AbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO ( "D-Bus Interface","org.thesii.Wintermute.Network.System" )
    Q_CLASSINFO ( "D-Bus Introspection", ""
                  "  <interface name=\"org.thesii.Wintermute.Network.System\">\n"
                  "    <signal name=\"messageRecieved\">\n"
                  "      <arg direction=\"in\" type=\"Wintermute::Network::Message\"/>\n"
                  "      <annotation value=\"Wintermute::Data::Linguistics::Rules::Chain\" name=\"com.trolltech.QtDBus.QtTypeName.In0\"/>\n"
                  "    </signal>\n"
                  "    <method name=\"isActive\" />\n"
                  "      <arg direction=\"out\" type=\"Wintermute::Data::Linguistics::Rules::Chain\"/>\n"
                  "      <arg direction=\"in\" type=\"Wintermute::Data::Linguistics::Rules::Chain\"/>\n"
                  "      <annotation value=\"Wintermute::Data::Linguistics::Rules::Chain\" name=\"com.trolltech.QtDBus.QtTypeName.In0\"/>\n"
                  "      <annotation value=\"Wintermute::Data::Linguistics::Rules::Chain\" name=\"com.trolltech.QtDBus.QtTypeName.Out0\"/>\n"
                  "    </method>\n"
                  "    <method name=\"sendMessage\">\n"
                  "      <arg direction=\"in\" type=\"Wintermute::Network::Message\"/>\n"
                  "      <annotation value=\"Wintermute::Data::Linguistics::Rules::Chain\" name=\"com.trolltech.QtDBus.QtTypeName.In0\"/>\n"
                  "    </method>\n"
                  "    <method name=\"isActive\">\n"
                  "      <arg direction=\"out\" type=\"b\"/>\n"
                  "    </method>\n"
                  "    <method name=\"quit\"/>\n"
                  "  </interface>\n"
                  "" )
public:
    explicit SystemAdaptor();

signals:
    void messageRecieved ( const Network::Message& );
    void started();
    void stopped();

public slots:
    Q_INVOKABLE Q_NOREPLY void sendMessage ( const Network::Message& );
    Q_INVOKABLE const bool isActive() const;
    virtual void quit() const;
};
}
};

#endif // ADAPTORS_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
