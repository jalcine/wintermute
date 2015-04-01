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

#ifndef WINTERMUTE_CORE_MODULE_HPP
#define WINTERMUTE_CORE_MODULE_HPP

#include <string>
#include <list>
#include <functional>
#include <wintermute-core/call.hpp>
#include <wintermute-core/globals.hpp>
#include <wintermute-core/events.hpp>
#include <wintermute-core/util/serializable.hpp>

using std::list;
using std::string;

namespace Wintermute
{
class Method;
class Message;
class DesignationPrivate;
class ModulePrivate;
class ModulePoolPrivate;
class ModuleCallPrivate;

/**
 * Represents the basis of all functional interfaces for Wintermute.
 * @ingroup Procedure Calling
 * @see Wintermute::Module::Call
 * @see Wintermute::Module::Designation
 * @see Wintermute::Module::Pool
 *
 * Modules are programmatic objects that are meant to receive & invoke calls
 * and invoke signals. They allow for the flexibility one would expect from
 * Wintermute.
 */
class Module : public Wintermute::Events::Emittable
#ifndef DOXYGEN_SKIP
  , W_DEF_SHAREABLE(Module)
#endif
{
public:
  W_DECL_PTR_TYPE(Module)

  /// Serves a means of holding a list of modules.
  /// @see Wintermute::Module::Ptr
  typedef std::list<Module::Ptr> List;

  /**
   * Serves as a means of naming a Module.
   * @see Wintermute::Module
   * @ingroup Procedure Calling
   *
   * Provides a means of naming Module instances. This is accurate to a
   * individual name, a shared domain (or collective realm of Module objects)
   * and the PID at which this specific Module instance can be found to be
   * running at.
   */
  class Designation : public Wintermute::Util::Serializable
  {
  public:
    /**
     * Default constructor for Designation.
     * @param name The name of the module.
     * @param domain The domain in which this module resides.
     * @see Wintermute::Module::Call
     *
     * Creates a new Designation with the provided `name` and `domain`.
     */
    explicit Designation(const string& name, const string& domain);

    /// Copy constructor.
    Designation(const Designation& other);

    /// Crafts a Designation from a serialized value.
    /// @see Wintermute::Util::Serializable
    Designation(const Serializable::PlainType& jsonString);

    /// Nullified constructor.
    Designation();

    /// Explicit destructor.
    virtual ~Designation();

    /// Obtains the name of this Designation.
    string name() const;

    /// Obtains the domain of this Designation.
    string domain() const;

    /// Determines if this Designation is nulled out.
    bool isNull() const;

    /// Determines if this Designation is local to this executing process.
    bool isLocal() const;

    /// Equality comparsion operator.
    bool operator==(const Designation& other) const;

    /// Equality comparsion operator.
    bool operator!=(const Designation& other) const;

  private:
    virtual Serializable::Map serialize() const;
    virtual void deserialize(const Serializable::Map& data);
    W_DEF_PRIVATE(Designation)
  };

  /**
   * Represents a repository of the known modules in Wintermute.
   * @ingroup Procedure Calling
   * Allows a process of Wintermute to interface with the known instances of
   * modules that are provided to the system.
   */
  class Pool
  {
  private:
    explicit Pool();

    using ModulePool = Module::Pool;

    W_DEF_PRIVATE(ModulePool);

  protected:
    /**
     * Adds an module to the known list.
     * @param module The designation of the module in question.
     * @return A boolean value on the success of the addition of said Module.
     * @sa Wintermute::Module::Pool::remove
     *
     * Attempts to add a Module to the Pool.
     * */
    bool add(Module::Ptr& module);

    /**
     * Removes a module from the known list.
     * @param designation The designation of the module in question.
     * @return A boolean value on the success of the removal of said Module.
     * @sa Wintermute::Module::Pool::add
     *
     * Attempts to remove a Module from the Pool.
     * */
    bool remove(const Module::Designation& designation);

  public:
    friend class Wintermute::Module;
    friend class Wintermute::ModulePrivate;

    W_DEF_SINGLETON(Pool);

    /// Default destructor for the pool.
    virtual ~Pool();

    /**
     * Looks up a Module with the provided Designation.
     * @sa Wintermute::Module::Pool::has
     * @param designation The designation of the Module in question.
     * @return A pointer to the Module if it's been found.
     * @return A nullptr if the Module exists in the pool.
     *
     * With the provided designation, this methods obtains a pointer to the
     * Module in question. If it can't be found, a pointer to a null Module is
     * returned. Otherwise, a pointer to the Module in question is returned.
     */
    Module::Ptr __hot find(const Module::Designation& designation) const;

    /**
     * Determines if the module with the provided designation is in the pool.
     * @sa Wintermute::Module::isEnabled
     * @param designation The designation of the module in question.
     * @return True if the module is in the pool (equivalent to being enabled).
     *
     * With the provided designation, it determines if any Module pointers are
     * in the Pool with said Designation and returns that as a boolean value
     * of its existence.
     */
    bool __hot has(const Module::Designation& designation) const;

    /**
     * Provides a list of all of the known Module pointers.
     * @return A list of Module pointers in the Pool.
     */
    List modules() const;
  };

  /**
   * Represents a Call object designated to be used with modules.
   * TODO: So with Module calls, they can generate a message that's meant to represent some sort of RPC call. Still have yet to draft how that'll look.
   * FIXME: Anything in quotes and <> *needs* to be named.
   *
   * ## RPC Call Format for ModuleCallMessage or whatever.
   * \code{.javascript}
   * # This is meant for a incoming call.
   * {
   *  "type": "call.in", // find better name
   *  "<incoming>": {
   *    "module": <serialized format of module's designation>,
   *    "name": <name of the call>,
   *   },
   *  "<outgoing>": {
   *    "module": <serialized format of module's designation>,
   *    "name": <name of the call>,
   *  },
   *  "arguments": ['a', 'b', 'c'] <list of arguments for the call>
   * }
   * \endcode
   *
   */
  class Call :
    public Wintermute::Call
  {
  private:
    using ModuleCall = Module::Call;
    W_DEF_PRIVATE(ModuleCall)
    W_SERIALIZABLE(Call)

  protected:
    explicit Call(const Module::Ptr modulePtr, const string& name);

  public:
    virtual ~Call();
    Module::Ptr module() const;
    W_DECL_PTR_TYPE(Call)

  };

  /// Default destructor.
  virtual ~Module();

  /// Obtains the designation of this Module.
  Designation designation() const;

  /// Determines if this Module is enabled.
  bool isEnabled() const;

  /// Fetches the call in question.
  Call::Ptr call(const string & callName) const;

  /// Adds this module to the pool.
  bool enable();

  /// Removes this module from the pool.
  bool disable();

  /// Adds a call to this Module.
  bool addCall(Module::Call::Ptr & callToAdd);

  /// Removes a call with the provided name.
  bool removeCall(const string & callName);

protected:
  W_DEF_PRIVATE(Module)

  /// Default constructor.
  explicit Module(const Designation & designation);

  /// Handles an incoming message for this module.
  virtual bool receiveMessage(const Message & message) const;

  /// Handles the act of sending out a message.
  virtual bool sendMessage(const Message& message);

private:
  virtual Events::Emitter::Ptr emitter() const final;
};
}

#endif
