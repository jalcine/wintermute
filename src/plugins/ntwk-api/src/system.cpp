/**
 * @file    server.cpp
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

#include "system.hpp"
#include <QUuid>
#include <QFile>
#include <QtDebug>
#include <QHostInfo>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QLocalServer>
#include <QLocalSocket>
#include <QUdpSocket>

namespace Wintermute {
namespace Network {
System System::s_system;
System::ServerList System::s_servers;
LocalServer LocalServer::s_lclSrv;
TcpServer TcpServer::s_tcpSrv;

System::System( ) : QObject() { }

System::System ( const System& p_systm ) { }

void System::start () {
    foreach ( Server* p_srvr, s_servers )
    p_srvr->start ();

    qDebug() << "(ntwk) [System]" << s_servers.size () << "server(s) started.";
}

void System::stop () {
    foreach ( Server* p_srvr, s_servers )
    p_srvr->stop ();

    qDebug() << "(ntwk) [System]" << s_servers.size () << "server(s) stopped.";
}

System* System::instance () {
    return &s_system;
}

void System::send ( const Message &p_msg, const Features &p_ftr ) {
    foreach ( Server* p_srvr, s_servers ) {
        if ( p_srvr->features () == p_ftr || p_ftr == None )
            p_srvr->send ( p_msg );
    }
}

void System::send ( const Message &p_msg, const QString &p_ptrl ) {
    QStringList clnts = allServers();
    const int indx = clnts.indexOf ( p_ptrl );

    if ( indx != -1 )
        s_servers.at ( indx )->send ( p_msg );
}

const QStringList System::allServers () {
    QStringList srvrs;

    foreach ( const Server* srvr, s_servers )
    srvrs << srvr->protocol();

    return srvrs;
}

const bool System::isActive() {
    foreach ( const Server* srvr, s_servers ) {
        if ( srvr->isActive () )
            return true;
    }

    return false;
}

/// @todo Be sure to prohibit the user from having qualifiers that are reserved qualifiers.
/// @todo Where should the qualifiers list be stored?
/// @todo Doing a host-name lookup could return a list of hosts. Should we return that list?
/// @todo QHostInfo seems to require an async host name lookup.
const QHostAddress System::toHostAddress ( const QString &p_qual ) {
    if ( p_qual.toLower () == "broadcast" )
        return QHostAddress::Broadcast;
    else if ( p_qual.toLower () == "self" )
        return QHostAddress::LocalHost;

    if ( p_qual.indexOf ( "wntr::" ) == 0 ) {
    } else {
        QHostInfo hstInf = QHostInfo::fromName ( p_qual );
        if ( !hstInf.addresses().empty() )
            return hstInf.addresses ().front ();
        else
            return QHostAddress::Broadcast;
    }

    return QHostAddress::Broadcast;
}

const QString System::toQualifier ( const QHostAddress &p_hostAddr ) {
    return "broadcast";
}

System::~System () {  }

Server::Server() {
    System::s_servers.append ( this );
    connect ( System::instance (),SIGNAL ( messageRecieved ( Message ) ), this,SIGNAL ( messageRecieved ( Message ) ) );
}

Server::Server ( const Server &p_srvr )  { }

Server::~Server () { }

LocalServer::LocalServer() : Server(), m_server ( 0 ), m_sckt ( 0 ) { }

LocalServer::LocalServer ( const LocalServer &p_srvr ) : Server ( p_srvr ),
    m_server ( p_srvr.m_server ), m_sckt ( p_srvr.m_sckt ) {
    qDebug() << "(ntwk) [LocalServer] New local server.";
}

void LocalServer::disconnectFromSocket() {
    qDebug() << "(ntwk) [LocalServer] Stopping local server" << m_server->serverName ();
    m_server->close ();
}

void LocalServer::connectToSocket() {
    QUuid uuid = QUuid::createUuid();
    m_server->listen ( "Wintermute_" + uuid.toString () );

    if ( m_server->isListening () ) {
        qDebug() << "(ntwk) [LocalServer] Starting local server with name" << m_server->serverName ();
        connect ( m_server,SIGNAL ( newConnection() ),this,SLOT ( handleConnection() ) );

        m_brdcstSckt = new QLocalSocket ( this );
        m_brdcstSckt->connectToServer ( "Wintermute",QLocalSocket::ReadOnly );
        connect ( m_brdcstSckt,SIGNAL ( connected() ),this,SLOT ( handleConnected() ) );
    } else
        qDebug() << "(ntwk) [LocalServer] Failed to listen:" << m_server->errorString ();
}

/// @todo Where do we send a Message if it's to be sent to the broadcast server?
void LocalServer::send ( const Message& p_msg ) {
    m_messgeQueue.enqueue ( &p_msg );
    const QString hst = p_msg.property ( "Recipient" ).toString ();

    if ( hst.toLower() == "broadcast" ) {

    } else if ( QFile::exists ( "/tmp/" + hst ) ) {
        m_sckt = new QLocalSocket ( this );
        connect ( m_sckt,SIGNAL ( connected() ), this,SLOT ( handleConnected() ) );
        m_sckt->connectToServer ( hst );
    } else {
        //qDebug() << "(ntwk) [LocalServer] Host not found at" << hst;
    }
}

/// @todo Send a queued Message, close the socket and then request that the Server sent the next message.
/// @todo Determine if the method 'toUft8' is a better solution than 'toLocal8Bit'.
void LocalServer::handleConnected() {
    if ( m_sckt->isValid () ) {
        qDebug() << "(ntwk) [LocalServer] Socket connected to" << m_sckt->serverName();
        const Message* msg = m_messgeQueue.dequeue();
        const QByteArray buffer = msg->toString ().toUtf8 ();
        if ( m_sckt->write ( buffer ) == buffer.size () ) {
            m_sckt->disconnectFromServer ();
            m_sckt->deleteLater ();
        }
    } else
        qDebug() << "(ntwk) [LocalServer]" << m_sckt->errorString ();

    if ( m_brdcstSckt->isValid () ) {
        qDebug() << "(ntwk) [LocalServer] Socket connected to" << m_sckt->serverName();
        connect ( m_brdcstSckt,SIGNAL ( readyRead() ),this,SLOT ( handleRead() ) );
    } else
        qDebug() << "(ntwk) [LocalServer]" << m_brdcstSckt->errorString ();
}

/// @todo Find a way to have this handle multiple connections at once.
void LocalServer::handleConnection() {
    while ( m_server->hasPendingConnections () ) {
        m_sckt = m_server->nextPendingConnection ();
        if ( m_sckt->isValid () && m_sckt->isReadable () && m_sckt->size () > 0 ) {
            qDebug() << "(ntwk) [LocalServer] Incoming connection from" << m_sckt->serverName () << "; bytes:" << m_sckt->size ();
            connect ( m_sckt,SIGNAL ( readyRead() ),this,SLOT ( handleRead() ) );
        } else
            qDebug() << "(ntwk) [LocalServer] Incoming connection failed:" << m_sckt->errorString ();
    }
}

/// @todo Determine if the data's a properly formed Message before converting it into a Message.
void LocalServer::handleRead() {
    if ( m_sckt->isValid () && m_sckt->isReadable () ) {
        qDebug() << "(ntwk) [LocalServer] Reading data from" << m_sckt << "; bytes:" << m_sckt->size ();
        QByteArray buffer = m_sckt->readAll ();
        qDebug() << buffer;
        const Message* msg = Message::fromString ( buffer );
        emit messageRecieved ( *msg );
    } else
        qDebug() << m_sckt->errorString ();

    if ( m_brdcstSckt->isValid () && m_brdcstSckt->isReadable () ) {
        qDebug() << "(ntwk) [LocalServer] Reading data from" << m_brdcstSckt << "; bytes:" << m_sckt->size ();
        QByteArray buffer = m_brdcstSckt->readAll ();
        qDebug() << buffer;
        const Message* msg = Message::fromString ( buffer );
        emit messageRecieved ( *msg );
    } else
        qDebug() << m_brdcstSckt->errorString ();
}

void LocalServer::stop () {
    if ( m_server ) {
        const QString socketName = m_server->serverName ();
        disconnectFromSocket();
        m_server->blockSignals ( true );
        m_server->deleteLater ();
        QFile::remove ( "/tmp/" + socketName );
    }
}

void LocalServer::start () {
    if ( !m_server ) {
        m_server = new QLocalServer ( this );
        connectToSocket();
    }
}

const QString LocalServer::protocol () const {
    return "local";
}

const Features LocalServer::features () const {
    return Reliable;
}

const bool LocalServer::isActive () const {
    return m_server != 0 && m_server->isListening ();
}

LocalServer::~LocalServer () { }

TcpServer::TcpServer() : m_server ( 0 ), m_socket ( 0 ), m_broadcastSocket ( 0 ), Server ( *this ) { }

TcpServer::TcpServer ( const TcpServer& p_srvr ) : m_server ( p_srvr.m_server ),
    m_broadcastSocket ( 0 ), m_socket ( p_srvr.m_socket ), Server ( p_srvr ) { }

void TcpServer::start() {
    if ( !m_server ) {
        m_server = new QTcpServer ( this );
        m_broadcastSocket = new QTcpSocket ( m_server );
        connectToSocket();
    }
}

void TcpServer::stop() {
    if ( m_server ) {
        disconnectFromSocket();
        m_server->blockSignals ( true );
        m_server->deleteLater();
        m_broadcastSocket->blockSignals ( true );
        m_broadcastSocket->deleteLater();
    }
}

void TcpServer::connectToSocket() {
    quint16 port = 1300;
    if ( m_server->listen ( QHostAddress::Broadcast ) )
        qDebug() << "(ntwk) [TcpServer] Started TCP server at" << m_server->serverAddress().toString() << ":" << m_server->serverPort();
    else
        qDebug() << "(ntwk) [TcpServer]" << m_server->errorString();
    connect ( m_server,SIGNAL ( newConnection() ),this,SLOT ( handleConnection() ) );

    m_broadcastSocket->connectToHost ( QHostAddress::Broadcast,port );
    connect ( m_broadcastSocket,SIGNAL ( readyRead() ), this,SLOT ( handleRead() ) );
}

void TcpServer::disconnectFromSocket() {
    qDebug() << "(ntwk) [TcpServer] Stopping TCP server" << m_server->serverAddress ().toString () << m_server->serverPort ();
    m_broadcastSocket->disconnectFromHost();
    m_server->close();
}

void TcpServer::send ( const Message& p_msg ) {
    m_messgeQueue.enqueue ( &p_msg );
    const QString hostName = p_msg.property ( "Recipient" ).toString ();
    const QHostAddress hostAddress = System::toHostAddress ( hostName );

    if ( hostAddress != QHostAddress::Null ) {
        int hostPort = 0;
        if ( hostAddress == QHostAddress::Broadcast || !hostName.contains ( ":" ) )
            hostPort = 1300;
        else
            hostPort = hostName.split ( ":" ).at ( 1 ).toInt ();

        m_socket = new QTcpSocket ( this );
        connect ( m_socket,SIGNAL ( connected() ), this,SLOT ( handleConnected() ) );
        qDebug() << "(ntwk) [TcpServer] Connecting to " << hostAddress << hostPort;
        m_socket->connectToHost ( hostAddress,hostPort,QIODevice::ReadOnly );
    } else {
        qWarning() << "(ntwk) [TcpServer] Host not found at" << hostName;
    }
}

/// @todo Implement a means for this to handle multiple connections.
void TcpServer::handleConnection() {
    while ( m_server->hasPendingConnections () ) {
        m_socket = m_server->nextPendingConnection ();
        if ( m_socket->isValid () && m_socket->isReadable () && m_socket->size () > 0 ) {
            qDebug() << "(ntwk) [TcpServer] Incoming connection from" << m_socket->peerName () << "; bytes:" << m_socket->size ();
            connect ( m_socket,SIGNAL ( readyRead() ),this,SLOT ( handleRead() ) );
        } else
            qDebug() << "(ntwk) [TcpServer] Incoming connection failed:" << m_socket->errorString ();
    }
}

void TcpServer::handleConnected() {
    if ( m_socket->isValid () ) {
        qDebug() << "(ntwk) [TcpServer] Socket connected to (" << m_socket->peerName() << ")"<< m_socket->peerAddress() << ":" << m_socket->peerPort();
        const Message* msg = m_messgeQueue.dequeue();
        const QByteArray buffer = msg->toString ().toUtf8 ();
        if ( m_socket->write ( buffer ) == buffer.size () ) {
            m_socket->disconnectFromHost ();
            m_socket->deleteLater ();
        }
    } else
        qDebug() << "(ntwk) [TcpServer]" << m_socket->errorString ();
}

void TcpServer::handleRead() {
    if ( m_socket->isValid () && m_socket->isReadable () ) {
        qDebug() << "(ntwk) [TcpServer] Reading data from" << m_socket << "; bytes:" << m_socket->size ();
        QByteArray buffer = m_socket->readAll ();
        qDebug() << buffer;
        const Message* msg = Message::fromString ( buffer );
        emit messageRecieved ( *msg );
    } else
        qDebug() << m_socket->errorString ();

    if ( m_broadcastSocket->isValid() && m_broadcastSocket->isReadable() ) {
        qDebug() << "(ntwk) [TcpServer] Reading data from" << m_broadcastSocket << "; bytes:" << m_socket->size ();
        QByteArray buffer = m_socket->readAll ();
        qDebug() << buffer;
        const Message* msg = Message::fromString ( buffer );
        emit messageRecieved ( *msg );
    } else
        qDebug() << m_broadcastSocket->errorString ();
}

const QString TcpServer::protocol() const {
    return "tcp";
}

const Features TcpServer::features() const {
    return Reliable;
}

const bool TcpServer::isActive() const {
    return m_server != 0 && m_server->isListening();
}

TcpServer::~TcpServer() { }
}
}

#include "system.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
