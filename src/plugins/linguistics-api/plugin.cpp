/**
 * @file wntrling.hpp
 * This file is part of Wintermute Linguistics
 *
 * Copyright (C) 2011 - Wintermute Development <wntr-devel@thesii.org>
 *
 * Wintermute Linguistics is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute Linguistics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute Linguistics; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#include <QDebug>
#include <QtPlugin>
#include <app/ipc.hpp>
#include "linguistics-api/plugin.hpp"
#include "linguistics-api/system.hpp"
#include "linguistics-api/dbus/adaptors.hpp"

using namespace Wintermute;
using Wintermute::Linguistics::Plugin;
using Wintermute::Linguistics::System;

void Plugin::start () const
{
    connect (this, SIGNAL (started()), System::instance (), SLOT (start()));
    connect (this, SIGNAL (stopped()), System::instance (), SLOT (stop()));

    Linguistics::SystemAdaptor* adpt = new Linguistics::SystemAdaptor;

    if (!Wintermute::IPC::System::registerObject ("/System" , adpt))
        qDebug() << "(ling) [Plugin] Failed to register System D-Bus object.";

    System::tellSystem ("I am Wintermute.");
}

void Plugin::stop () const
{
}

QObject* Plugin::instance () const
{
    return System::instance();
}

Plugin::~Plugin()
{

}

Q_EXPORT_PLUGIN2 (wplugin - linguistics, Wintermute::Linguistics::Plugin)
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
