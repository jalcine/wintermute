/**
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
 * @file ipc.hpp
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
#ifndef NCURSES_HPP
#define IPC_HPP

#include "config.hpp"
#include <QtDBus>
#include <string>
#include <iostream>

using namespace std;

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
     * - <b>Master</b>: Represents the core module, or the master daemon that'll be a means
     * of regulating the sub processes.
     *
     * - <b>Network</b>: Represents the network module. This module typically runs asynchronously
     * of the core Wintermute processes and implements most of its work in the core networking library (WntrNtwk).
     *
     * - <b>Plugin</b>: Represnts the plugin module. Plug-ins are run in their process, allowing them
     * to take advantage of their own work, and prevents system failure if the plug-in happens to
     * crash.
     *
     * @todo We need to implement locks (and remind developers that work on plug-ins with local files) to implement locks for their files.
     * @see DBusAdaptor
     * @class IPC ipc.hpp "include/wntr/ipc.hpp"
     */
    class IPC : public QObject {
        friend class DBusAdaptor;
        Q_OBJECT
        Q_DISABLE_COPY(IPC)

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
            static void Initialize ( const QString& = "Master" );

            /**
             * @brief Obtains the current module.
             * @fn currentModule
             * @return The name of the running module (either 'Master', 'Network' or 'Plugin').
             */
            static inline const QString currentModule() { return s_appMod; }

        private:
            static QString s_appMod;
    };

    class DBusAdaptor : public QDBusAbstractAdaptor {
        friend class IPC;
        Q_OBJECT
        Q_CLASSINFO("D-Bus Interface","org.thesii.Wintermute")
        Q_PROPERTY(const QString Module READ module)

        signals:
            void started();
            void stopped();

        private:
            static DBusAdaptor* s_dbus;
            QDBusInterface *m_mstrIntr;

        public:
            DBusAdaptor();
            ~DBusAdaptor();

        public slots:
            const QString module() const;
            Q_NOREPLY void quit() const;

        protected slots:
            void reportAlive(const QDBusMessage &) const;
    };
}

#endif
#endif /* IPC_HPP */
// kate: indent-mode cstyle; space-indent on; indent-width 4;
