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
 *
 */

/**
 * @file adaptors.hpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 * @date Sun, 30 Oct 2011 21:54:16
 */

#include <app/ipc.hpp>
#include <app/core.hpp>

#include "data-api/config.hpp"
#include "syntaxinterface.hpp"

using Wintermute::Core;
using Wintermute::Data::SyntaxInterface;
using namespace Wintermute::Data::Linguistics;

SyntaxInterface::SyntaxInterface()
    : QDBusAbstractInterface (WNTRDATA_DBUS_SERVICE, "/Syntax", staticInterfaceName(), *IPC::System::bus(), Core::instance())
{
}

QDBusPendingReply< bool > SyntaxInterface::exists (const QString& in0, const QString& in1)
{
    QList<QVariant> argumentList;
    qDebug() << "(data) [SyntaxInterface::exists()]" << in0 << in1;
    argumentList << qVariantFromValue (in0) << qVariantFromValue (in1);
    return asyncCallWithArgumentList (QLatin1String ("exists"), argumentList);
}

QDBusPendingReply< void > SyntaxInterface::quit()
{
    QList<QVariant> argumentList;
    return asyncCallWithArgumentList (QLatin1String ("quit"), argumentList);
}

QDBusPendingReply< Syntax::Chain > SyntaxInterface::read (Syntax::Chain in0)
{
    QList<QVariant> argumentList;
    qDebug() << "(data) [SyntaxInterface::read()]" << in0.toString();
    argumentList << in0.toString();
    return asyncCallWithArgumentList (QLatin1String ("read"), argumentList);
}


QDBusPendingReply< Syntax::Chain > SyntaxInterface::write (Syntax::Chain in0)
{
    QList<QVariant> argumentList;
    qDebug() << "(data) [SyntaxInterface::write()]" << in0.toString();
    argumentList << in0.toString();
    return asyncCallWithArgumentList (QLatin1String ("write"), argumentList);
}

SyntaxInterface::~SyntaxInterface() { }

#include "dbus/syntaxinterface.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
