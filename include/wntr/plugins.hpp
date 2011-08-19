/**
 * @author Jacky Alcine <jackyalcine@gmail.com>
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

#ifndef PLUGINS_HPP
#define PLUGINS_HPP
#include <QMap>
#include <QFile>
#include <QVector>
#include <QPluginLoader>
#include "config.hpp"

using namespace std;
using std::vector;
using std::map;

namespace Wintermute {
    namespace Plugins {
        struct Factory;
        struct PluginBase;

        /**
         * @brief Represents a set of plugins.
         * @typedef PluginVector
         */
        typedef QVector<PluginBase*> PluginVector;
        /**
         * @brief Represents a named set of plugins.
         * @typedef PluginMap
         */
        typedef QMap<const string, PluginBase*> PluginMap;

        /**
         * @brief Factory management of plugins.
         * This abstract class manages anything and everything to do with plugins; from
         * loading, unloading, obtaining information and more about plugins.
         * @class Factory plugins.hpp "include/wintermute/plugins.hpp"
         * @see Plugin
         */
        class Factory {
            friend class Plugin;
            public:
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
                /**
                 * @brief Loads a plugin.
                 * @fn loadPlugin
                 * @param
                 */
                static const PluginBase* loadPlugin ( const string& );
                /**
                 * @brief Loads a plugin.
                 * @fn loadPlugin
                 * @overload
                 * @param
                 */
                static const PluginBase* loadPlugin ( const QFile* );
            private:
                static PluginVector s_allPlgns;
        };

        /**
         * @brief Abstract class representing the outlining information of a plug-in.
         * @todo Implement a means of updating plug-ins as well.
         * @class Plugin plugins.hpp "include/wntr/plugins.hpp"
         */
        class PluginBase {
            friend class Factory;

            private:
                QPluginLoader* m_plgnLdr;

            public:
                explicit PluginBase();
                PluginBase(const PluginBase& );
                virtual ~PluginBase() = 0;
                virtual inline const string id () const { return ""; }
                virtual inline const string name () const { return ""; }
                virtual inline const string friendlyName () const { return ""; }
                virtual inline const string vendorName () const{ return ""; }
                virtual inline const string description () const { return ""; }
                virtual inline const string webPage () const { return ""; }
                virtual inline const double version() const { return -1.0; }
                virtual void initialize() { }
                virtual void deinitialize() { }
                virtual QObject* instance() { }

                const bool isSupported() const;
                const QString path() const;

                bool operator == (const PluginBase& );
        };

    }
}

Q_DECLARE_INTERFACE(Wintermute::Plugins::PluginBase, "thesii.Wntr.PluginBase");

#endif /* PLUGINS_HPP */
// kate: indent-mode cstyle; space-indent on; indent-width 4;
