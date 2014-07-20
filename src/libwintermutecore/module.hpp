#ifndef WINTERMUTE_CORE_MODULE_HPP
#define WINTERMUTE_CORE_MODULE_HPP

#include <string>
#include "globals.hpp"

using std::string;

namespace Wintermute
{
class DesignationPrivate;
class ModulePrivate;
class Message;

/* Represents the basis of all functional interfaces for Wintermute.
 * Modules are programmatic objects that are meant to receive & invoke calls
 * and invoke signals. They allow for the flexibility one would expect from
 * Wintermute.
 */
class Module
{
public:
  /// Serves a means of holding references to Module objects.
  typedef SharedPtr<Module> Ptr;

  /* Serves as a means of naming a Module.
   * Provides a means of naming Module instances. This is accurate to a
   * individual name, a shared domain (or collective realm of Module objects)
   * and the PID at which this specific Module instance can be found to be
   * running at.
   */
  class Designation
  {
  private:
    W_DEFINE_PRIVATE(Designation);

  public:
    /// Default constructor for Designation objects.
    explicit Designation(const string& name = "", const string& domain = "", const pid_t& pid = getpid());

    /// Copy constructor.
    Designation(const Designation& other);

    /// Explicit destructor.
    virtual ~Designation();

    /// Obtains the PID from this Designation.
    pid_t pid() const;

    /// Obtains the name of this Designation.
    string name() const;

    /// Obtains the domain of this Designation.
    string domain() const;

    /// Determines if this Designation is nulled out.
    bool isNull() const;

    /// Determines if this Designation is local to this executing process.
    bool isLocal() const;

    /// Converts this Designation into a string.
    operator string() const;

    /// Equality comparsion operator.
    bool operator==(const Designation& other) const;
  };

  /// Default destructor.
  virtual ~Module();

  /// Obtains the designation of this Module.
  Designation designation() const;

protected:
  W_DEFINE_PRIVATE(Module);

protected:
  /// Default constructor.
  explicit Module(const Designation& designation);

  /// Handles an incoming message for this module.
  virtual bool receiveMessage(const Message& message) const;

  /// Handles the act of sending out a message.
  virtual bool sendMessage(const Message& message) const;

};
}

#endif
