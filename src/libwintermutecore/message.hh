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
struct MessagePrivate
{
  map<string, string> data = map<string, string>();
  Designation sender       = Designation();
  Designation reciever     = Designation();

  void clone(const unique_ptr<MessagePrivate>& d)
  {
    this->data = d->data;
    this->sender = d->sender;
    this->reciever = d->reciever;
  }
};
}
}
