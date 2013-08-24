/**
 * vim: ft=cpp
 * Copyright (C) 2013 Jacky Alciné <me@jalcine.me>
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

#include "logging.hpp"
#include "application.hpp"
#include <SimpleTimeLayout>
#include <ColorConsoleAppender>
#include <LogManager>

using Wintermute::Application;
using Wintermute::Logging;
using Wintermute::Logger;

namespace Wintermute {
  class LoggingPrivate {
    public:
      Log4Qt::Layout* primaryLayout;
      Log4Qt::ColorConsoleAppender* stdOutAppender;
      Log4Qt::ColorConsoleAppender* stdErrAppender;

      LoggingPrivate() : primaryLayout(0), stdOutAppender(0), stdErrAppender(0) {
        Log4Qt::LogManager::startup();
        primaryLayout  = new Log4Qt::SimpleTimeLayout();
        stdOutAppender = new Log4Qt::ColorConsoleAppender(primaryLayout, Log4Qt::ConsoleAppender::STDOUT_TARGET);
        stdErrAppender = new Log4Qt::ColorConsoleAppender(primaryLayout, Log4Qt::ConsoleAppender::STDERR_TARGET);

        primaryLayout->setName("root");
        stdOutAppender->setName("stdout");

        primaryLayout->activateOptions();
        stdOutAppender->activateOptions();
        stdErrAppender->activateOptions();

        Log4Qt::Logger::rootLogger()->addAppender(stdOutAppender);
      }

      virtual ~LoggingPrivate(){ }
  };
}

Logging* Logging::self = 0;

Logging::Logging() : QObject(Application::instance()), d_ptr(new LoggingPrivate) {
}

Logger*
Logging::obtainLogger(const QString& loggerName){
  Logger* log = Log4Qt::Logger::logger(loggerName);
  log->setParent(Wintermute::Application::instance());
  return log;
}

Logger*
Logging::obtainLogger(const QObject* object){
  if (object == 0)
    return 0;

  return Logging::obtainLogger(object->metaObject()->className());
}

Logging*
Logging::instance() {
  if (!self)
    self = new Logging;

  return self;
}

Logging::~Logging(){
  Log4Qt::LogManager::shutdown();
}

#include "Wintermute/logging.moc"
