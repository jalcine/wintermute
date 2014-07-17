#include <sys/types.h>
#include <unistd.h>
#include <string>

using std::string;

namespace Wintermute
{
namespace Procedure
{
struct Designation
{
  pid_t m_pid     = getpid();
  string m_name   = string();
  string m_domain = string();

public:
  inline pid_t pid() const
  {
    return m_pid;
  }
  inline string name() const
  {
    return m_name;
  }
  inline string domain() const
  {
    return m_domain;
  }
};
}
}
