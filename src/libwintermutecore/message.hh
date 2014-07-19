#include <map>
#include <string>
#include <memory>
#include "designation.hpp"
#include "message.hpp"

namespace Wintermute
{
namespace Procedure
{
class Designation;
class MessagePrivate
{
public:
  Message::HashType data = Message::HashType();
  Designation sender;
  Designation receiver;

  void clone(const SharedPtr<MessagePrivate>& d);
  bool isEmpty() const;
};
}
}
