/**
 * @author Jacky Alcine <jackyalcine@gmail.com>
 *
 * @legalese
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

#include <QtDBus>
#include "ipc.hpp"
#include <QCoreApplication>

namespace Wintermute {
    string IPC::s_mod;
    DbusAdaptor* IPC::s_dbus;

    /// @todo #1: Start the network and plugin processes and check if they're alive.
    /// @todo #2: Initialize the network system and begin attempting to connect to a local socket, the Intranet, and the Internet.
    /// @todo #3: Load system plugins (input management) and load user plugins.
    void IPC::Initialize(const string& p_ipcMod){
        s_mod = p_ipcMod;
        s_dbus = new DbusAdaptor(QCoreApplication::instance ());
        QDBusConnection::sessionBus().registerObject("/Wintermute", QCoreApplication::instance ());

        cout << "(core) [IPC] Module '" << p_ipcMod << "' running." << endl;

        if (p_ipcMod == "master"){
            // See the first TODO.
        } else if (p_ipcMod == "network") {
            // See the second TODO.
        } else if (p_ipcMod == "plugin") {
            // See the third TODO.
        }
    }
}

// kate: indent-mode cstyle; space-indent on; indent-width 4;
