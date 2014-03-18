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

#include <TTCCLayout>
#include <ColorConsoleAppender>
#include <SignalAppender>
#include <FileAppender>
#include <MainthreadAppender>
#include <LogManager>
#include <QtCore/QCoreApplication>

using Log4Qt::LogManager;
using Log4Qt::Logger;
using Log4Qt::Layout;
using Log4Qt::TTCCLayout;
using Log4Qt::ConsoleAppender;
using Log4Qt::FileAppender;
using Log4Qt::MainThreadAppender;
using Log4Qt::ColorConsoleAppender;

namespace Wintermute
{
class LoggingPrivate
{
public:
  Layout* primaryLayout;
  ColorConsoleAppender* stdOutAppender;
  ColorConsoleAppender* stdErrAppender;
  FileAppender* fileAppender;
  MainThreadAppender* mainThreadAppender;

  LoggingPrivate() :
    primaryLayout ( 0 ), stdOutAppender ( 0 ), stdErrAppender ( 0 ) ,
    fileAppender ( 0 )
  {
    LogManager::startup();
    LogManager::handleQtMessages();
#ifdef WINTERMUTE_DEBUG 
    LogManager::setThreshold ( Log4Qt::Level::INFO_INT );
#else
    LogManager::setThreshold ( Log4Qt::Level::WARN_INT );
#endif
    primaryLayout  = new TTCCLayout( "ABSOLUTE" );
    primaryLayout->setName ( "root" );
    ((TTCCLayout*) primaryLayout)->setThreadPrinting ( true );
    ((TTCCLayout*) primaryLayout)->setContextPrinting ( false );
    primaryLayout->activateOptions();
    addAppenders();
  }

  void
  addAppenders()
  {
    stdOutAppender = new ColorConsoleAppender ( primaryLayout,
        ConsoleAppender::STDOUT_TARGET );
    stdOutAppender->setName ( "stdout" );
    stdOutAppender->activateOptions();

    stdErrAppender = new ColorConsoleAppender ( primaryLayout,
        ConsoleAppender::STDERR_TARGET );
    stdErrAppender->setName ( "stderr" );
    stdErrAppender->activateOptions();

    fileAppender = new FileAppender ( primaryLayout, "wintermute.log",
        true, true, QCoreApplication::instance() );
    fileAppender->setName( "file" );
    fileAppender->activateOptions();

    mainThreadAppender = new MainThreadAppender ( );

    Logger::rootLogger()->addAppender ( stdOutAppender );
    Logger::rootLogger()->addAppender ( fileAppender );
  }

  virtual ~LoggingPrivate() {
    LogManager::shutdown();
  }
};
}
