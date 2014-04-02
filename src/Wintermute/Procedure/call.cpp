/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2011 - 2013 Jacky Alciné <me@jalcine.me>
 *
 * Wintermute is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <qjson/serializer.h>
#include <qjson/parser.h>
#include "Wintermute/logging.hpp"
#include "Wintermute/application.hpp"
#include "Wintermute/Procedure/call.hpp"
#include "Wintermute/private/Procedure/call.hpp"
#include "Wintermute/Procedure/reply_call.hpp"
#include "Wintermute/Procedure/module.hpp"

using Wintermute::Procedure::Call;
using Wintermute::Procedure::Module;
using Wintermute::Procedure::CallPrivate;

bool wCallCheckFlag ( const Call& call, const Call::Types& flag )
{
  return call.type().testFlag ( flag );
}


Call::Call ( QObject* parent ) : QObject ( parent ), 
  d ( new CallPrivate (this) )
{
}

Call::Call ( const CallPrivate::Pointer &other_d ) : 
  QObject ( Wintermute::Application::instance() ), d ( other_d )
{
  d.detach();
  d->q_ptr = this;
}

Call::Call ( const Call& other ) : 
  QObject ( other.parent() ), d ( other.d )
{
  d.detach();
  d->q_ptr = this;
}

bool
Call::isValid() const { return d->isValid(); }

void
Call::setRecipient ( const QString& moduleName )
{
  d->recipient = moduleName;
}

Call::Type
Call::type() const
{
  return d->type;
}

QString
Call::name() const
{
  return d->name;
}

QString
Call::recipient() const
{
  return d->recipient;
}

quint64
Call::id() const
{
  return d->id.toTime_t();
}

QString
Call::toString() const
{
  Q_ASSERT ( isValid() );
  bool ok;
  QJson::Serializer serializer;
  QVariantMap callData = d->toVariantMap();
  QString json = serializer.serialize ( callData, &ok );
  return ( ok ? json : QString::null );
}

Call*
Call::fromString ( const QString& data )
{
  QJson::Parser parser;
  bool ok;
  QVariantMap callData = parser.parse ( data.toLocal8Bit(), &ok ).toMap();

  if ( !ok )
  {
    werr ( wntrApp, QString("Failed to convert '%1' into a Call.").arg(data) );
    return nullptr;
  }

  CallPrivate::Pointer d_ptr = CallPrivate::fromVariantMap(callData);
  return new Call ( d_ptr );
}

Call::~Call()
{
}
