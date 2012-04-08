/**
 * @author Wintermute Development <wntr-devel@thesii.org>
 * @file   ipc.cpp
 *
 * @section lcns Licensing
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
 *
 */

// Local
#include "ipc.hpp"
#include "core.hpp"
#include "config.hpp"
#include "adaptors.hpp"
#include "plugins.hpp"
#include "factory.hpp"

// Qt
#include <QtDBus>
#include <QProcess>

namespace Wintermute
{
namespace IPC
{
System* System::s_inst = new System;

System::System (QObject* p_obj) : QObject (p_obj) { }

System::~System() { }

void System::start()
{
    instance()->m_cnntn = new QDBusConnection (QDBusConnection::sessionBus().connectToBus (QDBusConnection::SessionBus, "Wintermute"));
    instance()->m_appMod = Core::arguments()->value ("ipc").toString().toLower();
    QString serviceName = WNTR_DBUS_SERVICE_NAME,
            objectName;
    qDebug() << "(core) [IPC::System]" << "Mode:" << instance()->m_appMod;

    if (instance()->m_appMod == "master") {
        objectName = "master";
        connect (Core::instance(), SIGNAL (started()), Plugins::Factory::instance(), SLOT (Startup()));
        connect (Core::instance(), SIGNAL (stopped()), Plugins::Factory::instance(), SLOT (Shutdown()));

        CoreAdaptor* coreAdaptor = new CoreAdaptor;
        Plugins::PluginFactoryAdaptor* pluginFactoryAdaptor = new Plugins::PluginFactoryAdaptor;

        registerObject ("/Master", coreAdaptor);
        registerObject ("/Factory", pluginFactoryAdaptor);
        instance()->m_adapt = coreAdaptor;
    }
    else if (instance()->m_appMod == "plugin") {
        const QString plgn = Core::arguments()->value ("plugin").toString();
        objectName = "Plugin." + plgn;
        connect (Core::instance(), SIGNAL (started()), Plugins::Factory::instance(), SLOT (loadStandardPlugin()));
        connect (Core::instance(), SIGNAL (stopped()), Plugins::Factory::instance(), SLOT (unloadStandardPlugin()));
    }

    if (objectName != "master") serviceName += "." + objectName;

    bool serviceRegistered = false;

    if (instance()->m_cnntn->registerService (serviceName))
        serviceRegistered = true;

    if (!serviceRegistered && objectName == "master") {
        qDebug() << "(core) Fatal: Cannot run more than one Wintermute service (" << serviceName << ")"
                 << "under the same user on the same computer";
        Core::exit (1);
    }

    if (serviceRegistered)
        qDebug() << "(core) [D-Bus] Service" << serviceName << "running.";

    emit instance()->registerDataTypes();
}

/// @todo Find a way to expose adaptors _properly_ over D-Bus.
bool System::registerObject (const QString& p_pth, QDBusAbstractAdaptor* p_obj)
{
    QDBusConnection::RegisterOptions opts = QDBusConnection::ExportAllContents
                                            | QDBusConnection::ExportAllSignals
                                            | QDBusConnection::ExportAllSlots
                                            | QDBusConnection::ExportAllProperties
                                            | QDBusConnection::ExportAllInvokables
                                            | QDBusConnection::ExportChildObjects
                                            | QDBusConnection::ExportAdaptors;

    if (instance()->m_cnntn->objectRegisteredAt (p_pth)) {
        qDebug() << "(core) [D-Bus] Object" << p_pth << "already registered on" << instance()->m_cnntn->interface ()->service ();
        return false;
    }
    else {
        qDebug() << "(core) [D-Bus] Registered" << p_pth << "on" << instance()->m_cnntn->interface ()->connection().name() << "with" << p_obj->metaObject()->className() << ".";
        return instance()->m_cnntn->registerObject (p_pth , p_obj, opts);
    }

    return false;
}

void System::stop ()
{
    instance()->m_cnntn->disconnectFromBus (instance()->m_cnntn->name());
}

QDBusConnection* System::bus()
{
    return instance()->m_cnntn;
}

AbstractAdaptor* System::adaptor ()
{
    return instance()->m_adapt;
}

System* System::instance()
{
    return s_inst;
}
}
}

#include "ipc.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
