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
class Module
{
protected:
  W_DEFINE_PRIVATE(Module);

public:
  class Designation
  {
  private:
    W_DEFINE_PRIVATE(Designation);

  public:
    explicit Designation(const string& name = "", const string& domain = "", const pid_t& pid = getpid());
    Designation(const Designation& other);
    ~Designation();
    pid_t pid() const;
    string name() const;
    string domain() const;
    bool isNull() const;
    bool isLocal() const;

    static Designation local();

    inline bool operator==(const Designation& other)
    {
      return other.name() == name() &&
             other.domain() == domain() &&
             other.pid () == pid();
    }
  };

  explicit Module(const Designation& designation);
  virtual ~Module();

  Designation designation() const;
  virtual bool receiveMessage(const Message& message) const;
  virtual bool sendMessage(const Message& message) const;
};
}

#endif
