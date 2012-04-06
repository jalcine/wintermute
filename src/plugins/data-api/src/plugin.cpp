/*
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
 */
/**
 * @file linguistics.hpp
 * @author: Wintermute Development <wntr-devel@thesii.org>
 * @date March 29, 2011, 3:08 PM
 */

// Qt includes
#include <QtPlugin>

// Wintermute includes
#include <ipc.hpp>

// local includes
#include "plugin.hpp"
#include "system.hpp"
#include "dbus/adaptors.hpp"

using namespace Wintermute::Data;

void Plugin::start () const
{
    connect (this, SIGNAL (started()), System::instance (), SLOT (start()));
    connect (this, SIGNAL (stopped()), System::instance (), SLOT (stop()));

    Wintermute::IPC::System::registerObject ("/Nodes"  , new NodeAdaptor);
    Wintermute::IPC::System::registerObject ("/Syntax"  , new SyntaxAdaptor);
    Wintermute::IPC::System::registerObject ("/System" , new SystemAdaptor);
}

void Plugin::stop () const { }

Q_EXPORT_PLUGIN2 (wplugin - data, Wintermute::Data::Plugin)

#include "plugin.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
