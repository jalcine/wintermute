/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2014 Jacky Alciné <me@jalcine.me>
 *
 * Wintermute is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef WINTERMUTE_DBUS_ADADPTOR_HPP
#define WINTERMUTE_DBUS_ADADPTOR_HPP

#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusMessage>
#include <Wintermute/Globals>
#include <Wintermute/Procedure/Call>
#include "globals.hpp"

namespace Wintermute
{
namespace DBus
{
class Module;
class Adaptor : public QDBusAbstractAdaptor
{
  Q_OBJECT;
  Q_CLASSINFO ( "D-Bus Interface", "in.wintermute.dbus" );
   Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"in.wintermute.dbus\">\n"
"    <method name=\"handleIncomingCall\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"arguments\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"hasModule\">\n"
"      <arg direction=\"out\" type=\"b\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"name\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
  public:
    explicit Adaptor( Module* module );
    void registerOnDBus();
    virtual ~Adaptor();

  public slots:
    Q_NOREPLY void handleIncomingCall ( const QString& arguments,
        const QDBusMessage& message );
    bool hasModule ( const QString& name );
};
}
}

#endif
