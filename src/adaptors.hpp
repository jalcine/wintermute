/*
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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

/**
 * @file    adaptors.hpp
 * @author  Wintermute Development <wntr-devel@thesii.org>
 */

#ifndef WINTERMUTE_ADAPTORS_HPP
#define WINTERMUTE_ADAPTORS_HPP

#include <QObject>
#include <QStringList>
#include <QVariantMap>
#include <QtDBus/QDBusAbstractAdaptor>

class QTimer;
class QSettings;
class QDBusMessage;

namespace Wintermute
{
namespace IPC
{
struct GenericAdaptorPrivate;

/**
 * @brief The foundational adaptor for Wintermute's IPC.
 *
 * Wintermute uses D-Bus on Linux systems as its means of achieving a
 * stabilized layer of inter-process communication. In doing so, a foundational
 * and optimized base for said transactions are required. The GenericAdaptor
 * serves as a base for communicating between multiple processes of Wintermute.
 *
 * @see Plugins::PluginFactoryAdaptor
 * @see Plugins::PluginPluginHandleAdaptor
 * @see CoreAdaptor
 */
class GenericAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO ("D-Bus Interface", "org.thesii.Wintermute.Adaptor")

private:
    QScopedPointer<GenericAdaptorPrivate> d_ptr;
    Q_SLOT void doDetect();

protected:
    Q_DECLARE_PRIVATE(GenericAdaptor)

public:
    /**
     * @brief Default constructor.
     * @fn GenericAdaptor
     * @param The QObject to set as this parent.
     */
    explicit GenericAdaptor (QObject* p_parent = 0);

    /**
     * @brief Obtains the module's name.
     * @fn module The name of the module.
     * @note This method is callable over D-Bus.
     */
    Q_INVOKABLE const QString module() const;

    /**
     * @brief Obtains the process's ID.
     * @fn pid The pid of the process.
     * @note This method is callable over D-Bus.
     */
    Q_INVOKABLE int pid() const;

signals:
    /**
     * @brief Emitted right before this process exits.
     * @see Wintermute::core::stopped()
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
    virtual void quit() const = 0;
};
}

typedef IPC::GenericAdaptor AbstractAdaptor;

namespace Plugins
{
struct AbstractPlugin;

/**
 * @brief An adaptor representing the management work of the plug-in factory.
 *
 * The plug-in factory is meant to be a hub of management for all of the
 * plug-ins loaded in Wintermute. It merely serves as a means of exposure
 * for the Factory interfacing class.
 *
 * @see Plugins::Factory
 */
class PluginFactoryAdaptor : public AbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO ("D-Bus Interface", "org.thesii.Wintermute.Factory")

public:
    /**
     * @brief Default constructor.
     * @fn PluginFactoryAdaptor
     */
    explicit PluginFactoryAdaptor();

signals:
    void pluginLoaded (const QString&) const;
    void pluginUnloaded (const QString&) const;
    void pluginCantLoad (const QString&) const;
    void pluginCrashed (const QString&) const;

public slots:
    virtual void quit() const;
    void loadPlugin (const QString&);
    void unloadPlugin (const QString&);
    const QStringList allPlugins() const;
    const QStringList loadedPlugins() const;
};

class PluginHandleAdaptor : public AbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO ("D-Bus Interface", "org.thesii.Wintermute.PluginInstance")

public:
    explicit PluginHandleAdaptor (Plugins::AbstractPlugin*);

signals:
    void pluginLoaded (const QString&) const;
    void pluginUnloaded (const QString&) const;
    void pluginCantLoad (const QString&) const;

public slots:
    virtual void quit() const;
    virtual void loadBackend (const QString&);
};

} // end namespace

class CoreAdaptor : public AbstractAdaptor
{
    Q_OBJECT
    Q_PROPERTY (const QVariantMap Arguments READ arguments)
    Q_CLASSINFO ("D-Bus Interface", "org.thesii.Wintermute.Master")

public:
    explicit CoreAdaptor();

public slots:
    virtual void quit() const;
    static void haltSystem();
    const QVariantMap arguments() const;
    Q_INVOKABLE Q_NOREPLY void ping (const QString&);
};
} // namespace

#endif // WINTERMUTE_ADAPTORS_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
