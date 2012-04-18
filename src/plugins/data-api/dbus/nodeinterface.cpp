/***
 *  This file is part of the Wintermute project.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
 *
 *  Wintermute is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  Wintermute is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with Wintermute.
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 04/18/12 4:31:38 PM
 */

#include <app/core.hpp>
#include <app/ipc.hpp>

#include "data-api/config.hpp"
#include "data-api/dbus/nodeinterface.hpp"

using namespace Wintermute;
using namespace Wintermute::Data::Linguistics;
using Wintermute::Data::NodeInterface;

NodeInterface::NodeInterface()
    : QDBusAbstractInterface (WNTRDATA_DBUS_SERVICE, "/Nodes", staticInterfaceName(), *IPC::System::bus(), Core::instance())
{
}

const char* NodeInterface::staticInterfaceName()
{
    return "org.thesii.Wintermute.Data.Nodes";
}

QDBusPendingReply< bool > NodeInterface::exists (Lexical::Data in0)
{
    QList<QVariant> argumentList;
    argumentList << in0.toJson();

    if (in0.id() == "d41d8cd98f00b204e9800998ecf8427e")
        qFatal ("WOAH BUDDY!");

    return asyncCallWithArgumentList (QLatin1String ("exists"), argumentList);
}

QDBusPendingReply< bool > NodeInterface::isPseudo (Lexical::Data in0)
{
    QList<QVariant> argumentList;
    argumentList << in0.toJson();
    return asyncCallWithArgumentList (QLatin1String ("isPseudo"), argumentList);
}

QDBusPendingReply< Lexical::Data > NodeInterface::pseudo (Lexical::Data in0)
{
    QList<QVariant> argumentList;
    argumentList << in0.toJson();
    return asyncCallWithArgumentList (QLatin1String ("pseudo"), argumentList);
}

QDBusPendingReply< void > NodeInterface::quit()
{
    QList<QVariant> argumentList;
    return asyncCallWithArgumentList (QLatin1String ("quit"), argumentList);
}

QDBusPendingReply< Lexical::Data > NodeInterface::read (Lexical::Data in0)
{
    QList<QVariant> argumentList;
    argumentList << in0.toJson();
    return asyncCallWithArgumentList (QLatin1String ("read"), argumentList);
}

QDBusPendingReply< Lexical::Data > NodeInterface::write (Lexical::Data in0)
{
    QList<QVariant> argumentList;
    argumentList << in0.toJson();
    return asyncCallWithArgumentList (QLatin1String ("write"), argumentList);
}

void NodeInterface::generate()
{
    QList<QVariant> argumentList;
    callWithArgumentList (QDBus::NoBlock, QLatin1String ("generate"), argumentList);
}

NodeInterface::~NodeInterface() { }


#include "dbus/nodeinterface.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
