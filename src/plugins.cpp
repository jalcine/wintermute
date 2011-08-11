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

#include <iostream>
#include <dlfcn.h>
#include "plugins.hpp"

using namespace std;
using std::cout;
using std::endl;

namespace Wintermute {
    namespace Plugins {
        PluginVector Factory::m_allPlgns;

        /// @todo Load the plugins designated to be loaded.
        void Factory::Startup (){
            cout << "(core) [Factory] Starting up..." << endl;
            const Plugin* l_plgn = Plugin::fromPath("/usr/lib/i386-linux-gnu/libstdc++.so.6");
        }

        /// @todo Unload every loaded plugin and free all resources.
        void Factory::Shutdown (){
            cout << "(core) [Factory] Shutting down..." << endl;
        }

        Plugin::Plugin() : _handle(NULL){}
        Plugin::Plugin(const Plugin &copiedPlugin): _handle(copiedPlugin._handle), _path(copiedPlugin._path) { init(); }
        Plugin::Plugin(const string &p_plgnPth) : _path(p_plgnPth), _handle(NULL) { init(); }
        Plugin::~Plugin () { runCleanupMethod (); }

        const string Plugin::getLibraryPath (){
            return _path;
        }

        void Plugin::init(){
            Factory::m_allPlgns.push_back (this);
            if (!_handle){
                _handle = dlopen(this->getLibraryPath().c_str (),RTLD_NOW);
                const char* dler = dlerror ();
                if (dler != NULL){
                    cout << "(core) [Plugins] Error loading '" << this->getLibraryPath () << "'; " << dler << endl;
                    return;
                }
            }
            runEntryMethod();
        }

        void Plugin::runEntryMethod (){
            void* entryMethod = dlsym(_handle,"init");
            const char* dler = dlerror ();
            if (dler != NULL)
                cout << "(core) [Plugins] Error finding init() in '" << this->getLibraryPath () << "'; " << dler << endl;
            /*else
                (entryMethod)();*/
        }

        void Plugin::runCleanupMethod (){
            void* cleanMethod = dlsym(_handle,"cleanup");
            const char* dler = dlerror ();
            if (dler != NULL)
                cout << "(core) [Plugins] Error finding cleanup() in '" << this->getLibraryPath () << "'; " << dler << endl;
            /*else
                (cleanMethod)();*/
        }

        void Plugin::deinit (){
            runCleanupMethod ();
            if (_handle){
                dlclose (_handle);
                const char* dler = dlerror ();
                if (dler != NULL){
                    cout << "(core) [Plugins] Error releasing '" << this->getLibraryPath () << "'; " << dler << endl;
                    return;
                }
            }

            /// @todo Remove this plugin from the master factory plugin list.
        }

        const Plugin* Plugin::fromPath(const string& p_plgnPth){
            return new Plugin(p_plgnPth);
        }
    }
}
