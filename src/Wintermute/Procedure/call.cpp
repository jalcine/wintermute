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
#include "Wintermute/Procedure/module.hpp"

using Wintermute::Procedure::Call;
using Wintermute::Procedure::Module;

Call::Call ( QObject* parent ) : 
  QObject ( parent ), d_ptr ( new CallPrivate ( this ) )
{
}

Call::Call ( CallPrivate* d ) : 
  QObject ( Wintermute::Application::instance() ), d_ptr ( d )
{
}

QVariant
Call::invoke ( const QVariantList& data )
{
  Q_D ( Call );
  return d->function ( data );
}

void
Call::setRecipient ( const QString moduleName )
{
  Q_D ( Call );
  d->recipient = QString ( moduleName );
}

Call::Type
Call::type() const
{
  Q_D ( const Call );
  return d->type;
}

QString
Call::name() const
{
  Q_D ( const Call );
  return d->name;
}

QString
Call::recipient() const
{
  Q_D ( const Call );
  return d->recipient;
}

QString
Call::toString() const
{
  Q_D ( const Call );
  QJson::Serializer serializer;
  bool ok;
  QMap<QString, QVariant> callMap;
  callMap["type"] = ( int ) type();
  callMap["recipient"] = recipient();
  callMap["data"] = d->data;
  QByteArray json = serializer.serialize ( callMap, &ok );
  if ( !ok )
    return QString::null;

  return QString ( json );
}

Call*
Call::fromString(const QString& data)
{
  QJson::Parser parser;
  bool ok;
  QVariant callData = parser.parse(data.toLocal8Bit(), &ok);
  if (!ok)
    return 0;

  QVariantMap callMap = callData.toMap();
  Call* aCall = new Call(wntrApp);
  aCall->d_ptr->type = (Call::Type) callMap["type"].toInt();
  aCall->d_ptr->recipient = callMap["recipient"].toString();
  aCall->d_ptr->data = callMap["data"].toMap();
  return aCall;
}

QVariant
Call::operator() ( const QVariantList& data )
{
  return this->invoke ( data );
}

bool
Call::attemptInvocation(const Call* call)
{
  Procedure::Module* module = wntrApp->findModule(call->recipient());

  if (!module){
    werr(staticMetaObject.className(), QString("Can't find module '%1' in this process.").arg(call->recipient()));
    return false;
  }

  QVariant result = module->invoke(call->d_ptr->data["method"].toString(), call->d_ptr->data["arguments"].toList());
  winfo(staticMetaObject.className(), result.toString());
  return true;
}

Call::~Call()
{
}

#include "Wintermute/Procedure/call.moc"
