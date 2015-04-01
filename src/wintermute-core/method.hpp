/*
 * Wintermute is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with Wintermute; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef WINTERMUTE_CORE_METHOD_CALL_HPP
# define WINTERMUTE_CORE_METHOD_CALL_HPP

#include <functional>
#include <wintermutecore/module.hpp>
#include <wintermutecore/util/serializable.hpp>
#include <wintermutecore/util/invokable.hpp>

namespace Wintermute
{
class MethodPrivate;
/**
 * Represents a method invocation for a Module.
 * Methods are meant to represent a call that's sent over the wire. It
 * serves to invoke the method it represents. This allows for the method in
 * question to have pre-packaged arguments and just be invokable by this call.
 *
 * Methods hold more information about where the place of invocation
 * should take place. Things like the PID of the process, or the hostname of
 * which machine this method call is to take place are all collected to make
 * invocation more accurate.
 *
 * \code{.cpp}
 * Wintermute::Method method('eat', 'me.jalcine.wintermute.person', arguments);
 * method.setPID(303);                 // - Setting the PID where you want this to run.
 * method.setHostname('*.jalcine.me'); // - Setting the hostname of the machine.
 *
 * Wintermute::Tunnel::sendMessage(method);
 * \endcode
 *
 * That's all there is to it. The Method would serialize the call for
 * consumption over the Tunnel and it'd be properly handled on the opposing
 * side.
 */
class WINTERMUTE_EXPORT_PUBLIC Method :
#ifndef DOXYGEN_SKIP
  W_DEF_SHAREABLE(Method),
#endif
  public Wintermute::Util::Serializable
{
private:
  W_DEF_PRIVATE(Method);

protected:
  friend class Module;

  /**
   * Sets the Designation of the Module that crafted this Method.
   */
  void setOriginatingModule(const Module::Designation& moduleDesignation);

public:
  W_DECL_PTR_TYPE(Method)

  typedef Util::Serializable::Map Arguments;

  /**
   * Constructs a new Method.
   * @param callName The name of the Call to invoke.
   * @param moduleName The name of the Module to work with.
   * @param originatingModule The name of the Module that's making this call.
   * @param arguments The arguments to use.
   *
   * With the provided methodName and moduleName, this crafts a call to invoke
   * the provided method on the provided module with the provided arguments.
   */
  explicit Method(const string & callName,
    const Module::Designation & destinationModule,
    const Module::Designation & originatingModule = Module::Designation(),
    const Method::Arguments & arguments = Arguments() );

  /// Destructor for the method.
  virtual ~Method();

  /// Obtains the arguments for this method.
  Util::Serializable::Map arguments() const;

  /// Obtains the name of the Call to invoke by this method.
  string name() const;

  /// Obtains the Designation of the Module that this'll be invoked on.
  Module::Designation module() const;

  /// Obtains the Designation of the Module that requested this Method.
  Module::Designation originatingModule() const;

  /// Determines if this method can be invoked.
  bool isInvokable() const;

  /**
   * Invokes this method.
   * @param providedArguments {optional} The Arguments to use in place of the stored arguments.
   * @sa invoke
   */
  inline string operator()(const Arguments providedArguments = Arguments() )
  {
    return invoke(providedArguments);
  }

  /**
   * Invokes the Call in the provided Module.
   * @param arguments {optional} The arguments to invoke the Call with.
   * @return The return value of said call.
   *
   * Attempts to invoke a Call from the Module with the provided Designation.
   *
   * @todo Make return value into a tuple; <ReturnValue, WasInvoked>.
   */
  string invoke(const Arguments arguments = Arguments() );

  /**
   * Crafts a Message that can be used by the Tunnel.
   * @return A Message object representign this Method.
   * With the provided Designation `originatingModule`, this Method crafts
   * message that can be used
   */
  Message toMessage() const;
  static Method::Ptr fromMessage(const Message& msg);

  W_SERIALIZABLE(Method)
};
}

#define WINTERMUTE_MESSAGE_CALL_METHOD "Call.Method"

#endif

