/**
 * vim: ft=qt.cpp
 * Copyright (C) 2013 Jacky Alcine <me@jalcine.me>
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

#ifndef WINTERMUTE_CORE_PROCEDURE_MODULE_HPP
#define WINTERMUTE_CORE_PROCEDURE_MODULE_HPP

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <Wintermute/Procedure/Call>

namespace Wintermute {
  namespace Procedure {
    class ModulePrivate;
    class Module : public QObject {
      Q_OBJECT;
      Q_DISABLE_COPY(Module);
      Q_DECLARE_PRIVATE(Module);
      QScopedPointer<ModulePrivate> d_ptr;

      public:
      explicit Module(QObject* parent);
      virtual ~Module();

      /**
       * @fn qualifiedName
       * Obtains a fully qualified name for this Module.
       */
      virtual QString qualifiedName() const;

      protected:
      /**
       * @fn invokeCall
       * @param name
       *
       * Searches for and invokes a registered call and returns the result.
       */
      QVariant invokeCall(const QString& name);

      /**
       * @fn mountCall
       * @param callSig A lambda to be used on invocation.
       * @param The name of the method to expose this as.
       *
       * Registers the provided lambda for invocation when this module is
       * called to invoke the method by the provided name.
       *
       */
      void mountCall(Call::Signature callSig, const QString& name);
    };
  } /*  Procedure */
} /*  Wintermute */

#endif /* WINTERMUTE_CORE_PROCEDURE_MODULE_HPP */
