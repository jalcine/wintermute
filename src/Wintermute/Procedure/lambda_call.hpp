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

#ifndef WINTERMUTE_PROCEDURE_LAMBDA_CALL_HPP
#define WINTERMUTE_PROCEDURE_LAMBDA_CALL_HPP

#include <Wintermute/Procedure/Call>

namespace Wintermute
{
namespace Procedure
{
class LambdaCall : public Call
{
public:
  /**
   * @typedef Signature
   * @brief   Provides the short-hand signature for Call methods.
   */
  typedef std::function<QVariant ( QVariantList ) > Signature;

  explicit LambdaCall ( const QString& name, const Signature& lambda );
  virtual ~LambdaCall();
  Signature function() const;
  void setFunction ( const Signature& newFunction );
  QVariant invoke ( const QVariantList& data = QVariantList() );
private:
  Signature m_function;
};
}
}

#endif
