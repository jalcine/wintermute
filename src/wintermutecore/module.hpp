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
#include "globals.hpp"
#include "util/serializable.hpp"

using std::list;
using std::string;

namespace Wintermute
{
class DesignationPrivate;
class ModulePrivate;
class ModulePoolPrivate;
class Message;

/* Represents the basis of all functional interfaces for Wintermute.
 * Modules are programmatic objects that are meant to receive & invoke calls
 * and invoke signals. They allow for the flexibility one would expect from
 * Wintermute.
 */
class Module
{
public:
  // Serves a means of holding references to Module objects.
  typedef SharedPtr<Module> Ptr;

  // Serves a means of holding a list of modules.
  typedef std::list<Module::Ptr> List;

  /*
   * Serves as a means of naming a Module.
   * Provides a means of naming Module instances. This is accurate to a
   * individual name, a shared domain (or collective realm of Module objects)
   * and the PID at which this specific Module instance can be found to be
   * running at.
   */
  class Designation : public Wintermute::Util::Serializable
  {
  public:
    // Default constructor for Designation objects.
    explicit Designation(const string& name, const string& domain, const pid_t& pid = getpid());

    // Copy constructor.
    Designation(const Designation& other);

    // JSON string constructor.
    Designation(const string& jsonString);

    // Nullified constructor.
    Designation();

    // Explicit destructor.
    ~Designation();

    // Obtains the PID from this Designation.
    pid_t pid() const;

    // Obtains the name of this Designation.
    string name() const;

    // Obtains the domain of this Designation.
    string domain() const;

    // Determines if this Designation is nulled out.
    bool isNull() const;

    // Determines if this Designation is local to this executing process.
    bool isLocal() const;

    // Equality comparsion operator.
    bool operator==(const Designation& other) const;

    // Equality comparsion operator.
    bool operator!=(const Designation& other) const;

  private:
    // @virtual Virtual method from Serializable.
    virtual Serializable::Map serialize() const;

    // @internal Virtual method from Serializable.
    virtual void deserialize(const Serializable::Map& data);

    W_DEF_PRIVATE(Designation)
  };

  /* Represents a repository of the known modules in Wintermute. */
  class Pool
  {
  public:
    W_DEF_SINGLETON(Pool);

    /* Default destructor for the pool. */
    virtual ~Pool();

    /* Looks up a module with the provided designation. */
    Module::Ptr __hot find(const Module::Designation& designation) const;

    /* Provides a list of all of the known modules. */
    List modules() const;

    /* Adds a module to the known list. */
    bool add(Module::Ptr& module);

    /* Removes a module from the known list. */
    bool remove(const Module::Designation& designation);

  private:
    /* Private constructor for the pool. */
    explicit Pool();

    W_DEF_PRIVATE(ModulePool);
  };

  // Default destructor.
  virtual ~Module();

  // Obtains the designation of this Module.
  Designation designation() const;

protected:
  W_DEF_PRIVATE(Module)

protected:
  // Default constructor.
  explicit Module(const Designation& designation);

  // Handles an incoming message for this module.
  virtual bool receiveMessage(const Message& message) const;

  // Handles the act of sending out a message.
  virtual bool sendMessage(const Message& message) const;

};
}

#endif
