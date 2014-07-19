#ifndef WINTERMUTE_CORE_MESSAGESERIALIZER_HPP
#define WINTERMUTE_CORE_MESSAGESERIALIZER_HPP

#include "globals.hpp"
#include "message.hpp"

namespace Wintermute
{
class WINTERMUTECORE_EXPORT MessageSerializer
{
private:
  explicit MessageSerializer();

public:
  virtual ~MessageSerializer();
  static Message deserialize(const string& data);
  static const string serialize(const Message& data);

  W_DEFINE_SINGLETON(MessageSerializer);
};
}

#endif
