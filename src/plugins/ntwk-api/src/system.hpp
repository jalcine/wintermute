/**
 * @file    server.hpp
 * @author  Wintermute Development <wntr-devel@thesii.org>
 * @date 8/26/2011
 *
 * Copyright (C) 2011 by Wintermute Development <wntr-devel@thesii.org>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute Linguistics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute Linguistics; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <QObject>
#include <QQueue>
#include <QStringList>
#include "message.hpp"

struct QHostAddress; //forward decl
struct QLocalServer;
struct QLocalSocket;
struct QTcpServer;
struct QTcpSocket;
struct QUdpSocket;

namespace Wintermute
{
namespace Network
{
struct System;
struct Server;

/**
 * @brief Features exposed by a Server or Client.
 *
 * This enumerator allows servers to express the connection environment over their
 * protocol. For example, the UDP protocol is used for lightweight, unreliable,
 * and datagram-oriented messages whereas the TCP protocol could be used for
 * reliable, stream-oriented, and continuous rivers of data.
 *
 * @see Message
 * @see BroadcastMessage
 * @enum Features
 */
enum Features {
    None = 0, /**< No server should really use this flag, it's used mainly to express all of the servers for the Server::send() method. */
    Reliable, /**< Designates that this is a reliable protocol. We tend to hope everyone's reliable :) */
    Connectionless /**< Designates that this isn't a reliable protocol. This could be used for less-senstitive information. */
};

/**
 * @brief Represents the management system of Wintermute's network interactions.
 *
 * The System class provides a front-end to having developers connect with the
 * interaction of the AngelNet. Wintermute already uses this internally (see Broadcast)
 * but this interface provides a means of plug-in developers a 'port' into the AngelNet.
 * Simple usage would be like the following:
 *
 * @code
 * Message* l_msg = new Message("Foo","Bar");
 * System::send(l_msg,System::Connectionless);
 * @endcode
 *
 * The above snippet sends a simple message with the data 'Foo' : 'Bar'
 * to any connectionless server. It doesn't matter if it doesn't get anywhere
 * in particular.
 *
 * It's also good to mention that the <i>recipient</i> of a @see Message should be specified
 * in the Message, otherwise it'd be sent to the broadcast address (255.255.255.255).
 * This may result in unexpected results and data infrigment. See @see Message for more
 * information.
 *
 * @see Message
 * @class System "src/system.hpp"
 */
class System : public QObject
{
    Q_OBJECT
    friend class Server;

signals:
    /**
     * @brief Signal recieved when a message is recieved.
     *
     * This signal is emitted when one of the registered servers in the system
     * emit a sub-signal informing that a new Message has been recieved.
     *
     * @note <i>This</i> is where developers interested in the AngelNet should
     * hook their response counterparts.
     *
     * @fn messageRecieved
     * @param p_msg The recieved @see Message.
     */
    void messageRecieved (const Message&);

public:
    /**
     * @brief Represents a list of @see Server s.
     * @typedef ServerList
     */
    typedef QList<Server*> ServerList;

    /**
     * @brief Sends a @see Message via all client sockets.
     *
     * This method sends the Message p_msg to all of the servers that
     * have the Features flag p_ftr. If p_ftr is set to Features::None,
     * then p_msg is sent to all of the severs.
     *
     * @fn send
     * @todo Have this method work asynchronously.
     * @param p_msg The Message to send.
     * @param p_tr The kind of Server to send the Message.
     * @see Features
     */
    static void send (const Message& , const Features& = Reliable);

    /**
     * @brief Sends a Message to a designated client socket.
     *
     * This method sends the Message p_msg to the servers that are
     * specifying the protocol as represented by p_prtcl.
     *
     * @fn send
     * @param p_msg The Message to be sent.
     * @param p_prtcl The specific protocol to use.
     */
    static void send (const Message& , const QString&);

    /**
     * @brief Starts the AngelNet system.
     *
     * This method does the internal working to begin the act of
     * initializing the System.
     *
     * @fn start
     */
    static void start();

    /**
     * @brief Stops the AngelNet system.
     *
     * This method does the internal working to quit the AngelNet system.
     *
     * @fn stop
     */
    static void stop();

    /**
     * @brief Obtains all servers.
     *
     * Returns a listing of all of the protocols that are implemented
     * by the servers on this system.
     *
     * @fn allServers
     */
    static const QStringList allServers();
    /**
     * @brief
     *
     * @fn isActive
     */
    static const bool isActive();
    /**
     * @brief Obtains an instance.
     *
     * This returns an object (possibly used for signals and slots) that
     * represents the internal object of the Server.
     *
     * @fn instance
     */
    static System* instance();
    /**
     * @brief Obtains the address from a qualifier.
     *
     * This method determines the host address from a qualifier.
     *
     * @todo Implement a means of obtaining host addresses from qualifiers.
     *
     * @fn toHostAddress
     * @param
     */
    static const QHostAddress toHostAddress (const QString&);
    /**
     * @brief Obtains an qualifier from a host address.
     *
     * This method determines a valid qualifier from a host address.
     *
     * @todo Implement a means of generating qualifiers from host addresses.
     * This <b>might</b> need to use the ontology; but that's only when Wintermute
     * can store information about other Wintermute processes.
     *
     * @fn toQualifier
     * @param
     */
    static const QString toQualifier (const QHostAddress&);

protected:
    /**
     * @brief Constructor.
     * @fn System
     */
    System();

