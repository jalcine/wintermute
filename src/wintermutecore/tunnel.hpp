/*
 * Wintermute is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with Wintermute; if not, write to the
 * ree Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef WINTERMUTE_CORE_TUNNEL_HPP
#define WINTERMUTE_CORE_TUNNEL_HPP

#include <list>
#include <wintermutecore/globals.hpp>
#include <wintermutecore/message.hpp>
#include <wintermutecore/events.hpp>

using std::list;

namespace Wintermute
{
class Message;
class TunnelPrivate;
struct DispatcherPrivate;
struct ReceiverPrivate;

/**
 * @brief Handles the act of sending and receiving new messages.
 * @group Procedure Calling
 *
 * The `Tunnel` serves as a bridge between the `Dispatcher`, `Receiver` and
 * `ModulePool` singleton to transfer a message from one place to another
 * transparently. The Tunnel works closely with `Message` to convert
 * incoming data into a Message and outgoing Message objects into strings
 * that can be more readily manipulated..
 */
class Tunnel : public Events::Emittable
{
  W_DEF_PRIVATE(Tunnel)
  explicit Tunnel();

public:
  W_DEF_SINGLETON(Tunnel)

  // Public destructor.
  virtual ~Tunnel();

  virtual Events::Emitter::Ptr emitter() const;

  /**
   * @brief Represents the abstraction of a `Message` sender.
   * @group Procedure Calling
   * @interface Wintermute::Dispatcher
   * @sa Wintermute::Receiver
   * @sa Wintermute::Tunnel
   * @sa Wintermute::Message
   *
   * 'Dispatcher' classes are used by Wintermute to handle the act of taking a
   * Message in its composite form and sending it over what its implementation
   * defines, be it the network or over a file descriptor. Such broad levels of
   * abstraction now allows Wintermute to do whatever it'd like when it comes
   * to sending out messages. Implementations can range from JSON-RPC to
   * sending out audible morse code from one machine to another.
   *
   * @todo Look into adding ones that have some form of encryption.
   * @todo Look into adding ones that have some form of signing.
   */
  class Dispatcher
    : public Events::Emittable
#ifndef DOXYGEN_SKIP
    , W_DEF_SHAREABLE(Dispatcher)
#endif

  {
  public:
    W_DECL_PTR_TYPE(Dispatcher)
    typedef std::list<Dispatcher::Ptr> List;

    /* Public destructor. */
    virtual ~Dispatcher();

    /* Obtain the name of the dispatcher. */
    string name() const;

    /* Sends out the provided message over the wire. */
    virtual bool send(const Message & message) = 0;

    virtual Events::Emitter::Ptr emitter() const;

  protected:
    /* Protected constructor. */
    explicit Dispatcher(const string & name);

  private:
    W_DEF_PRIVATE(Dispatcher)
  };

  /**
   * @brief Represents the abstraction of a `Message` obtainer.
   * @group Procedure Calling
   * @interface Wintermute::Receiver
   * @sa Wintermute::Dispatcher
   * @sa Wintermute::Tunnel
   * @sa Wintermute::Message
   *
   * `Receiver` classes are used by Wintermute to handle the act of obtaining
   * a `Message` from any arbitrary format and converting it into a composite
   * `Message` locally. This level of abstraction allows Wintermute to fetch a
   * `Message` without any real concern from where or how the `Message`came
   * from or came to be, respectfully.
   *
   * @todo Look into adding ones that have some form of encryption.
   * @todo Look into adding ones that have some form of signing.
   */
  class Receiver
    : public Events::Emittable
#ifndef DOXYGEN_SKIP
    , W_DEF_SHAREABLE(Receiver)
#endif
  {
  public:
    W_DECL_PTR_TYPE(Receiver)
    typedef std::list<Receiver::Ptr> List;

    // Public destructor.
    virtual ~Receiver();

    /**
     * Obtains the name of this Receiver.
     */
    string name() const;

    /**
     * Obtains a `Message` from the implementation's details.
     * @sa Wintermute::Dispatcher::send
     *
     * Requests a new message from the Receiver's internal specifications.
     */
    virtual Message receive() = 0;

    virtual Events::Emitter::Ptr emitter() const;

  protected:
    explicit Receiver(const string & name);

  private:
    W_DEF_PRIVATE(Receiver)
  };

  // Registers a dispatcher into the Tunnel.
  static bool registerDispatcher(const Dispatcher::Ptr& dispatcher);

  // Unregisters a dispatcher into the Tunnel.
  static bool unregisterDispatcher(const Dispatcher::Ptr& dispatcher);

  // Removes all of the dispatchers from the Tunnel.
  static void clearAllDispatchers();

  // Determines if this Tunnel knows of the provided dispatcher.
  static bool knowsOfDispatcher(const string& dispatcherName);

  // Obtains all of the dispatchers in the Tunnel.
  static Dispatcher::List dispatchers();

  // Registers a receiver into the Tunnel.
  static bool registerReceiver(const Receiver::Ptr& receiver);

  // Unregisters a receiver into the Tunnel.
  static bool unregisterReceiver(const Receiver::Ptr& receiver);

  // Removes all of the receivers from the Tunnel.
  static void clearAllReceivers();

  // Determines if the Tunnel knows of the provided receiver.
  static bool knowsOfReceiver(const string& receiverName);

  // Obtains all of the receivers in the Tunnel.
  static Receiver::List receivers();

  // Sends a message through the Tunnel through all dispatchers.
  static bool sendMessage(const Message& message);

  // Determines if there's pending messages for fetching off the received queue.
  static bool hasPendingMessages();

  // Takes the oldest message off the recieved queue and returns it.
  static const Message receiveMessage();
};
}

#endif
