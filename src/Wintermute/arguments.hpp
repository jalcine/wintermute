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

#ifndef WINTERMUTE_ARGUMENTS_HPP
#define WINTERMUTE_ARGUMENTS_HPP

#include <Wintermute/Globals>
#include <QtCore/QVariantMap>

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

    explicit Arguments();

    public:
      virtual ~Arguments();
      /**
       * @fn instance()
       * @brief Provides a pointer to the active Arguments instance.
       *
       * TODO: Complete documentation.
       */

      static inline Arguments* instance() { return self; }

      /** TODO: Complete documentation. */
      QVariant argument(const QString& argumentName) const;
      /** TODO: Complete documentation. */
      QVariantMap arguments() const;

    private Q_SLOTS:
      void switchFound(const QString& );
      void paramFound(const QString&, const QVariant& );
      void optionFound(const QString&, const QVariant& );
      void parseError(const QString& );
  };
}

#endif /* WINTERMUTE_ARGUMENTS_HPP */
