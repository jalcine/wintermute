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

#ifndef WINTERMUTE_UTIL_INVOKABLE_HPP
# define WINTERMUTE_UTIL_INVOKABLE_HPP

#include <string>
#include <functional>
#include <stdexcept>
#include <wintermutecore/globals.hpp>

namespace Wintermute
{
namespace Util
{
template<typename _Rt, typename... _Args>
struct InvokablePrivate;

/**
 * Represents the interface required to have an invokable object to Wintermute.
 * @ingroup Utilities
 * @tparam ReturnType The value to be returned by this Invokable's function.
 * @tparam Arguments The type(s) of the arguments to be passed into the
 * Invokable's target.
 *
 * The Invokable interface provides a common-sense base for objects in
 * Wintermute to handle themselves as something that adheres to the Callable
 * concept. This interface serves as the basis for all things invokable in
 * Wintermute to allow for a strict type checking across Wintermute's classes.
 */
template<typename ReturnType, typename... Arguments>
class WINTERMUTE_EXPORT_PRIVATE Invokable
{

public:
  /**
   * Represents the function that this Invokable template uses.
   * Wraps a `std::function` with the provided return value type (ReturnType)
   * and the varadiac typenames (Arguments) for easier use throughout the
   * class.
   */
  typedef std::function<ReturnType(Arguments... arguments)> Function;

  /**
   * Constructor for Invokable.
   * @see Function
   *
   * Builds a new Invokable object with the function to be wrapped as its
   * invocation.
   */
  Invokable(Function& functionToUse) : function(functionToUse)
  {
  }

  /**
   * Default constructor for Invokable.
   *
   * Creates a new Invokable interface with no internal function value.
   */
  explicit Invokable() {  }

  /**
   * Default destructor.
   *
   * Destroys this Invokable.
   */
  inline virtual ~Invokable() {  }

  /**
   * Invokes this object with the provided serialized values.
   * @param arguments The arguments to use to invoking.
   * @see Wintermute::Util::Invokable::operator ReturnType()
   * @return The value returned from said function.
   *
   * Passes the provided arguments to the stored function in this Invokable
   * object.
   */
  ReturnType invokeFunction(const Arguments... arguments) const
  {
    return function(arguments...);
  }

  /**
   * Attaches a provided into this Invokable.
   * @param functionToBind The function to be encapsulated.
   * @see Function
   *
   * Takes the provided function and sets the internal function to
   * `functionToBind`. If there's already a function stored, it's replaced
   * with `functionToBind`.
   */
  void bindFunction(Function& functionToBind)
  {
    if (functionToBind)
    {
      function = functionToBind;
    }
  }

  /**
   * Invokes this object's function with the provided arguments.
   * @sa Wintermute::Util::Invokable::invokeFunction
   * @return The value returned from said function.
   * @throw std::invalid_argument Throws this exception if no function is
   * stored.
   *
   * Using the provided arguments, this invokes the stored function.
   */
  ReturnType operator()(const Arguments... arguments)
  {
    return invokeFunction(arguments...);
  }

private:
  Function function;
};
}
}

#endif
