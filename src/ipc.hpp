/**
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
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
#ifndef IPC_HPP
#define IPC_HPP

#include "wintermute.hpp"
#include <QtDBus>
#include <string>
#include <iostream>

using namespace std;
using namespace Wintermute;

namespace Wintermute {
    struct IPC;
    struct DBusAdaptor;

    /**
     * @brief Represents the Inter Process Communication (IPC) management of Wintermute.
     *
     * This class manages the inter process communication of Wintermute. It handles the
     * incoming and outcoming requests of data, and allows Wintermute to fork off into
     * the approriate processes that represent it.
     *
     * Wintermute can run as one of the following sub-modules:
     * - <b>master</b>: Represents the core module, or the master daemon that'll be a means
     * of regulating the sub processes.
     *
     * - <b>network</b>: Represents the network module. This module typically runs asynchronously
     * of the core Wintermute processes and implements most of its work in the core networking library (WntrNtwk).
     *
     * - <b>plugin</b>: Represnts the plugin module. Plug-ins are run in their process, allowing them
     * to take advantage of their own work, and prevents system failure if the plug-in happens to
     * crash.
     *
     * @note If you're attempting to send messages via Wintermute, you should look at @c DBusAdaptor.
     *
     * @see DBusAdaptor
     * @class IPC ipc.hpp "include/wntr/ipc.hpp"
     */
    class IPC {
        friend class DBusAdaptor;
        public:
            /**
             * @brief Initializes the IPC system.
             *
             * Starts up the IPC system by storing the type of module that Wintermute's
             * running under and executing the code required to render that module.
             *
             * @fn Initialize
             * @param
             */
            static void Initialize (const string& = "master");

            /**
             * @brief Obtains the current module.
             * @fn currentModule
             * @return The name of the running module (either 'master', 'network' or 'plugin').
             */
            static const string currentModule() { return s_mod; }

        private:
            /**
             * @brief
             * @fn IPC
             */
            IPC();
            /**
             * @brief
             * @fn ~IPC
             */
            ~IPC();
            static string s_mod;
            static DBusAdaptor* s_dbus;
    };

    /**
     * @brief
     * @class DBusAdaptor ipc.hpp "include/wntr/ipc.hpp"
     */
    class DBusAdaptor : public QDBusAbstractAdaptor {
        Q_OBJECT
        Q_CLASSINFO("D-Bus Interface","org.thesii.DBus.Wintermute")
        Q_CLASSINFO("Author","Synthetic Intellect Institute")
        Q_CLASSINFO("URL","http://www.thesii.org")

        Q_PROPERTY(string module READ module)

        public:
            /**
             * @brief
             *
             * @fn DBusAdaptor
             */
            DBusAdaptor() : QDBusAbstractAdaptor(NULL) { }
            /**
             * @brief
             * @fn DbusAdaptor
             * @param p_app
             */
            DBusAdaptor(QCoreApplication* p_app) : QDBusAbstractAdaptor(p_app) { }
            /**
             * @brief
             *
             * @fn DBusAdaptor
             * @param p_dbus
             */
            DBusAdaptor(const DBusAdaptor& p_dbus) : QDBusAbstractAdaptor(NULL) { }
            /**
             * @brief
             * @fn ~DbusAdaptor
             */
            ~DBusAdaptor() { emit destroyed (this); }

        public slots:
            /**
             * @brief
             * @fn module
             * @return string
             */
            Q_INVOKABLE string module() { return IPC::s_mod; }
            /**
             * @brief
             * @fn quit
             */
            Q_NOREPLY void quit(){
                cout << "(core) [Dbus] Recieved quit signal. Quitting..." << endl;
                QCoreApplication::instance ()->quit ();
                exit(0);
            }
    };
}

#endif /* IPC_HPP */
// kate: indent-mode cstyle; space-indent on; indent-width 4;
