/*
 * @author Jacky Alciné <me@jalcine.me>
 * @copyright © 2011, 2012, 2013, 2014 Jacky Alciné <me@jalcine.me>
 * @if 0
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
 * @endif
 **/

#ifndef WINTERMUTE_PROCEDURE_METHOD_CALL_HPP
#define WINTERMUTE_PROCEDURE_METHOD_CALL_HPP

#include <functional>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <Wintermute/Procedure/Message>
#include <Wintermute/Procedure/Designation>
#include <Wintermute/Procedure/Call>

namespace Wintermute
{
namespace Procedure
{
class ReplyCall;
/*
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
 *  #include <Wintermute/Procedure/Designation>
 *
 *  using Wintermute::Procedure::MethodCall;
 *  using Wintermute::Procedure::Dispatcher;
 *  using Wintermute::Procedure::Module;
 *
 *  int main()
 *  {
 *    // Define the module that should be reached.
 *    Designation designation("in.wintermute.heartbeat","monitor");
 *
 *    // Define the call to use to invoke upon said module.
 *    MethodCall aCall("ping", designation, QVariantList());
 *
 *    // Dispatch the call.
 *    Dispatcher::queueCall(aCall);
 *  }
 *  \endcode
 */
class MethodCall : public Call
{
  public:
    /*
     * @brief Crafts a new MethodCall.
     * @param[in] QString The name of the method.
     * @param[in] Module::Designation The designation of the module whose
		 *   method would be invoked.
     * @param[in] QVariant The variant argument sent with this method.
     *
     * Invokes method 'methodName' on module 'designation' with 'arguments'
     * as arguments.
     */
    explicit MethodCall(const QString& methodName,
			const Designation& designation, const QVariant& arguments);

    MethodCall(const Call& other); ///< @brief Reimplemented copy constructor.
    MethodCall(const Message& other); ///< @brief Reimplemented copy constructor.
    virtual ~MethodCall(); ///< @brief Destructor.

    ///< @brief Signature for callback methods.
    typedef std::function<void (QVariant)> CallbackSignature;

    /*
     * @brief Determines if this is a valid MethodCall.
     * @retval boolean Whether or not this is a valid MethodCall.
     */
    virtual bool valid() const;

    /*
     * @brief Obtains the arguments used for this MethodCall.
     * @retval QVariant A variant value for this MethodCall.
     */
    QVariant arguments() const;

    /*
     * @brief Crafts a ReplyCall from this MethodCall.
     * @param[in] QVariant The value to send back.
     * @retval ReplyCall The ReplyCall to use to dispatch with.
     * @retval Message::Null A null Message if from a local call.
     * @note This will return a null call is it's called from a local Call.
     *
     * This allows for a quick-n-easy way to craft a reply for a received
     * MethodCall. It would be used something like the following:
     *
     * \code{.cpp}
     *  using namespace Wintermute::Procedure;
     *  Module* module = ...
     *  LambdaCall* lambdaFoo = new LambdaCall("foo", module,
     *  [&] (QVariant args, const MethodCall& call) -> void {
     *    ReplyCall reply = call.craftReply(true);
     *    reply.queueForDispatch();
     *  } );
     *
     * \endcode
     * */
    ReplyCall craftReply(const QVariant& reply) const;

    /*
     * @brief Attaches a lambda signature to this MethodCall.
     * @param[in] CallbackSignature The lambda function to use as a
     *            callback.
     */
    void setCallback( const CallbackSignature& lambda );

    using Message::setSender;

  private:
    CallbackSignature m_lambda;
};
}
}

#endif
