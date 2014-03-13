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

#ifndef WINTERMUTE_LOGGING_HPP
#define WINTERMUTE_LOGGING_HPP

#include <Wintermute/Globals>
#include <Logger>

namespace Wintermute
{
class LoggingPrivate;

typedef ::Log4Qt::Logger Logger;

class Logging : public QObject
{
  Q_OBJECT;
  Q_DECLARE_PRIVATE ( Logging );

  friend class Application;
  friend class ApplicationPrivate;

  QScopedPointer<LoggingPrivate> d_ptr;
  static Logging* self;
  explicit Logging();

public:
  virtual ~Logging();

  static Logging* instance();
  static Logger* obtainLogger ( const QString& loggerName );
  static Logger* obtainLogger ( const QObject* object );
  static Logger* obtainRootLogger();
};
}

// Shortcuts for quick logging support.

/**
 * @fn wlog
 * @param qobj An object based on QObject.
 *
 * Obtains a logger for the specific QObject.
 */
#define wlog(qobj) Wintermute::Logging::obtainLogger(qobj)

/**
 * @fn wdebug
 * @param qobj An object based on QObject.
 * @param text The text to be displayed for debugging.
 *
 * Obtains a logger for the specific QObject and then prints out a
 * line of text for debubgging view.
 */
#ifdef WINTERMUTE_DEBUG
#define wdebug(qobj, text) wlog(qobj)->debug(text)
#endif

/**
 * @fn wwarn
 * @param qobj An object based on QObject.
 * @param text The text to be displayed as a warning.
 */
#define wwarn(qobj, text) wlog(qobj)->warn(text)

/**
 * @fn winfo
 * @param qobj An object based on QObject.
 * @param text The text to be shown informatively.
 *
 * Obtains a logger for the specific QObject and the prints out a line of text
 * for informative help. Typically used for informing the user of an action.
 */
#define winfo(qobj, text) wlog(qobj)->info(text)

/**
 * @fn werr
 * @param qobj An object based on QObject.
 * @param text The text to be shown as an error message.
 *
 * Obtains a logger for the specific QObject and then prints out a line of
 * text for error reporting.
 */
#define werr(qobj, text) wlog(qobj)->error(text)

#endif /* WINTERMUTE_LOGGING_HPP */
