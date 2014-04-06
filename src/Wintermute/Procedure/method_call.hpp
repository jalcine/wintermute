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

#include <Wintermute/Procedure/Call>

namespace Wintermute
{
  namespace Procedure
  {
    /**
     * @brief Basis for method invocation on remote modules.
     * @sa    Dispatcher::queueCall()
     *
     * MethodCall objects are used to send a request to invoke a method on a
     * remote module. They can tuned to invoke on a single module in a specific
     * process or in that module in multiple processes. A request to send a ping
     * to the heartbeat's monitor would be:
     *
     *  \code{.cpp}
     *  #include <Wintermute/Procedure/MethodCall>
     *  #include <Wintermute/Procedure/Dispatcher>
     *
     *  using Wintermute::Procedure::MethodCall;
     *  using Wintermute::Procedure::Dispatcher;
     *
     *  int main()
     *  {
     *    MethodCall aCall("ping", "in.wintermute.heartbeat.monitor",
     *      QVariantList(), 0);
     *
     *    // This would dispatch the call.
     *    Dispatcher::queueCall(aCall);
     *  }
     *  \endcode
     */
    class MethodCall : public Call
    {
      public:
        /**
         * @brief Crafts a new MethodCall.
         * @param[in] QString The name of the method.
         * @param[in] QString The remote module's name.
         * @param[in] QVariant The variant argument sent with this method.
         * @param[in] quint64 The PID of the module to invoke this upon.
         *
         * Invokes method 'methodName' on module 'remoteModule' of PID 'pid' (if
         * provided, otherwise, invoked on all processes with 'remoteModule')
         * with 'arguments' as arguments.
         */
        explicit MethodCall(const QString &methodName, const QString &remoteModule,
                            const QVariant &arguments, const quint64 &pid = 0);

        virtual ~MethodCall(); ///< @brief Destructor.

        /**
         * @brief Determines if this is a valid MethodCall.
         * @retval boolean Whether or not this is a valid MethodCall.
         */
        virtual bool valid() const;

        /**
         * @brief Obtains the arguments used for this MethodCall.
         * @retval QVariant A variant value for this MethodCall.
         */
        QVariant arguments() const;
    };
  }
}

#endif
