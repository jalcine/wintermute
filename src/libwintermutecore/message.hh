#include <map>
#include <string>
#include <memory>
#include "designation.hpp"

using std::map;
using std::string;
using std::unique_ptr;

namespace Wintermute
{
namespace Procedure
{
class Designation;
class MessagePrivate
{
public:
  map<string, string> data = map<string, string>();
  Designation sender       = Designation();
  Designation reciever     = Designation();

  void clone(const SharedPtr<MessagePrivate>& d);
  bool isEmpty() const;
};
}
}
