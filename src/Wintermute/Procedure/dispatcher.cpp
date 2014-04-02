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

#include <QtCore/QCoreApplication>
#include "Wintermute/private/Procedure/dispatcher.hpp"
#include "Wintermute/Events/call.hpp"
#include "Wintermute/application.hpp"
#include "Wintermute/Procedure/method_call.hpp"
#include "Wintermute/logging.hpp"

using Wintermute::Procedure::Dispatcher;
using Wintermute::Procedure::DispatcherPrivate;
using Wintermute::Events::CallEvent;

QList<Dispatcher*> DispatcherPrivate::dispatchers = QList<Dispatcher*>();

Dispatcher::Dispatcher() :
  QObject ( wntrApp )
{
  DispatcherPrivate::addDispatcher ( this );
}

void
Dispatcher::postDispatch ( const MethodCall& call, Module* object )
{
  Q_ASSERT ( call.isValid() );
  QCoreApplication::postEvent ( object, new CallEvent ( CallEvent::TypeDispatch, call ) );
}

Dispatcher::~Dispatcher()
{
  DispatcherPrivate::removeDispatcher ( this );
}
