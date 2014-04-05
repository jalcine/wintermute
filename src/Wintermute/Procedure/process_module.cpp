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
#include <QtCore/QProcess>
#include <Wintermute/Globals>
#include <Wintermute/Procedure/MethodCall>
#include "Wintermute/logging.hpp"
#include "Wintermute/Procedure/process_module.hpp"

using Wintermute::Procedure::ProcessModule;
using Wintermute::Procedure::MethodCall;

ProcessModule::ProcessModule() :
  Module ( Wintermute::Application::instance() )
{
  setDomain  ( WINTERMUTE_DOMAIN );
  setPackage ( "process" );
  mountCall ( new LambdaCall ( "stop", this,
  [&] (QVariantList args, const MethodCall& call) -> QVariant {
    wdebug ( this, "Remote stop initiated." );
    stop ();
    return true;
  } ) );
  mountCall ( new LambdaCall ( "quit", this,
  [&] (QVariantList args, const MethodCall& call) -> QVariant {
    wdebug ( this, "Remote quit initiated." );
    quit ( args[0].toInt() );
    return true;
  } ) );
  mountCall ( new LambdaCall ( "reboot", this,
  [&] (QVariantList args, const MethodCall& call) -> QVariant {
    wdebug ( this, "Remote reboot initiated." );
    quit ( args[0].toInt() );
    return true;
  } ) );
}

void
ProcessModule::greetSystem ( const QString &name )
{
  QPointer<Module> module = Module::findModule (name);
  const QVariantList args = QVariantList() << module->qualifiedName() <<
                            QCoreApplication::applicationPid();
  const MethodCall methodCall ( WINTERMUTE_DOMAIN".heartbeat.monitor", "greet",
                                args );
  methodCall.dispatch ();
}

void
ProcessModule::start()
{
  connect ( wntrApp, SIGNAL ( started() ), SLOT ( start() ) );
  winfo ( this, QString ( "Currently %1 modules loaded so far." ).arg (
            Module::knownModules().length() ) );
}

void
ProcessModule::reboot()
{
  QProcess::startDetached ( QCoreApplication::applicationFilePath(),
                            QCoreApplication::arguments() );
  quit ( 0 );
}

void
ProcessModule::stop()
{
  winfo ( this, QString ("Stopping Wintermute...") );
  quit ();
}

void
ProcessModule::quit ( const int exitcode )
{
  winfo ( this, "Invoking remote exit..." );
  wntrApp->stop ( exitcode );
}

ProcessModule::~ProcessModule()
{
  if ( !QCoreApplication::closingDown() ) {
    stop();
  }
}
