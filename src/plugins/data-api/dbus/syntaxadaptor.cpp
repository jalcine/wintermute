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

#include "data-api/syntax/manager.hpp"
#include "data-api/wntrdata.hpp"
#include "syntaxadaptor.hpp"

using namespace Wintermute::Data;
using namespace Wintermute::Data::Linguistics;
using Wintermute::Data::Linguistics::Syntax::Manager;

SyntaxAdaptor::SyntaxAdaptor()
    : QDBusAbstractAdaptor (Manager::instance())
{
    setAutoRelaySignals (true);
}

SyntaxAdaptor::~SyntaxAdaptor() { }

bool SyntaxAdaptor::exists (const QString& in0, const QString& in1)
{
    bool out0;
    QMetaObject::invokeMethod (parent(), "exists", Q_RETURN_ARG (bool, out0), Q_ARG (QString, in0), Q_ARG (QString, in1));
    return out0;
}

void SyntaxAdaptor::quit()
{
    QMetaObject::invokeMethod (parent(), "quit");
}

QString SyntaxAdaptor::read (QString in0)
{
    Syntax::Chain chn = Syntax::Chain::fromString (in0);
    Manager::instance()->read (chn);
    return chn.toString();
}

QString SyntaxAdaptor::write (QString in0)
{
    Syntax::Chain chn = Syntax::Chain::fromString (in0);
    Manager::instance()->write (chn);
    return chn.toString();
}

#include "dbus/syntaxadaptor.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
