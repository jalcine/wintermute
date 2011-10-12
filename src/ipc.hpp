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
#include "adaptors.hpp"
#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusInterface>
#include <QVariantMap>


namespace Wintermute {
    namespace Plugins { struct Factory; } //forward class decl;
    namespace IPC {
        struct System;

        /**
         * @brief Represents the Inter Process Communication (IPC) management of Wintermute.
         *
         * This class manages the inter process communication of Wintermute. It handles the
         * incoming and outcoming requests of data, and allows Wintermute to fork off into
         * the approriate processes that represent it. Wintermute uses D-Bus to do the inter
         * communications and also can manage external resource requests. This makes it possible
         * to extend Wintermute's public API to other applications or even languages that can
         * work with D-Bus.
         *
         * @section N01 Modules
         *
         * Wintermute can run as one of the following sub-modules:
         * - <b>Master</b>: Represents the core module, or the master daemon that'll be a means
         * of regulating the sub processes.
         *
         * - <b>Plugin</b>: Represents the plugin module. Plug-ins are run in their process, allowing them
         * to take advantage of their own work, and prevents system failure if the plug-in happens to
         * crash.
         *
         * @section N02 D-Bus
         *
         * Wintermute's domain lives on the session bus, under the name <b>org.thesii.Wintermute</b>.
         * The Core object is exposed as "/Master" on the home domain, with the interface name "org.thesii.Wintermute.Master".
         * You can find the plug-in factory under "/Factory" on the same domain, but this may move to the domain
         * <b>org.thesii.Wintermute.Plugin</b>. Other components of Wintermute, such as the <b>Data</b>,
         * <b>Network</b> and the <b>Linguistics</b> module can be found at sub-domains of the home domain.
         * Start Wintermute and use the <b>qdbusviewer</b> application to peek at the intermingling.
         *
         * @section N03 Plug-ins
         *
         * Plug-ins each have their own D-Bus domain (since they're run in a sandbox; see @c PluginInstance).
         * The domain would be a subset of <b>org.thesii.Wintermute.Plugin-{PLUGIN_UUID}</b>. This allow remote access
         * of plug-ins from processes and remoting loading of plug-ins whenever needed. Plug-ins domains
         * are able to run certain exposed parts of a plug-in by using the <b>invoke()</b> method (see @c PluginBase)
         * and thus allowing a dynamic API based on plug-ins for Wintermute.
         *
         * @see CoreAdaptor, PluginBase, PluginInstance, Factory
         * @class System ipc.hpp "src/ipc.hpp"
         */
        class System : public QObject {
            friend class Plugins::Factory;
            Q_OBJECT
            Q_DISABLE_COPY(System)

            public:
                /**
                 * Starts up the IPC system by storing the type of module that Wintermute's
                 * running under and executing the code required to render that module.
                 * @fn Initialize
                 */
                static void start ( );

                /**
                 * Stops all of the work of the IPC system and then exits Wintermute.
                 * @fn stop
                 */
                static void stop();

                /**
                 * @brief Obtains the current module.
                 * @fn currentModule
                 * @return The name of the running module.
                 */
                static inline const QString module() { return s_appMod; }

                /**
                 * @brief Obtains the currently running bus.
                 * @fn connection
                 */
                static QDBusConnection* bus();

                /**
                 * @brief Obtains the adaptor being used (most likely the SystemAdaptor).
                 * @fn adaptor
                 */
                static Adaptor* adaptor();

                /**
                 * @brief Registers an Adaptor onto the current D-Bus bus.
                 * @fn registerObject
                 * @param QString The name of the Adaptor.
                 * @param Adaptor* The Adaptor to be added.
                 */
                static const bool registerObject(const QString&, Adaptor* );

                /**
                 * @brief Does the work of adding user data-types (user PODs) as
                 *        recognizable, marshallable types for QDBusArgument.
                 * @fn registerDataTypes
                 */
                static void registerDataTypes();

            private:
                static QString s_appMod;
                static QDBusConnection* s_cnntn;
                static Adaptor* s_adapt;
        };
    }
}

#endif
#endif /* IPC_HPP */
// kate: indent-mode cstyle; space-indent on; indent-width 4;
