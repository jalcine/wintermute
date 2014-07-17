#include <map>
#include <string>
#include "designation.hpp"

using std::map;
using std::string;

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

  void clone(const MessagePrivate* d)
  {
    this->data = d->data;
    this->sender = d->sender;
    this->reciever = d->reciever;
  }
};
}
}
