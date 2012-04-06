/**
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.

 * @file broadcast.hpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 * @date April 9, 2011, 1:44 PM
 */

#ifndef BROADCAST_HPP
#define BROADCAST_HPP

#include "config.hpp"
#include "message.hpp"

using namespace Wintermute::Network;
using Wintermute::Network::Message;

struct QHostAddress; //forward decl

namespace Wintermute {
namespace Network {
struct Broadcast;
struct BroadcastMessage;

/**
 * @brief Management class for broadcasting.
 *
 * This class mananges every aspect of Wintermute's ability to send
 * broadcast signals out onto the network. Typically, these messages
 * are sent to the broadcasting address on TCP & UDP(255.255.255.255::1300+)
 * on local (::1:1300+).
 *
 * @see BroadcastMessage
 */
class Broadcast : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( Broadcast )

private:
    static Broadcast* s_brdcst;
    Broadcast();

signals:
    /**
     * @brief Emitted when a host replies to a ping request.
     * @fn pingReply
     * @param p_hst The qualifier representing the sender host.
     */
    void pingReply ( const QString& ) const;

public:
    ~Broadcast();
    /**
     * @brief Determines if it's accepting.
     *
     * Checks if the broadcasting system has been initialized and if it's been started.
     * @see start()
     * @see stop()
     */
    static const bool isActive( );

    /**
     * @brief Sends a ping to a specified host by its qualifier.
     * @fn ping
     * @param p_qualifier The qualifier of the host to send the ping to.
     */
    static void ping ( const QString& );

    /**
     * @brief Sends a ping to a specified host by its address.
     * @fn ping
     * @param p_addr The direct addres of the host to send the ping to.
     */
    static void ping ( const QHostAddress& );

    /**
     * @brief
     *
     * @fn instance
     */
    static Broadcast* instance();

public slots:
    /**
     * @brief Initializes the broadcast system.
     *
     * This does the inital work of sending out the 'online' broadcast
     * message to show that it's active on the AngelNet.
     *
     * @note If you want to <i>start</i> the broadcasting system, use start() instead.
     * @see start()
     */
    static void unload( );

    /**
     * @brief Deinitializes the broadcast system.
     *
     * This does the work of disabling the broadcast system from sending
     * any more broadcast messages out onto the network.
     *
     * @note If you want to <i>stop</i> the broadcasting system, use stop() instead.
     * @see stop()
     */
    static void load( );
    /**
      * @brief Starts the system.
      *
      * This starts the internal cycle of pumping out a broadcast message at an
      * internally set interval, set at build-time. This activaties, but doesn't
      * disable the system.
      *
      * @todo Make this interval visible from CMake, and changable at run-time.
      * @note Unlike initialize(), this method can be run multiple times (if stop() is called before each time).
      * @note This method will do nothing if the system has been deinitialized.
      * @see stop()
      * @see isActive()
      */
    static void start();

    /**
      * @brief Stops the system.
      *
      * Cancels any pending broadcast messages and prevents any more broadcast
      * messages from being sent. This deactivates, but doesn't disable the system.
      *
      * @note Unlike deinitialize(), this method can be run multiple times (if start() is called before each time).
      * @see stop()
      * @see isActive()
      */
    static void stop();

    /**
      * @brief Send broadcast signal.
      *
      * This forcibly sends a broadcast message, regardless if there's a message queued to be
      * sent; it'd be sent when it is scheduled.
      *
      * @note Nothing will happen to this if the system isn't active.
      * @see isActive
      */
    static void forceSignal();


private slots:
    static void sendSignal();
    static void readSignal ( const Message& );
};

/**
 * @brief A message designed to represent existence on a network.
 * @see Message
 * @see Broadcast
 *
 * Inherited from Message, this class handles the work of representing, determining and querying the
 * existence of other Wintermute processes in the local and remote network. These messages are
 * typically sent over UDP, as supplied by the UDPServer, and therefore have random destination-arrival
 * dates.
 */
class BroadcastMessage : public Message {
    Q_GADGET
    Q_ENUMS ( BroadcastType )
    Q_PROPERTY ( const BroadcastType BroadcastType READ broadcastType )

public:
    /**
     * @brief Enumerator containing types of potential broadcast message.
     *
     * Represents the types of broadcast messages that can be sent.
     *
     * @note For developers, please maintain the order of enumeration and add your new
     *       enumeration to the <b>end</b> of the list. This way, we can maintain backwards-
     *       capability.
     *
     * @see Broadcast
     * @see BroadcastMessage
     */
    enum BroadcastType {
        Unspecified = 0, /**< Define an unspecified broadcast message. */
        Ping,            /**< Defines a message that represents a ping request. The property 'Returned' should be set on reply. */
        Online           /**< Defines a message that states that this Wintermute process is actively on the network. This may include meta-data similar to that found in instant messaging such as profile images and statuses (Online, Busy, Away, Idle) */
    };

    /**
     * @brief Constructor.
     *
     * Creates a new BroadcastMessage of type Unspecified.
     * @param type A reference to a broadcast type, defaults to Unspecified.
     * @see BroadcastType
     */
    explicit BroadcastMessage ( const BroadcastType & = Unspecified );

    /**
     * @brief Copy constructor.
     * @fn BroadcastMessage
     * @param p_msg The source message.
     */
    BroadcastMessage ( const Message& );

    /**
     * @brief Obtains the broadcast type.
     *
     * Gets the broadcast type that was stored by this broadcast message.
     * @return BroadcastType The type of broadcast.
     * @see BroadcastType
     */
    const BroadcastType broadcastType( ) const;
};
}
}
#endif	/* BROADCAST_HPP */

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
