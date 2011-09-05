/**
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
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
 *
 * @todo Perhaps implement a means of upgrading old plug-in definition files?
 */

#include <QDir>
#include <QtDBus>
#include <QtDebug>
#include <QProcess>
#include "config.hpp"
#include "plugins.hpp"
#include "core.hpp"

using namespace std;

namespace Wintermute {
    namespace Plugins {
        PluginList Factory::s_allPlgns;
        Factory* Factory::s_factory = new Factory;

        void Factory::Startup () {
            qDebug() << "(core) [Factory] Starting up...";
            QDir *d = new QDir ( WINTER_PLUGINSPEC_PATH );
            d->setFilter ( QDir::Files | QDir::Readable | QDir::NoSymLinks );
            d->setNameFilters (QString("*.spec").split (" "));
            d->setSorting ( QDir::Name );

            QStringList l_plgnSpecLst = d->entryList();
            foreach ( const QString l_plgnSpecFile, l_plgnSpecLst ){
                const QString l_plgnName = l_plgnSpecFile.split (".").at (0);
                Factory::loadPlugin (l_plgnName);
            }
        }

        /// @todo Monitor plug-in status when the program loads and it connects to D-Bus.
        const PluginBase* Factory::loadPlugin ( const QString &p_plgName ) {
            WNTR_APPLICATION::addLibraryPath (WINTER_PLUGIN_PATH);
            if (Core::arguments ()->value ("plugin").toString () != "root"){
                const QString l_plgnSpecPath = QString(WINTER_PLUGINSPEC_PATH) + "/" + p_plgName + ".spec";
                const QString l_plgPth = QString(WINTER_PLUGIN_PATH) + "/lib" + p_plgName + ".so";

                if (!QFile::exists (l_plgnSpecPath)){
                    qWarning() << "(core) [Factory] Failed to load " << l_plgnSpecPath;
                    return NULL;
                }

                QSettings* l_plgnSpec = new QSettings(l_plgnSpecPath,QSettings::IniFormat);
                QPluginLoader* l_plgnLdr = new QPluginLoader ( l_plgPth );
                l_plgnLdr->setLoadHints ( QLibrary::ExportExternalSymbolsHint | QLibrary::ResolveAllSymbolsHint );
                if ( l_plgnLdr->load () ) {
                    PluginBase* l_plgnBase = dynamic_cast<PluginBase*> ( l_plgnLdr->instance () );
                    l_plgnBase->m_plgnLdr = l_plgnLdr;
                    l_plgnBase->m_settings = l_plgnSpec;

                    if ( !l_plgnBase->isSupported () ) {
                        qWarning() << "(core) [Factory] The plugin" << l_plgnBase->name () << "is incompatiable with this version of Wintermute.";
                        return NULL;
                    } else
                        qDebug() << "(core) [Factory] Plugin" << l_plgnBase->name () << "v." << l_plgnBase->version() << "loaded.";

                    qDebug() << "(core) [Factory] {plug-in:" << l_plgnBase->uuid () << "}: Dependency checking skipped.";

                    l_plgnBase->doInitialize ();
                    QObject::connect ( Core::instance (),SIGNAL ( deinitialized() ),l_plgnBase,SLOT ( doDeinitialize() ) );

                    Factory::s_allPlgns.insert ( l_plgnBase->uuid (),l_plgnBase );
                    return l_plgnBase;
                } else {
                    qWarning() << "(core) [Factory] Err loading plugin (" << l_plgnLdr->fileName () << "):" << l_plgnLdr->errorString ();
                    return NULL;
                }


            } else {
                const QStringList l_plgnArgs = QString("--ipc plugin --plugin " + p_plgName).split(" ");
                QProcess::startDetached(WNTR_APPLICATION::applicationFilePath (),l_plgnArgs);
            }
        }

        void Factory::unloadPlugin ( const QString& p_uuid ) {
            PluginBase* l_plgn = Factory::s_allPlgns.take ( p_uuid );
            if ( l_plgn )
                delete l_plgn;
        }

        void Factory::loadStandardPlugin () {
            loadPlugin(Core::arguments ()->value ("plugin").toString ());
        }

        void Factory::unloadStandardPlugin () {
            unloadPlugin(Core::arguments ()->value ("plugin").toString ());
        }

        /// @todo Unload every loaded plugin and free all resources.
        void Factory::Shutdown () {
            qDebug() << "(core) [Factory] Unloading plugins..";
            foreach ( PluginBase* l_plgnBs, s_allPlgns ) {
                qDebug() << "(core) [Factory] Unloaded" << l_plgnBs->name () << ".";
                l_plgnBs->deinitialize ();
            }
        }

        const QString PluginBase::author () const { return m_settings->value ("Description/Author").toString (); }

        const QString PluginBase::name () const { return m_settings->value ("Description/Name").toString (); }

        const QString PluginBase::vendorName () const { return m_settings->value ("Description/Vendor").toString (); }

        const QString PluginBase::uuid () const { return m_settings->value ("Version/UUID").toString (); }

        const QString PluginBase::description () const { return m_settings->value ("Description/Blurb").toString (); }

        const QString PluginBase::webPage () const { return m_settings->value ("Description/WebPage").toString (); }

        const QStringList PluginBase::dependencies () const {
            QStringList l_dep = m_settings->value ("Dependencies/Plugins").toStringList ();
            l_dep.append (m_settings->value ("Dependencies/Packages").toStringList ());
            return l_dep;
        }

        const double PluginBase::version () const { return m_settings->value ("Version/Plugin").toDouble (); }

        const double PluginBase::compatVersion () const { return m_settings->value ("Version/Compat").toDouble (); }

        const bool PluginBase::isSupported () const { return WINTERMUTE_VERSION >= compatVersion (); }

        const QString PluginBase::path () const { return m_plgnLdr->fileName (); }

        void PluginBase::doDeinitialize () const {
            qDebug() << "(core) [Factory] {plug-in:" << this->uuid () << "} Deinitializing..";
            emit deinitializing ();
            deinitialize ();
            Factory::unloadPlugin(uuid());
        }

        void PluginBase::doInitialize() const {
            qDebug() << "(core) [Factory] {plug-in:" << this->uuid () << "} Initializing..";
            emit initializing ();
            initialize ();
        }
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;
