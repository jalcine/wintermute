/***
 *  This file is part of the project project.
 *
 *  Copyright (C) 2012 Jacky Alciné <me@jalcine.me>
 *  Copyright (C) 2012 Adrian Borucki <gentoolx@gmail.com>
 *
 *  project is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with project .
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <me@jalcine.me>
 * @author Adrian Borucki <gentoolx@gmail.com>
 * @date 05/02/12 2:02:28 PM
 */

#ifndef WINTERMUTE_FACTORY_HPP
#define WINTERMUTE_FACTORY_HPP

#define WINTER_COMMAND_LINE_FACTORY        "plugin"
#define WINTER_COMMAND_LINE_FACTORY_DAEMON "daemon"

#include <global.hpp>
#include <plugin.hpp>
#include <pluginhandle.hpp>

WINTER_FORWARD_DECLARE_STRUCT (FactoryPrivate)
WINTER_BEGIN_NAMESPACE

class Factory;

/**
 * @brief Provides factory management of plug-ins.
 *
 * This static class manages anything and everything to do with plug-ins; from
 * loading, unloading, obtaining information and more about plugins. A lot of the
 * internal working dealing with plug-ins, however, are done within the AbstractPlugin
 * class itself. This merely manages the loaded plug-ins and executes prerequisties commands.
 *
 * @class Factory plugins.hpp "include/wintermute/plugins.hpp"
 * @see AbstractPlugin
 */
class WINTER_EXPORT Factory : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (Factory)
    WINTER_DECLARE_PRIVATE_STRUCT (Factory)
    WINTER_SINGLETON (Factory)
    friend struct AbstractPluginPrivate;
    QScopedPointer<FactoryPrivate> d_ptr;

signals:
    /**
     * @brief Emitted when a plug-in has been successfully loaded into the system.
     * @fn pluginLoaded
     * @param p_uuid The UUID of the loaded plug-in.
     */
    void pluginLoaded (const QString& p_uuid) const;

    /**
     * @brief Emitted when a plug-in has been successfully unloaded from the system.
     * @fn pluginUnloaded
     * @param p_uuid The UUID of the unloaded plug-in.
     */
    void pluginUnloaded (const QString& p_uuid) const;

    /**
     * @brief Emitted when a plug-in experiences a sporadic crash.
     * @fn pluginCrashed
     * @param p_uuid The UUID of the faulty plug-in.
     */
    void pluginCrashed (const QString& p_uuid) const;

    /**
     * @brief Emitted when the factory's up and running.
     */
    void started() const;

    /**
     * @brief Emitted when the factory's down for the count.
     */
    void stopped() const;

public slots:

    /**
     * @brief Starts the plug-in system.
     * @fn Startup
     */
    static void startup();
    /**
     * @brief Stops the plug-in system.
     * @fn Shutdown
     */
    static void shutdown();

public:
    /**
     * @brief Loads a plug-in.
     * @fn loadPlugin
     * @param
     */
    static AbstractPlugin* loadPlugin (const QString& p_uuid);

    /**
     * @brief Unloads a plug-in from the system.
     * @fn unloadPlugin
     * @param
     */
    static void unloadPlugin (const QString& p_uuid);

    /**
     * @brief Determines if a plug-in exists in the system.
     * @fn existsPlugin
     * @param 
     */
    static bool existsPlugin(const QString& p_uuid);

    /**
     * @brief Returns a list of all currently plug-ins with meta-data information.
     * @fn loadedPlugins
     * @returns A QList of plug-ins that are currently loaded into the system.
     */
    static QStringList loadedPlugins();

    /**
     * @brief Obtains a list of all of the plug-ins that are installed and recognized by Wintermute.
     * @fn allPlugins
     * @return const QStringList
     */
    static QStringList allPlugins();

    /**
     * @brief Obtains the currently loaded plug-in.
     *
     * Typically, if this is running a plug-in module (IPC::module() == WINTER_COMMAND_LINE_IPC_PLUGIN),
     * then this'll return a pointer to the running plug-in instance.
     *
     * @return A pointer to a AbstractPlugin that represents the currently loaded plug-in; NULL otherwise.
     **/
    static AbstractPlugin* currentPlugin();

    /**
     * @brief Obtains the value of an attribute of a plug-in's configuration.
     *
     * @param p_uuid The UUID of the plug-in.
     * @param p_attributePath The path of the configuration option in question.
     **/
    static QVariant attribute (const QString& p_uuid, const QString& p_attributePath);

    /**
     * @brief Sets an attribute with a given value for a plug-in's configuration.
     *
     * @param p_uuid The UUID of the plug-in.
     * @param p_attributePath The path of the configuration option in question.
     * @param p_attributeValue The value to be used.
     **/
    static void setAttribute (const QString& p_uuid, const QString& p_attributePath, const QVariant& p_attributeValue);

    /**
     * @brief Obtains a plug-in from the loaded pool.
     *
     * @param p_uuid The UUID of the plug-in.
     **/
    static AbstractPlugin* obtainPlugin (const QString& p_uuid);

    /**
     * @brief Sets up the Factory for external communications.
     */
    static void prepareAdaptor();

    /**
     * @brief Sets up the Factory to start a plugin for external 
     * communications.
     */
    static void preparePluginAdaptor();

protected:
    /**
     * @brief Obtains the specification options (not the configuration used by the plug-in).
     * @param string The UUID of the plug-in in question.
     * @return A pointer to a QSettings object or NULL if the UUID doesn't refer to a plug-in.
     */
    static QSettings* getPluginSettings (const QString& p_uuid);

    /**
     * @brief
     * @param string The UUID of the plug-in in question.
     * @return True if the plug-in has been loaded successfully, false otherwise.
     */
    static bool loadBackendPlugin (const QString& p_uuid);

private slots:
    /**
     * @brief Loads the plug-in defined to be executed from the command-line.
     * @fn loadStandardPlugin
     */
    static void loadStandardPlugin();

    /**
     * @brief Unloads the plug-in defined to be executed from the command-line.
     * @fn unloadStandardPlugin
     */
    static void unloadStandardPlugin();

    /**
     * @brief Represents the notifying of a plug-in's loading into the plug-in pool.
     * @fn doPluginLoad
     * @param string The UUID of the plug-in.
     */
    static void doPluginLoad (const QString& p_uuid);

    /**
     * @brief Represents the notifying of a plug-in's unloading from the plug-in pool.
     * @fn doPluginUnload
     * @param string The UUID of the plug-in.
     */
    static void doPluginUnload (const QString& p_uuid);

    /**
     * @brief Represents the notifying of a plug-in's crash.
     * @fn doPluginCrash
     * @param string The UUID of the plug-in.
     */
    static void doPluginCrash (const QString& p_uuid);
};

WINTER_END_NAMESPACE

#endif // _FACTORY_HPP_
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
