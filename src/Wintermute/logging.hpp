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

    static inline Logging* instance() { return self; }

    static Logger* obtainLogger(const QString& loggerName);
    static Logger* obtainLogger(const QObject* object);
    static Logger* obtainRootLogger();
  };
}

#endif /* WINTERMUTE_LOGGING_HPP */
