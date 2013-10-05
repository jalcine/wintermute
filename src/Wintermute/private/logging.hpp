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
#include <LogManager>

using Log4Qt::LogManager;
using Log4Qt::Logger;
using Log4Qt::Layout;
using Log4Qt::TTCCLayout;
using Log4Qt::ConsoleAppender;
using Log4Qt::ColorConsoleAppender;

namespace Wintermute
{
class LoggingPrivate
{
public:
  Layout* primaryLayout;
  ColorConsoleAppender* stdOutAppender;
  ColorConsoleAppender* stdErrAppender;

  LoggingPrivate() :
    primaryLayout ( 0 ), stdOutAppender ( 0 ), stdErrAppender ( 0 )
  {
    LogManager::startup();
    primaryLayout  = new TTCCLayout();
    primaryLayout->setName ( "root" );
    primaryLayout->activateOptions();

    stdOutAppender = new ColorConsoleAppender ( primaryLayout, ConsoleAppender::STDOUT_TARGET);
    stdOutAppender->setName ( "stdout" );
    stdOutAppender->activateOptions();
    Logger::rootLogger()->addAppender ( stdOutAppender );
  }

  virtual ~LoggingPrivate()
  {
    LogManager::shutdown();
  }
};
}