    /**
     * @brief Copy constructor.
     * @fn System
     * @param p_sytm The System being copied.
     */
    System (const System&);

    /**
     * @brief Deconstructor.
     * @fn System
     */
    virtual ~System();

private:
    static System s_system; /**< An instance of the System, so that signals and slots can be done. */
    static ServerList s_servers;
};

/**
 * @brief An abstract server class for connecting with an AngelNet.
 *
 * This class provides a model that of which classes derviving this class can
 * use to implement another protocol to be amended into the system. By reimplementing
 * this class, one can create a protocol wrapper.
 *
 * Servers can send and recieve data over the network, thus making them an
 * efficient wrapper for protocols like TCP or UDP.
 *
 * It's required, if you want your server to be initialized immediately,
 * that your code looks like this:
 *
 * A <b>server.hpp</b> ...
 * @code
 * class MyServer : public Server {
 *      private:
 *          static MyServer s_myServer;
 *          MyServer(const MyServer& );
 *          MyServer& operator=(const MyServer& );
 *
 *      public:
 *          ....
 * };
 * @endcode
 *
 * ... and <b>server.cpp</b>,
 * @code
 * MyServer MyServer::s_myServer;
 *
 * MyServer::MyServer() : Server(*this) { }
 * @endcode
 *
 * It's important that whatever you do, you call the base class constructor of your
 * dervived Server class to have the base class add it to the system. You could use
 * a pointer as well, but it's recommended that you make your server class NonCopyable,
 * as a copy of your server wouldn't be the Server class that's registered in the server.
 *
 * @note If you're not extending this class, it's best that you avoid using it.
 *
 * @see LocalServer
 * @see TcpServer
 * @see UdpServer
 * @class Server system.hpp "src/system.hpp"
 */
class Server : public QObject
{
    Q_OBJECT
    Q_PROPERTY (const QString Protocol READ protocol)
    Q_PROPERTY (const Features Features READ features)

signals:
    /**
     * @brief Signals for recieving messages.
     *
     * This signal should be emitted when the server's recieved
     * a message from a server over the network.
     *
     * @fn messageRecieved
     * @param p_msg The message crafted from the data from the network.
     */
    void messageRecieved (const Message&);

public:
    /**
     * @brief Default constructor.
     * @fn Server
     */
    explicit Server();

    /**
     * @brief Copy constructor.
     * @fn Server
     * @param p_srvr The Server to be copied.
     */
    Server (const Server&);

    /**
     * @brief Deconstructor.
     * @fn ~Server
     */
    virtual ~Server() = 0;

    /**
     * @brief Determines if the server is active.
     *
     * This returns a booelan that should be true whenever the server
     * is active. It doesn't mean that there's no queued incoming messages,
     * it means, more so, whether or not the server's socket is open.
     *
     * @fn isActive
     */
    virtual const bool isActive() const = 0;

    /**
     * @brief Starts up the Server.
     *
     * This method should open the Server's socket, but not necessarily
     * send anything. After the initialization code for the server is done,
     * before connecting, call connectToSocket().
     *
     * @fn start
     * @see connectToSocket ()
     */
    virtual void start() = 0;

    /**
     * @brief Stops the Server.
     *
     * This method should close the Server's socket, but not necessarily
     * cancel anything.
     *
     * @fn stop
     * @see disconnectFromSocket ()
     */
    virtual void stop() = 0;

    /**
     * @brief
     *
     * @fn send
     * @param
     */
    virtual void send (const Message&) = 0;

    /**
     * @brief Returns the protocol name.
     *
     * This method should return a string representing the
     * canonical name of the protocol wrapping by the Server.
     *
     * @fn protocol
     */
    virtual const QString protocol() const = 0;

    /**
     * @brief Returns the features of the server.
     *
     * This represents a flag set based on Features to
     * help the system pick an approriate server for recieving
     * a particular message.
     *
     * @fn Server::features
     */
    virtual const Features features() const = 0;

protected:
    QQueue<const Message*> m_messgeQueue; /**< Used for subclasses to hold a queue of Messages. */
    /**
     * @brief
     *
     * @fn connectToSocket
     */
    virtual void connectToSocket() = 0;

    /**
     * @brief
     *
     * @fn disconnectFromSocket
     */
    virtual void disconnectFromSocket() = 0;

protected slots:

    /**
     * @brief Handles the actions to take place once an ongoing socket has been connected.
     * @fn handleConnected
     */
    virtual void handleConnected() = 0;

