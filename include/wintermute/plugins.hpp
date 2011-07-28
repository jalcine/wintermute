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
#include <map>
#include <vector>
#include <QFile>

using namespace std;
using std::vector;
using std::map;

namespace Wintermute {
    /**
      This namespace should contain all of the required classes and work needed to
      properly load 'plugins' or libraries compiled against Wintermute's headers, but
      are loaded dynamically to provide extra functionality to the system.
    */
    namespace Plugins {
        struct Factory;
        struct Plugin;

        /**
         * @brief
         *
         * @typedef PluginVector
         */
        typedef vector<Plugin*> PluginVector;
        /**
         * @brief
         *
         * @typedef PluginMap
         */
        typedef map<const string, Plugin*> PluginMap;

        /**
         * @brief Factory management of plugins.
         * This abstract class manages anything and everything to do with plugins; from
         * loading, unloading, obtaining information and more about plugins.
         * @class Factory plugins.hpp "include/wintermute/plugins.hpp"
         * @see Plugin;
         */
        class Factory {
        public:
            /**
             * @brief
             *
             * @fn Startup
             */
            static void Startup();
            /**
             * @brief
             *
             * @fn Shutdown
             */
            static void Shutdown();
        };

        /**
         * @brief
         *
         * @class Plugin plugins.hpp "include/wintermute/plugins.hpp"
         */
        class Plugin {
        private:
            void* _handle;
            void init();
            void deinit();
            void runEntryMethod();
            void runCleanupMethod();
        public:
            /**
             * @brief
             *
             * @fn Plugin
             */
            Plugin();
            /**
             * @brief
             *
             * @fn Plugin
             * @param
             */
            Plugin(const Plugin& );
            const string getLibraryPath();
            ~Plugin();
        };
    }
}

#endif /* PLUGINS_HPP */
