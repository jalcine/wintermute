/**
 * vim: ft=cpp
 * Copyright (C) 2013 Jacky Alcine <jacky.alcine@thesii.org>
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

#ifndef WINTERMUTE_LOGGING_HPP
#define WINTERMUTE_LOGGING_HPP

#include <Wintermute/Globals>
#include <Logger>

namespace Wintermute {
  class LoggingPrivate;

  typedef ::Log4Qt::Logger Logger;

  class Logging : public QObject {
    Q_OBJECT;
    Q_DECLARE_PRIVATE(Logging);

    friend class Application;
    friend class ApplicationPrivate;

    QScopedPointer<LoggingPrivate> d_ptr;
    static Logging* self;
    explicit Logging();

    public:
    virtual ~Logging();

    static Logging* instance();
    static Logger* obtainLogger(const QString& loggerName);
    static Logger* obtainLogger(const QObject* object);
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
 *
 * TODO: Make this conditionally visible if WINTERMUTE_DEBUG isn't set.
 */
#define wdebug(qobj, text) wlog(qobj)->debug(text)

/**
 * @fn wwarn
 * @param qobj An object based on QObject
 * @param text The text to be displayed as a warning.
 *
 * TODO: Make this conditionally visibile if WINTERMUTE_WARNINGS isn't set.
 */
#define wwarn(qobj, text) wlog(qobj)->warn(text)


#endif /* WINTERMUTE_LOGGING_HPP */