    /**
     * @brief Handles the actions to take place once an incoming socket has been connected.
     * @fn handleConnection
     */
    virtual void handleConnection() = 0;

    /**
     * @brief Handles the actions to take place once a socket's ready to be read.
     * @fn handleRead
     */
    virtual void handleRead() = 0;
};

/**
 * @brief Represents a local socket based server geared towards the AngelNet.
 *
 * This server is an implementation of Server and represents a server socket
 * on a local network. This network is a special network, as the name of the
 * server's connection resembles a socket file that would be found in the <b>/tmp</b>
 * directory.
 *
 * @bug There's no real broadcast server for the local server. This makes the
 *      process a bit difficult. It's as if users would have to explicity
 *      define which Wintermute socket they'd wish to speak to.
 *
 * @see Server
 * @class LocalServer system.hpp "src/system.hpp"
 */
class LocalServer : public Server
{
    Q_OBJECT
    Q_DISABLE_COPY (LocalServer)

public:
    /**
     * @brief Default constructor.
     *
     * @fn LocalServer
     */
    LocalServer();

    /**
     * @brief Deconstructor.
     *
     * @fn ~LocalServer
     */
    virtual ~LocalServer();

    /**
     * @brief Starts the local server.
     *
     * When the local server starts up, it generates a random
     * UUID and prefixes it with 'Wintermute_'. It then has the
     * internal server generate the said socket.
     *
     * @todo Have a one-time unique qualifier for the local server, something
     *       like a 'pseudo-static IP', instead of like a pseudo-dynamic IP.
     *
     * @fn start
     */
    virtual void start ();

    /**
     * @brief Stops the local server.
     *
     * The port used by the server is closed and released.
     *
     * @fn stop
     */
    virtual void stop ();

    /**
     * @brief Sends out a @see Message from this @see Server to the local network.
     *
     * @fn send
     * @param
     */
    virtual void send (const Message&);

    /**
     * @brief Determines if the local server is active.
     *
     * This finds out if the local server is currently able to
     * recieve a @see Message.
     *
     * @fn isActive
     */
    virtual const bool isActive () const;

    /**
     * @brief Determines the protocol of the server.
     *
     * This will always return 'local'.
     *
     * @fn protocol
     */
    virtual const QString protocol() const;

    /**
     * @brief Determines the features of the server.
     *
     * This will always return @see Features::Required.
     *
     * @fn features
     */
    virtual const Features features () const;

protected:

    /**
     * @brief Connects the local server to its approriate socket.
     * @fn connectToSocket
     */
    virtual void connectToSocket();

    /**
     * @brief Disconnects the local server from its socket.
     * @fn disconnectFromSocket
     */
    virtual void disconnectFromSocket();

protected slots:

    /**
     * @brief Handles the actions of outgoing sockets from the local server.
     * @fn handleConnected
     */
    virtual void handleConnected ();

    /**
     * @brief Handles any new connections to the local server.
     * @fn handleConnection
     */
    virtual void handleConnection();

    /**
     * @brief Handles the action of reading data from a server.
     * @fn handleRead
     */
    virtual void handleRead ();

private:
    QLocalServer* m_server;
    QLocalSocket* m_sckt;
    QLocalSocket* m_brdcstSckt;
    static LocalServer s_lclSrv;
};

/**
 * @brief
 *
 * @class TcpServer system.hpp "src/system.hpp"
 */
class TcpServer : public Server
{
    Q_OBJECT

public:
    /**
     * @brief
     *
     * @fn TcpServer
     */
    TcpServer();

    /**
     * @brief
     *
     * @fn TcpServer
     * @param
     */
    TcpServer (const TcpServer&);

    /**
     * @brief
     *
     * @fn ~TcpServer
     */
    virtual ~TcpServer();

    /**
     * @brief
     *
     * @fn start
     */
    virtual void start ();

    /**
     * @brief
     *
     * @fn stop
     */
    virtual void stop ();

    /**
     * @brief
     *
     * @fn send
     * @param
     */
    virtual void send (const Message&);

    /**
     * @brief
     *
     * @fn isActive
     */
    virtual const bool isActive() const;

    /**
     * @brief
     *
     * @fn features
     */
    virtual const Features features() const;

    /**
     * @brief
     *
     * @fn protocol
     */
    virtual const QString protocol () const;

protected:
    /**
     * @brief
     *
     * @fn connectToSocket
     */
    virtual void connectToSocket ();

    /**
     * @brief
     *
     * @fn disconnectFromSocket
     */
    virtual void disconnectFromSocket ();

    /**
     * @brief
     *
     * @fn handleConnected
     */
    virtual void handleConnected ();

    /**
     * @brief
     *
     * @fn handleConnection
     */
    virtual void handleConnection ();

    /**
     * @brief
     *
     * @fn handleRead
     */
    virtual void handleRead ();

private:
    static TcpServer s_tcpSrv;
    QTcpServer* m_server;
    QTcpSocket* m_socket;
    QTcpSocket* m_broadcastSocket;
};
}
}

#endif // SERVER_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
