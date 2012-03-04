/***
 * @file factory.hpp
 * @author Jacky Alcine <jacky.alcine@thesii.org>
 * @author Adrian Borucki <adrian@thesii.org>
 *
 * @legalese
 * This program is free software; you can redistribute it and/or
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

#ifndef _FACTORY_HPP_
#define _FACTORY_HPP_

// Local
#include "abstractplugin.hpp"
#include "pluginhandle.hpp"

namespace Wintermute {
namespace Plugins {

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
class Factory : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( Factory );
    friend class AbstractPlugin;

signals:
    /**
     * @brief Emitted when a plug-in has been successfully loaded into the system.
     * @fn pluginLoaded
     * @param p_uuid The UUID of the loaded plug-in.
     */
    void pluginLoaded ( const QString& ) const;

    /**
     * @brief Emitted when a plug-in has been successfully unloaded from the system.
     * @fn pluginUnloaded
     * @param p_uuid The UUID of the unloaded plug-in.
     */
    void pluginUnloaded ( const QString& ) const;

    /**
     * @brief Emitted when a plug-in experiences a sporadic crash.
     * @fn pluginCrashed
     * @param p_uuid The UUID of the faulty plug-in.
     */
    void pluginCrashed ( const QString& ) const;

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
    static void Startup();
    /**
     * @brief Stops the plug-in system.
     * @fn Shutdown
     */
    static void Shutdown();

public:
    /**
     * @brief A helper class for plug-ins.
     * Shell plug-ins allows the Factory to learn about a plug-in configuration
     * and setup without loading all of its libraries and dependencies. It's also
     * provided since AbstractPlugin is an abstract class.
     */
    class ShellPlugin : public AbstractPlugin {
        friend class Factory;
        friend class AbstractPlugin;

    public:
        ShellPlugin();
        ShellPlugin ( const QString & );
        virtual ~ShellPlugin();

    private:
        virtual void start () const { }
        virtual void stop () const { }
    };

    Factory();
    /**
     * @brief Loads a plug-in.
     * @fn loadPlugin
     * @param
     */
    static AbstractPlugin* loadPlugin ( const QString& );

    /**
     * @brief Unloads a plug-in from the system.
     * @fn unloadPlugin
     * @param
     */
    static void unloadPlugin ( const QString& );

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
     * @brief Obtains an instance of the Factory.
     * @fn instance
     * @return const Factory *
     */
    static Factory* instance();

    /**
     * @brief ...
     *
     * @return :Plugins::AbstractPlugin*
     **/
    static AbstractPlugin* currentPlugin();

    /**
     * @brief ...
     *
     * @param  ...
     * @param  ...
     * @return QVariant
     **/
    static QVariant attribute ( const QString& , const QString& );

    /**
     * @brief ...
     *
     * @param  ...
     * @param  ...
     * @param  ...
     * @return void
     **/
    static void setAttribute ( const QString&, const QString&, const QVariant& );

private:
    static PluginTable s_plgnLst; /**< Holds a list  */
    static Factory* s_fctry; /**< Holds the Factory's instance. */
    static AbstractPlugin* s_rtPlgn;
    QHash<const QString, PluginHandle *> m_plgnPool;

    /**
     * @brief Obtains the specification options (not the configuration used by the plug-in).
     * @param string The UUID of the plug-in in question.
     * @return A pointer to a QSettings object or NULL if the UUID doesn't refer to a plug-in.
     */
    static QSettings* getPluginSettings ( const QString& );

    /**
     * @brief
     * @param string The UUID of the plug-in in question.
     * @return True if the plug-in has been loaded successfully, false otherwise.
     */
    static bool loadBackendPlugin ( const QString& p_plgnUuid );

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
    static void doPluginLoad ( const QString& );

    /**
     * @brief Represents the notifying of a plug-in's unloading from the plug-in pool.
     * @fn doPluginUnload
     * @param string The UUID of the plug-in.
     */
    static void doPluginUnload ( const QString& );

    /**
     * @brief Represents the notifying of a plug-in's crash.
     * @fn doPluginCrash
     * @param string The UUID of the plug-in.
     */
    static void doPluginCrash ( const QString& );
};

} // namespaces
}

#endif // _FACTORY_HPP_
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
