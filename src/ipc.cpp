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

#include "ipc.hpp"
#include <wntrntwk.hpp>
#include <QtDBus>
#include <QProcess>
#include <QCoreApplication>

namespace Wintermute {
    string IPC::s_mod;
    DBusAdaptor* IPC::s_dbus;

    /// @todo Connect to D-Bus via an wrapper class (make that a static object of IPC).
    /// @todo Determine the actions to be taken depending on the IPC module.
    void IPC::Initialize(const string& p_ipcMod){
        s_mod = p_ipcMod;
        s_dbus = new DBusAdaptor(QCoreApplication::instance ());
        QDBusConnection::sessionBus().registerObject("/Wintermute", Core::appInstance());

        cout << "(core) [IPC] Module '" << p_ipcMod << "' running." << endl;

        if (p_ipcMod == "master")
            Core::Initialize ();
        else if (p_ipcMod == "network")
            Network::Initialize ();
    }
}

// kate: indent-mode cstyle; space-indent on; indent-width 4;
