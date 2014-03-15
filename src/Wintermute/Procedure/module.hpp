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
#include <Wintermute/Globals>
#include <Wintermute/Procedure/Call>
#include <Wintermute/Procedure/LambdaCall>

namespace Wintermute
{
namespace Procedure
{
class ModulePrivate;
class Module : public QObject
{
  Q_OBJECT;
  Q_DISABLE_COPY ( Module );
  Q_PROPERTY ( QString Domain READ domain );
  Q_PROPERTY ( QString Package READ package );
  friend class Call;

protected:
  Q_DECLARE_PRIVATE ( Module );
  QScopedPointer<ModulePrivate> d_ptr;

public:
  /**
   * @ctor
   * @brief Creates a new working module.
   */
  explicit Module ( QObject* parent );

  /**
   * @dtor
   * @brief Destructs this module instance.
   */
  virtual ~Module();

  /**
   * @fn domain
   * Obtains the domain name that this module lives under.
   */
  QString domain() const;

  /**
   * @fn domain
   * Obtains the package name that identifies this module from other
   * modules.
   */
  QString package() const;

  /**
   * @fn qualifiedName
   *
   * The fully formed version of the Module's name (typically a joining of the
   * domain and package).
   */
  QString qualifiedName() const;

  /**
   * @fn calls
   * @brief A list of calls that this module has.
   */
  QStringList calls() const;

  /**
   * @fn dispatch
   * @fn Sends a method call invocation over the wire.
   */
  const QVariant dispatch ( const Call& call ) const;

  /**
   * @fn start
   * @brief Starts the worker logic for this module.
   *
   * Use this method to invoke the starting up processes of this Module and
   * start its chugging.
   */
  Q_SLOT virtual void start() = 0;

  /**
   * @fn stop
   * @brief Halts the worker logic for this module.
   *
   * Use this method to cancel pending tasks and bring the purpose of this
   * Module to a full stop.
   */
  Q_SLOT virtual void stop()  = 0;

protected:
  /**
  * @fn invoke
  * @brief Looks for the named call and invoke with the provided data.
  *
  * Handles the act of invocation of the named call and provides it with the
  * necessary work to invoke it.
  *
  * @note This currently doesn't consider spawning a new thread to handle this
  * working process. A separate method will consider this.
  */
  QVariant invoke ( const QString name, const QVariantList data = QVariantList() );

  /**
   * @fn setDomain
   * @brief Sets the domain of this module.
   */
  void setDomain ( const QString& value );

  /**
   * @fn setPackage
   * @brief Sets the package of this module.
   */
  void setPackage ( const QString& value );

  /**
   * @fn mount
   * @brief Registers the call into the system.
   */
  void mount ( CallPointer call );

  /**
   * @fn mountLambda
   * @brief Crafts a LambdaCall out of a function pointer and adds the
   * call into the system.
   */
  LambdaCall* mountLambda ( const QString& name, 
      LambdaCall::Signature lambdaFunction );
};
} /*  Procedure */
} /*  Wintermute */

Q_DECLARE_INTERFACE(Wintermute::Procedure::Module, "in.wintermute.procedure.module/0.1.0");

#endif /* WINTERMUTE_CORE_PROCEDURE_MODULE_HPP */
