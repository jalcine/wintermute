/**
 * @file    adaptors.hpp
 * @author  Wintermute Developers <wintermute-devel@lists.launchpad.net>
 * @created 9/6/2011
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
        /// @todo Have this register WNTR_APPLICATION as its parent and hold an object so that the object's contents aren't exposed over D-Bus.
        class GenericAdaptor : public QDBusAbstractAdaptor {
            Q_OBJECT
            Q_CLASSINFO("D-Bus Interface","org.thesii.Wintermute.Adaptor")

            private:
                QTimer *m_tmr;
                mutable bool m_core;
                Q_SLOT void detect() const;

            public:
                explicit GenericAdaptor(QObject* = 0);
                Q_INVOKABLE const QString module() const;
                Q_INVOKABLE const int pid() const;

            signals:
                void aboutToQuit() const;
                void coreModuleLoaded() const;
                void coreModuleUnloaded() const;

            public slots:
                virtual void quit(const QDBusMessage&) const = 0;
        };
    }

    typedef IPC::GenericAdaptor Adaptor;

    namespace Plugins {
        struct PluginBase;
        class PluginFactoryAdaptor : public Adaptor {
            Q_OBJECT
            Q_CLASSINFO("D-Bus Interface","org.thesii.Wintermute.Factory")

            public:
                explicit PluginFactoryAdaptor();
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

        class PluginInstanceAdaptor : public Adaptor {
            Q_OBJECT
            Q_CLASSINFO("D-Bus Interface","org.thesii.Wintermute.PluginInstance")

            public:
                explicit PluginInstanceAdaptor(Plugins::PluginBase*);

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
