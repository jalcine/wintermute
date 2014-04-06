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

#include "Wintermute/logging.hpp"
#include "Wintermute/application.hpp"
#include "Wintermute/Procedure/lambda_call.hpp"

using Wintermute::Procedure::LambdaCall;

LambdaCall::LambdaCall(const QString &name, QPointer <const Module> module,
                       const Signature &lambda) : ModuleCall(name, module), m_lambda(lambda)
{
}

bool
LambdaCall::valid() const
{
  if (!ModuleCall::valid()) {
    return false;
  }
  if (!m_lambda) {
    return false;
  }
  return true;
}

LambdaCall::Signature
LambdaCall::lambda() const
{
  Q_ASSERT ( valid() );
  return m_lambda;
}

void
LambdaCall::setLambda ( const Signature &lambda )
{
  Q_ASSERT ( !lambda == false );
  m_lambda = lambda;
}

void
LambdaCall::invoke(const QVariantList &arguments, const MethodCall &call)
{
  Q_ASSERT ( valid() );
  if ( valid() ) {
    m_lambda(arguments,call);
  }
}

LambdaCall::~LambdaCall()
{
}
