/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2011 - 2013 Jacky Alciné <me@jalcine.me>
 *
 * Wintermute is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "Wintermute/Procedure/call.hpp"
#include "Wintermute/Procedure/module.hpp"
#include "Wintermute/logging.hpp"
#include <QtCore/QMap>
#include <QtNetwork/QLocalSocket>
#include <QtNetwork/QLocalServer>

namespace Wintermute
{
class ApplicationPrivate;
namespace Procedure
{
class Module;
class ModulePrivate
{
  Q_DECLARE_PUBLIC ( Module );

public:
  Module* q_ptr;
  QString package;
  QString domain;
  QLocalSocket* socket;
  QLocalServer* server;
  QMap<QString, CallPointer> calls;

  ModulePrivate ( Module* q ) :
    q_ptr ( q ), package ( "" ), domain ( "" ),
    socket ( new QLocalSocket ( q ) ), server ( new QLocalServer ( q ) ), calls() {
  }

  void connectToWire() {
    socket->connectToServer ( "/tmp/wintermute.socket" );
    server->listen ( "/tmp/wintermute.socket" );
    winfo ( q_ptr, "Listening & speaking at '/tmp/wintermute.socket' on this local machine." );
  }

  void disconnectFromWire() {
    socket->disconnectFromServer();
    server->close();
  }

  void sendData ( const QString& data ) {
    socket << data.toLocal8Bit();
  }

  void recieveDataAsync ( std::function<void ( QVariant ) > callback ) {
    // TODO: Recieve data in async.
    // TODO: Use a one-shot signal here.
    callback ( QVariant() );
  }

  QVariant receiveData() {
    QByteArray data;
    data.resize ( socket->bytesAvailable() );
    data = socket->read ( socket->bytesAvailable() );
    return data;
  }

  virtual ~ModulePrivate () {
    disconnectFromWire();
    winfo ( q_ptr, "We out!" );
  }
};
} /* Procedure */
} /* Wintermute  */
