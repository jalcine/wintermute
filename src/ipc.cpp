/**
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
 * @file   ipc.cpp
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

#include "ipc.hpp"
#include "core.hpp"
#include "config.hpp"
#include "plugins.hpp"
#include "adaptors.hpp"
#include <QtDBus>
#include <QProcess>

namespace Wintermute {
    namespace IPC {
        QString System::s_appMod;
        QDBusConnection* System::s_cnntn = NULL;
        Adaptor* System::s_adapt = NULL;

        void System::start ( ) {
            registerDataTypes();
            s_cnntn = new QDBusConnection(QDBusConnection::sessionBus ().connectToBus (QDBusConnection::SessionBus,"Wintermute"));
            s_appMod = Core::arguments ()->value ("ipc").toString ().toLower ();
            QString l_serviceName = WINTERMUTE_SERVICE_NAME, l_objectName;

            if ( s_appMod == "master") {
                l_objectName = "master";
                connect(Core::instance (),SIGNAL(started()), Plugins::Factory::instance(),SLOT(Startup()));
                connect(Core::instance (),SIGNAL(stopped()),Plugins::Factory::instance (),SLOT(Shutdown()));

                CoreAdaptor* l_adpt = new CoreAdaptor;
                Plugins::PluginFactoryAdaptor* l_adpt2 = new Plugins::PluginFactoryAdaptor;

                registerObject ("/Master" , l_adpt);
                registerObject ("/Factory" , l_adpt2);
                s_adapt = l_adpt;
            } else if ( s_appMod == "plugin" ) {
                const QString l_plgn = Core::arguments ()->value ("plugin").toString ();
                l_objectName = "Plugin." + l_plgn;
                connect(Core::instance (),SIGNAL(started()),Plugins::Factory::instance (),SLOT(loadStandardPlugin()));
                connect(Core::instance (),SIGNAL(stopped()),Plugins::Factory::instance (),SLOT(unloadStandardPlugin()));
            }

            if (l_objectName != "master") l_serviceName += "." + l_objectName;

            if (!s_cnntn->registerService (l_serviceName) && l_objectName == "master"){
                qDebug() << "(core) Fatal: Cannot run more than one Wintermute service (" << l_serviceName
                         << ") under the same user on the same computer";
                Core::endProgram ();
            }

            qDebug() << "(core) [D-Bus] Service" << l_serviceName << "running.";
        }

        const bool System::registerObject(const QString& p_pth, Adaptor* p_obj){
            QDBusConnection::RegisterOptions l_opts = QDBusConnection::ExportAllContents
                    | QDBusConnection::ExportAllSignals
                    | QDBusConnection::ExportAllSlots
                    | QDBusConnection::ExportAllProperties
                    | QDBusConnection::ExportAllInvokables
                    | QDBusConnection::ExportChildObjects
                    | QDBusConnection::ExportAdaptors;

            if (s_cnntn->objectRegisteredAt (p_pth)){
                qDebug() << "(core) [D-Bus] Object" << p_pth << "already registered on" << s_cnntn->interface ()->service ();
                return false;
            } else
                return s_cnntn->registerObject (p_pth , p_obj, l_opts);

            return false;
        }

        void System::stop () { s_cnntn->disconnectFromBus (s_cnntn->name ()); }

        QDBusConnection* System::bus() { return s_cnntn; }

        Adaptor* System::adaptor () { return s_adapt; }
    }
}

// kate: indent-mode cstyle; space-indent on; indent-width 4;
