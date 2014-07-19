#ifndef WINTERMUTE_PROCEDURE_MESSAGE_HPP
#define WINTERMUTE_PROCEDURE_MESSAGE_HPP

#include <string>
#include <map>
#include "globals.hpp"
#include "designation.hpp"

using std::string;
using std::map;

namespace Wintermute
{
namespace Procedure
{
class MessagePrivate;

/* Serves as the basis for message passing between modules.
 * Message is the data class that allows for passing of data
 * structures over ambigious means of data trasveral.
 *
 * @see Wintermute::Procedure::Dispatcher
 * @see Wintermute::Procedure::Receiver
 */
class WINTERMUTECORE_EXPORT Message
{
  W_DEFINE_PRIVATE(Message);

public:
  /* A type mapping used to handle the data hash's key. */
  typedef string HashKey;
  /* A type mapping used to handle the data hash's value. */
  typedef string HashValue;
  /* A type mapping used to handle the data hash. */
  typedef map<HashKey,HashValue> HashType;

  /* Builds a new Message with the provided data. */
  Message(const Message::HashType& data = Message::HashType());

  /* Copy constructor. */
  Message(const Message& other);

  /* Destructor */
  virtual ~Message();

  /* Determines if this Message was crafted locally.
   * Checks if the PID of the sender matches the currently executing processes'
   * PID.
   */
  bool isLocal() const;

  /* Determines if this Message represents no data.
   * Checks if the data key-value store is empty as well as the sender +
   * receiver values are empty.
   */
  bool isEmpty() const;

  /* Copies the content of this Message into another one. */
  Message clone() const;

  /* Obtains the payload that's been held by this message. */
  HashType payload() const;

  /* Obtains the designation of the sending module. */
  Designation sender() const;

  /* Obtains the designation of the receiving module. */
  Designation receiver() const;

  /* Changes the sender of this message.
   * @param Designation The designation of the new sender.
   */
  void setSender(const Designation& newSender = Designation::local());

  /* Changes the receiver of this message.
   * @param Designation The designation of the new receiver.
   * NOTE: This methods asserts if newReciever.null() == true
   */
  void setReciever(const Designation& newReciever);

  /* Changes the payload data that this Message contains.
   * @param HashType The data that this Message would contain.
   */
  void setPayload(const Message::HashType& newData);

};
}
}

#endif
