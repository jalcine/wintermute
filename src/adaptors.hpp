/**
 * @file    adaptors.hpp
 * @author  Wintermute Developers <wintermute-devel@lists.launchpad.net>
 *  
 *
 *
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Wintermute Linguistics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute Linguistics; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef WINTERMUTE_ADAPTORS_HPP
#define WINTERMUTE_ADAPTORS_HPP

#include <QStringList>
#include <QVariantMap>
#include <QSettings>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusAbstractAdaptor>

struct QTimer;

namespace Wintermute {
    namespace IPC {

        /**
         * @brief The foundational adaptor for Wintermute's IPC.
         *
         * Wintermute uses D-Bus on Linux systems as its means of achieveing a
         * stabilized layer of inter-process communication. In doing so, a foundational
         * and optimized base for said transactions are required. The GenericAdaptor
         * serves as a base for communicating between multiple processes of Wintermute.
         *
         * @see Plugins::PluginFactoryAdaptor, Plugins::PluginInstanceAdaptor, CoreAdaptor
         * @class GenericAdaptor adaptors.hpp "src/adaptors.hpp"
         */
        class GenericAdaptor : public QDBusAbstractAdaptor {
            Q_OBJECT
            Q_CLASSINFO("D-Bus Interface","org.thesii.Wintermute.Adaptor")

            private:
                QTimer *m_tmr;
                mutable bool m_core;
                Q_SLOT void detect() const;

            public:
                /**
                 * @brief Default constructor.
                 * @fn GenericAdaptor
                 * @param The QObject to set as this parent.
                 */
                explicit GenericAdaptor(QObject* = 0);

                /**
                 * @brief Obtains the module's name.
                 * @fn module The name of the module.
                 */
                Q_INVOKABLE const QString module() const;

                /**
                 * @brief Obtains the process's ID.
                 * @fn pid The pid of the process.
                 */
                Q_INVOKABLE const int pid() const;

            signals:
                /**
                 * @brief Emitted right before this process exits.
                 * @see Wintermute::core::stopped();
                 * @fn aboutToQuit
                 */
                void aboutToQuit() const;

                /**
                 * @brief Emitted when the core module has been detected.
                 * @fn coreModuleLoaded
                 */
                void coreModuleLoaded() const;

                /**
                 * @brief Emitted when the core module isn't detected anymore.
                 * @fn coreModuleUnloaded
                 */
                void coreModuleUnloaded() const;

            public slots:
                virtual void quit(const QDBusMessage&) const = 0;
        };
    }

    typedef IPC::GenericAdaptor Adaptor;

    namespace Plugins {
        struct AbstractPlugin;

        /**
         * @brief An adaptor representing the management work of the plug-in factory.
         *
         * The plug-in factory is meant to be a hub of management for all of the
         * plug-ins loaded in Wintermute. It merely serves as a means of exposure
         * for the Factory interfacing class.
         *
         * @see Plugins::Factory
         * @class PluginFactoryAdaptor adaptors.hpp "src/adaptors.hpp"
         */
        class PluginFactoryAdaptor : public Adaptor {
            Q_OBJECT
            Q_CLASSINFO("D-Bus Interface","org.thesii.Wintermute.Factory")

            public:
                /**
                 * @brief Default constructor.
                 * @fn PluginFactoryAdaptor
                 */
                explicit PluginFactoryAdaptor();

                /**
                 * @brief Loads a plug-in.
                 * @fn loadPlugin
                 * @param string The name of the plug-in.
                 */
                Q_INVOKABLE Q_NOREPLY void loadPlugin(const QString&, const QDBusMessage&);
                Q_INVOKABLE Q_NOREPLY void unloadPlugin(const QString&, const QDBusMessage&);
                Q_INVOKABLE const QStringList allPlugins(const QDBusMessage&) const;
                Q_INVOKABLE const QStringList loadedPlugins(const QDBusMessage&) const;
                Q_INVOKABLE const QSettings* pluginInfo(const QString&, const QDBusMessage&) const;

            signals:
                void pluginLoaded(const QString&) const;
                void pluginUnloaded(const QString&) const;
                void pluginCantLoad(const QString&) const;
                void pluginCrashed(const QString&) const;

            public slots:
                virtual void quit(const QDBusMessage&) const;
        };

        class InstanceAdaptor : public Adaptor {
            Q_OBJECT
            Q_CLASSINFO("D-Bus Interface","org.thesii.Wintermute.PluginInstance")

            public:
                explicit InstanceAdaptor(Plugins::AbstractPlugin*);

            signals:
                void pluginLoaded(const QString&) const;
                void pluginUnloaded(const QString&) const;
                void pluginCantLoad(const QString&) const;

            public slots:
                virtual void quit(const QDBusMessage&) const;
        };
    }

    class CoreAdaptor : public Adaptor {
            Q_OBJECT
            Q_CLASSINFO("D-Bus Interface","org.thesii.Wintermute.Master")
            Q_PROPERTY(const QVariantMap Arguments READ arguments)

            public:
                explicit CoreAdaptor();
                const QVariantMap arguments() const;
                Q_INVOKABLE Q_NOREPLY void ping(const QString&);
                static void haltSystem();

            public slots:
                virtual void quit(const QDBusMessage&) const;
    };
}

#endif // WINTERMUTE_ADAPTORS_HPP
