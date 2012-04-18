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

// local includes
#include "systemadaptor.hpp"
#include "../system.hpp"

using Wintermute::Data::System;
using Wintermute::Data::SystemAdaptor;

SystemAdaptor::SystemAdaptor()
    : QDBusAbstractAdaptor (System::instance())
{
    // constructor
    setAutoRelaySignals (true);
}

SystemAdaptor::~SystemAdaptor() { }

QString SystemAdaptor::directory() const
{
    return qvariant_cast< QString > (parent()->property ("Directory"));
}

void SystemAdaptor::setDirectory (const QString& value)
{
    parent()->setProperty ("Directory", qVariantFromValue (value));
}

bool SystemAdaptor::localeExists (const QString& in0)
{
    bool out0;
    QMetaObject::invokeMethod (parent(), "localeExists", Q_RETURN_ARG (bool, out0), Q_ARG (QString, in0));
    return out0;
}

void SystemAdaptor::quit()
{
    QMetaObject::invokeMethod (parent(), "quit");
}

void SystemAdaptor::start()
{
    QMetaObject::invokeMethod (parent(), "start");
}

void SystemAdaptor::stop()
{
    QMetaObject::invokeMethod (parent(), "stop");
}

#include "dbus/systemadaptor.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
