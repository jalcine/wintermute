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
        PluginVector Factory::s_allPlgns;

        /// @todo Load the plugins designated to be loaded.
        void Factory::Startup () {
            //cout << "(core) [Factory] Starting up..." << endl;
            QPluginLoader m("/usr/lib/libwntrdata.so");
            m.load();
        }

        /// @todo Unload every loaded plugin and free all resources.
        void Factory::Shutdown () {
            //cout << "(core) [Factory] Shutting down..." << endl;
        }

        Plugin::Plugin() : m_plgnLdr((new QPluginLoader)) { }
        Plugin::~Plugin() {
            unload();
        }

        Plugin::Plugin(const Plugin &p_plgn) : m_plgnLdr(p_plgn.m_plgnLdr) { }
        Plugin::Plugin(const string &p_str) : m_plgnLdr((new QPluginLoader)) {
            this->setPath(p_str);
        }

        void Plugin::setPath(const string& p_str){ m_plgnLdr->setFileName(p_str.c_str ()); }
        const string Plugin::getPath() const { return m_plgnLdr->fileName().toStdString(); }
        void Plugin::load() {}
        void Plugin::unload() {}
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;
