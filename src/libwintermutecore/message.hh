#include <map>
#include <string>
#include <memory>
#include "message.hpp"
#include "module.hpp"

namespace Wintermute
{
class Designation;
class MessagePrivate
{
public:
  Message::HashType data;
  Module::Designation sender;
  Module::Designation receiver;

  void clone(const SharedPtr<MessagePrivate>& d);
  bool isEmpty() const;

  MessagePrivate();
};
}
