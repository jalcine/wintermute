#include <string>
#include "globals.hpp"

using std::string;

namespace Wintermute
{
namespace Procedure
{
class DesignationPrivate
{
public:
  PID pid;
  string name;
  string domain;

  void clone(const SharedPtr<DesignationPrivate>& other)
  {
    this->pid = other->pid;
    this->name = other->name;
    this->domain = other->domain;
  }
};
}
}
