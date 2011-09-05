/**
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
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
#include "plugins.hpp"
#include <wntrntwk.hpp>
#include <wntrdata.hpp>
#include <QtDBus>
#include <QProcess>

namespace Wintermute {
    QString IPC::s_appMod;
    DBusAdaptor* DBusAdaptor::s_dbus = NULL;

    /// @todo Determine the actions to be taken depending on the IPC module.
    void IPC::Initialize ( const QString& p_appMod ) {
        if (p_appMod.isEmpty ())
            s_appMod = "Master";
        else
            s_appMod = p_appMod;

        s_appMod[0] = s_appMod[0].toUpper ();

        if ( s_appMod == "Master") {
            Data::Linguistics::Configuration::setLocale ( Core::arguments ()->value ("locale").toString () );
            connect(Core::instance (),SIGNAL(initialized()), Data::Configuration::instance (),SLOT(Initialize()));
            connect(Core::instance (),SIGNAL(deinitialized()),Data::Configuration::instance (),SLOT(Deinitialize()));
        } else if ( s_appMod == "Network" ) {
            Network::Initialize ();
        } else if ( s_appMod == "Plugin" ) {
            const QString l_plgn = Core::arguments ()->value ("plugin").toString ();
            if (l_plgn == "root"){
                connect(Core::instance (),SIGNAL(initialized()), Plugins::Factory::instance(),SLOT(Startup()));
                connect(Core::instance (),SIGNAL(deinitialized()),Plugins::Factory::instance (),SLOT(Shutdown()));
            }
            else {
                s_appMod += QString("_") + l_plgn;
                connect(Core::instance (),SIGNAL(initialized()),Plugins::Factory::instance (),SLOT(loadStandardPlugin()));
                connect(Core::instance (),SIGNAL(initialized()),Plugins::Factory::instance (),SLOT(unloadStandardPlugin()));
            }
        }

        DBusAdaptor::s_dbus = new DBusAdaptor;
        qDebug() << "(core) [IPC] Module" << s_appMod << "running.";
        Core::Initialize ();
    }

    DBusAdaptor::DBusAdaptor() : QDBusAbstractAdaptor(WNTR_APPLICATION::instance ()) {
        connect(this,SIGNAL(destroyed()),WNTR_APPLICATION::instance (),SLOT(quit()));
        connect(WNTR_APPLICATION::instance (),SIGNAL(aboutToQuit()),this,SLOT(deleteLater()));

        if (QDBusConnection::sessionBus().registerObject("/" + IPC::currentModule (), this , QDBusConnection::ExportAllContents)){
            qDebug() << "(core) [D-Bus] Connected to D-Bus as" << IPC::currentModule ();
            emit started();

            connect(WNTR_APPLICATION::instance (),SIGNAL(aboutToQuit()),this,SLOT(deleteLater()));
            if (IPC::currentModule () != "Master"){
                m_mstrIntr = new QDBusInterface("org.thesii.Wintermute","/Master");
                m_mstrIntr->connect (m_mstrIntr,SIGNAL(destroyed()),this,SLOT(deleteLater()));
                m_mstrIntr->call ("reportAlive");
            }
        } else {
            qDebug() << "(core) [D-Bus] Can't connect to D-Bus; a module with the name" << IPC::currentModule () << "already exists!";
            WNTR_APPLICATION::quit ();
            exit(0);
        }
    }

    DBusAdaptor::~DBusAdaptor() {
        qDebug() << "(core) [D-Bus] Disconnecting...";
        emit stopped();
    }

    const QString DBusAdaptor::module() const { return IPC::currentModule (); }

    void DBusAdaptor::quit () const { WNTR_APPLICATION::quit (); }

    void DBusAdaptor::reportAlive (const QDBusMessage &p_msg) const {
        qDebug() << "(core) [D-Bus] Module "<< p_msg.interface () << "reporting for duty.";
    }
}

// kate: indent-mode cstyle; space-indent on; indent-width 4;
