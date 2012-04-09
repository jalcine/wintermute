/***
 *  This file is part of Wintermute.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
 *
 *  Wintermute is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  Wintermute is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with Wintermute.  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <QDebug>
#include <QDBusMessage>
#include <config.hpp>
#include <ipc.hpp>
#include "adaptors.hxx"
using namespace Wintermute::IPC;

GenericAdaptorPrivate::GenericAdaptorPrivate(GenericAdaptor* p_qPtr) : q_ptr(p_qPtr) {

}

void GenericAdaptorPrivate::detect() const
{
    Q_Q(const GenericAdaptor);
    m_tmr->stop();
    const bool prv = m_core;
    QDBusMessage ping = QDBusMessage::createMethodCall (WNTR_DBUS_SERVICE_NAME, "/Master", WNTR_DBUS_MASTER_NAME, "ping");
    ping << System::module ();
    ping.setAutoStartService (true);
    QDBusMessage pingReply = IPC::System::bus ()->call (ping, QDBus::BlockWithGui);
    m_core = pingReply.type () != QDBusMessage::ErrorMessage;

    if (m_core != prv) {
        if (m_core) {
            qDebug() << "(core) [D-Bus] Core module found.";
            emit q->coreModuleLoaded ();
        }
        else {
            qDebug() << "(core) [D-Bus] Core module lost.";
            emit q->coreModuleUnloaded ();
        }
    }

    if (pingReply.type () == QDBusMessage::ErrorMessage) {
        //qDebug() << "(core) [D-Bus] Pong from core module:" << pingReply.errorMessage ();
        /*if (!Core::arguments().value ("daemon").toBool ())
            CoreAdaptor::haltSystem ();*/
    }

    m_tmr->start ();
}

GenericAdaptorPrivate::~GenericAdaptorPrivate()
{

}
