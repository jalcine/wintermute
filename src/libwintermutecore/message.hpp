#ifndef WINTERMUTE_PROCEDURE_MESSAGE_HPP
#define WINTERMUTE_PROCEDURE_MESSAGE_HPP

#include <string>
#include <map>

using std::string;
using std::map;

namespace Wintermute
{
namespace Procedure
{
class MessagePrivate;
class Designation;
class Message
{
  MessagePrivate* d;

public:
  Message();
  virtual ~Message();
  bool isLocal() const;
  Message clone() const;
  map<string, string> data() const;
  Designation sender() const;
  Designation receiver() const;
  string toString() const;

  inline operator string () const
  {
    return this->toString();
  }

  void setSender(const Designation& newSender);
  void setReciever(const Designation& newReciever);
  void setData(const map<string, string> newData);

};
}
}

#endif
