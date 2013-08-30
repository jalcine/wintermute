/**
 * vim: ft=qt.cpp
 * Copyright (C) 2013 Jacky Alcine <me@jalcine.me>
 *
 * This file is part of Wintermute, the extensible AI platform.
 *
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

#include "Wintermute/Procedure/method_call.hpp"
#include "Wintermute/private/Procedure/method_call.hpp"

using Wintermute::Procedure::MethodCall;
using Wintermute::Procedure::Call;

MethodCall::MethodCall ( const QString& module, const QString& method, const QVariantList arguments ) :
  Call ( new MethodCallPrivate ( this ) )
{
  Q_D ( MethodCall );
  d->module = module;
  d->method = method;
  d->arguments = arguments;
}

QString
MethodCall::name() const
{
  Q_D ( const MethodCall );
  return d->method;
}

MethodCall::~MethodCall()
{
}

#include "Wintermute/Procedure/method_call.moc"
