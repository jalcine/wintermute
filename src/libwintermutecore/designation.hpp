#ifndef WINTERMUTE_CORE_PROCEDURE_DESIGNATION_HPP
#define WINTERMUTE_CORE_PROCEDURE_DESIGNATION_HPP
#include <string>
#include "globals.hpp"

using std::string;

namespace Wintermute
{
namespace Procedure
{
class DesignationPrivate;
class Designation
{
private:
  W_DEFINE_PRIVATE(Designation);

public:
  explicit Designation();
  Designation(const Designation& other);
  ~Designation();
  pid_t pid() const;
  string name() const;
  string domain() const;
  bool isNull() const;
  bool isLocal() const;

  static Designation local();
};
}
}

#endif
