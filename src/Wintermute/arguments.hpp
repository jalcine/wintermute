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

#ifndef WINTERMUTE_ARGUMENTS_HPP
#define WINTERMUTE_ARGUMENTS_HPP

#include <Wintermute/Globals>
#include <QtCore/QVariantMap>
#include <QCommandLine>

namespace Wintermute {
  class ArgumentsPrivate;
  class Application;

  /** TODO: Write docs **/
  class Arguments : public QObject {
    friend class Application;
    friend class ApplicationPrivate;

    Q_OBJECT;
    Q_DECLARE_PRIVATE(Arguments);

    QScopedPointer<ArgumentsPrivate> d_ptr;
    static Arguments* self;

    /**
     * @ctor
     * Handles the internal creation of the command-line argument management 
     * system.
     */
    explicit Arguments();

    public:
      virtual ~Arguments();
      /**
       * @fn instance()
       * @brief Provides a pointer to the active Arguments instance.
       *
       * TODO: Complete documentation.
       */
      static Arguments* instance();

      /** TODO: Complete documentation. */
      QVariant argument(const QString& argumentName) const;

      /** TODO: Complete documentation. */
      QVariantMap arguments() const;

      /** TODO: Complete documentation. */
      void addOption(const QChar& optionName, const QString& longOptionName, const QString& description, QCommandLine::Flags flag);

      /** TODO: Complete documentation. */
      void addSwitch(const QChar& switchName, const QString& longSwitchName, const QString& description, QCommandLine::Flags flag);

      /** TODO: Complete documentation. */
      void addParameter(const QString& parameterName, const QString& description, QCommandLine::Flags flag);
      
      //void add
    private:
      Q_SLOT void switchFound (const QString& switchName);
      Q_SLOT void paramFound  (const QString& paramName,  const QVariant& paramValue);
      Q_SLOT void optionFound (const QString& optionName, const QVariant& optionValue);
      Q_SLOT void parseError  (const QString& argumentName);
  };
}

#endif /* WINTERMUTE_ARGUMENTS_HPP */
