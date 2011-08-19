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

#include <QtDebug>
#include "plugins.hpp"

using namespace std;

namespace Wintermute {
    namespace Plugins {
        PluginVector Factory::s_allPlgns;
        typedef void (*VoidFunction)(void);

        /// @todo Load the plugins designated to be loaded.
        void Factory::Startup () {
            qDebug() << "(core) [Factory] Starting up...";
            Factory::loadPlugin ("/usr/lib/libwntrgui.so");
        }

        /// @todo Unload every loaded plugin and free all resources.
        void Factory::Shutdown () {
            qDebug() << "(core) [Factory] Shutting down...";
        }

        const PluginBase* Factory::loadPlugin (const string &p_pth) {
            return loadPlugin((new QFile(QString::fromStdString (p_pth))));
        }

        const PluginBase* Factory::loadPlugin (const QFile *p_fl){
            if (!p_fl->exists ()){
                qWarning() << "(core) [Factory] Failed to load " << p_fl->fileName () << p_fl->errorString ();
                return NULL;
            }

            QPluginLoader* l_plgnLdr = new QPluginLoader(p_fl->fileName ());
            l_plgnLdr->setLoadHints (QLibrary::ExportExternalSymbolsHint | QLibrary::ResolveAllSymbolsHint);
            if (l_plgnLdr->load ()){
                PluginBase* l_plgnBase = qobject_cast<PluginBase*>(l_plgnLdr->instance ());
                //l_plgnBase->m_plgnLdr = l_plgnLdr;
                l_plgnBase->initialize ();
                Factory::s_allPlgns.push_back (l_plgnBase);
                return l_plgnBase;
            } else {
                qWarning() << "(core) [Factory] Err:" << l_plgnLdr->errorString ();
                return NULL;
            }
        }

        PluginBase::PluginBase() : m_plgnLdr(NULL) {

        }

        PluginBase::PluginBase(const PluginBase &p_1) : m_plgnLdr(p_1.m_plgnLdr) {

        }

        bool PluginBase::operator == (const PluginBase &p_1) {
            return p_1.m_plgnLdr == m_plgnLdr;
        }

        const bool PluginBase::isSupported () const {
            return version () >= WINTERMUTE_VERSION;
        }

        const QString PluginBase::path () const {
            return QString::null;
        }
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;
