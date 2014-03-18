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

#ifndef WINTERMUTE_PROCEDURE_METHOD_CALL_HPP
#define WINTERMUTE_PROCEDURE_METHOD_CALL_HPP

#include <QtCore/QObject>
#include <Wintermute/Procedure/Call>
#include <Wintermute/Application>

namespace Wintermute
{
namespace Procedure
{
class MethodCallPrivate;
/***
 * @class MethodCall
 *
 * A step up from the underlying Call class, this provides more concrete logic
 * for invoking method calls on remote objects.
 */
class MethodCall : public Call
{
  Q_OBJECT;
  Q_DISABLE_COPY ( MethodCall );
  Q_DECLARE_PRIVATE ( MethodCall );
  Q_PROPERTY(QVariantList Arguments READ arguments WRITE setArguments);
  Q_PROPERTY(QString Method READ method WRITE setMethod);
  Q_PROPERTY(QString Module READ module WRITE setModule);

public:
  /**
   * @ctor
   *
   * Crafts a call that can be used to invoke a remote method over the
   * network.
   */
  explicit MethodCall ( 
    const QString& module = QString::null,
    const QString& method = QString::null, 
    const QVariantList arguments = QVariantList() );

  /**
   * @dtor
   */
  virtual ~MethodCall();

  virtual bool isValid() const;

  /**
   * @fn arguments
   * @brief The arguments to pass over the wire.
   */
  QVariantList arguments() const;
  QString module() const;
  QString method() const;

  /**
   * @fn setArguments
   * @brief Sets the arguments to use for this call.
   */
  void setArguments ( const QVariantList& arguments );
  void setModule ( const QString& module );
  void setMethod ( const QString& method );

  /**
   * @fn dispatch
   * @brief A helper method to dispatch calls to a remote module.
   */
  void dispatch ( Module* module = wntrApp->module() );
};
}
}

#endif
