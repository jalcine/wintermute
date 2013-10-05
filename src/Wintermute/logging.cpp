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

#include "logging.hpp"
#include "application.hpp"
#include "private/logging.hpp"
#include "Wintermute/logging.moc"
#include <QtCore/QString>

using Wintermute::Application;
using Wintermute::Logging;
using Log4Qt::Logger;

Logging* Logging::self = nullptr;

Logging::Logging() : 
  QObject ( Application::instance() ), d_ptr ( new LoggingPrivate )
{
}

Logger*
Logging::obtainLogger ( const QString& loggerName )
{
  Logger* log = Logger::logger ( loggerName );
  log->setParent ( Application::instance() );
  return log;
}

Logger*
Logging::obtainLogger ( const QObject* object )
{
  if ( object == 0 )
    return Logging::obtainLogger(wntrApp);

  return Logging::obtainLogger ( object->metaObject()->className() );
}

Logging*
Logging::instance()
{
  if ( !self )
    self = new Logging();

  return self;
}

Logging::~Logging()
{
}
