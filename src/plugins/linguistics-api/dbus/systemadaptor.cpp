/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 03/04/12 6:27:46 AM
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
 * @endlegalese
 */


// Qt includes
#include <QDBusConnection>

// Wintermute includes
#include <app/adaptors.hpp>

// Local includes
#include <linguistics-api/system.hpp>
#include <app/config.hpp>
#include "systemadaptor.hpp"

using Wintermute::CoreAdaptor;
using Wintermute::Linguistics::SystemAdaptor;
using Wintermute::Linguistics::System;

SystemAdaptor::SystemAdaptor()
    : QDBusAbstractAdaptor (Wintermute::Linguistics::System::instance())
{
    QDBusConnection::sessionBus().connect (WNTR_DBUS_SERVICE_NAME, "/Master", WNTR_DBUS_MASTER_NAME, "aboutToQuit",
                                           this, SLOT (quit()));
    setAutoRelaySignals (true);
}

SystemAdaptor::~SystemAdaptor() { }

void SystemAdaptor::start()
{
    QMetaObject::invokeMethod (parent(), "start");
}

void SystemAdaptor::quit()
{
    QMetaObject::invokeMethod (parent(), "stop");
    CoreAdaptor::haltSystem();
}

void SystemAdaptor::stop()
{
    QMetaObject::invokeMethod (parent(), "stop");
    CoreAdaptor::haltSystem ();
}

void SystemAdaptor::tellSystem (const QString& in0)
{
    QMetaObject::invokeMethod (parent(), "tellSystem", Q_ARG (QString, in0));
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
