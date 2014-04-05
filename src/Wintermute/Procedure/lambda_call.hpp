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

#include <Wintermute/Application>
#include <Wintermute/Procedure/ModuleCall>

namespace Wintermute
{
  namespace Procedure
  {
    class Module;
    class MethodCall;
    class LambdaCall : public ModuleCall
    {
        Q_OBJECT;
        Q_DISABLE_COPY ( LambdaCall );

      public:
        /**
         * @typedef Signature
         * @brief   Provides the short-hand signature for Call methods.
         */
        typedef std::function<QVariant ( QVariantList, const MethodCall & ) > Signature;

        /**
         * @ctor
         */
        explicit LambdaCall ( const QString &name, Module *const module = nullptr,
                              const Signature &lambda = nullptr );

        /**
         * @dtor
         */
        virtual ~LambdaCall();

        /**
         * @fn function
         * @brief The function to be invoked when this call is interacted with.
         */
        Signature function() const;

        /**
         * @fn setFunction
         * @brief Sets the function to be invoked.
         */
        void setFunction ( const LambdaCall::Signature &newFunction );

        virtual QVariant invoke ( const QVariantList &arguments,
                                  const MethodCall &call );

      private:
        Signature m_function;
    };
  }
}

#endif
