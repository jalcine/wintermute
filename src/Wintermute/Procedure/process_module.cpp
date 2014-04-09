/**
 * @author Jacky Alciné <me@jalcine.me>
 * @copyright © 2011, 2012, 2013, 2014 Jacky Alciné <me@jalcine.me>
 * @if 0
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
 * @endif
 **/

#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>
#include <Wintermute/Globals>
#include <Wintermute/Logging>
#include "lambda_call.hpp"
#include "method_call.hpp"
#include "reply_call.hpp"
#include "process_module.hpp"

using Wintermute::Procedure::Module;
using Wintermute::Procedure::ProcessModule;
using Wintermute::Procedure::MethodCall;
using Wintermute::Procedure::ReplyCall;
using Wintermute::Procedure::LambdaCall;

ProcessModule::ProcessModule() :
  Module ( Wintermute::Application::instance() )
{
  setDefinition(WINTERMUTE_DOMAIN, "process");
  mountCall( new LambdaCall("stop", this,
  [&] (QVariant args, const MethodCall & call) -> void {
    winfo(this, QString("Remote stop initiated; %1.").arg(args.toString()));
    ReplyCall reply = call.craftReply(true);
    reply.queueForDispatch();
    stop ();
  } ) );
  mountCall(new LambdaCall("quit", this,
  [&] (QVariant args, const MethodCall & call) -> void {
    winfo( this, "Remote quit initiated." );
    ReplyCall reply = call.craftReply(true);
    reply.queueForDispatch();
    quit ( args.toInt() );
  } ) );
  mountCall(new LambdaCall("reboot", this,
  [&] (QVariant , const MethodCall & call) -> void {
    winfo( this, "Remote reboot initiated." );
    ReplyCall reply = call.craftReply(true);
    reply.queueForDispatch();
    reboot();
  } ) );
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
  wtrace(this, "Invoking reboot...");
  QProcess::startDetached ( QCoreApplication::applicationFilePath(),
                            QCoreApplication::arguments() );
  quit ( 0 );
}

void
ProcessModule::stop()
{
  wtrace ( this, QString ("Stopping Wintermute...") );
  quit ();
}

void
ProcessModule::quit ( const int exitcode )
{
  wtrace ( this, "Invoking remote exit..." );
  wntrApp->stop ( exitcode );
}

ProcessModule::~ProcessModule()
{
  if ( !QCoreApplication::closingDown() ) {
    wtrace(this, "Attempting to quit the process...");
    stop();
  } else {
    wtrace(this, "Already dying, not bothering to kill process.");
  }
}
