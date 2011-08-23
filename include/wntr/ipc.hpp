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
#ifndef IPC_HPP
#define IPC_HPP

#include <iostream>
#include <string>
#include <QtDBus>
#include "wintermute.hpp"

using namespace std;
using namespace Wintermute;

namespace Wintermute {
    struct IPC;
    struct DBusAdaptor;

    /**
     * @brief Represents the Inter Process Communication (IPC) management of Wintermute.
     * @class IPC ipc.hpp "include/wntr/ipc.hpp"
     */
    class IPC {
        friend class DBusAdaptor;
        public:
            /**
             * @brief
             * @fn Initialize
             * @param
             */
            static void Initialize (const string& = "master");
            /**
             * @brief
             * @fn currentModule
             * @return const string
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
     * @class DbusAdaptor ipc.hpp "include/wntr/ipc.hpp"
     */
    class DBusAdaptor : protected QDBusAbstractAdaptor {
        Q_OBJECT
        Q_CLASSINFO("D-Bus Interface","org.thesii.DBus.Wintermute")
        Q_CLASSINFO("Author","Synthetic Intellect Institute")
        Q_CLASSINFO("URL","http://www.thesii.org")

        Q_PROPERTY(string module READ module)

        public:
            /**
             * @brief
             * @fn DbusAdaptor
             * @param p_app
             */
            DBusAdaptor(QCoreApplication* p_app) : QDBusAbstractAdaptor(p_app) { }
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
