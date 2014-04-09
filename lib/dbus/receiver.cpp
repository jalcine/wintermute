/**
 * @author Jacky Alciné <me@jalcine.me>
 * @copyright © 2011, 2012, 2013, 2014 Jacky Alciné <me@jalcine.me>
 * @if 0
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
 * @endif
 **/

#include <Wintermute/Logging>
#include <QtDBus/QDBusPendingReply>
#include "adaptor.hpp"
#include "receiver.hpp"

using Wintermute::DBus::Receiver;
using Wintermute::DBus::Adaptor;
using Wintermute::Procedure::Message;

Receiver::Receiver() :
	Wintermute::Procedure::Receiver()
{
}

void
Receiver::receiveMessage ( const Message& message )
{
	// TODO Some meta-data about D-Bus or call origin could be added.
	Wintermute::Procedure::Receiver::receiveMessage(message);
}

void
Receiver::handleAsyncCallReply ( QDBusPendingCallWatcher* reply )
{
	QDBusPendingReply<QVariant> replyValue = *reply;
	if ( replyValue.isValid() && replyValue.isFinished() ) {
    const Message theMessage = replyValue.value().value<Message>();
    receiveMessage(theMessage);
	}
	reply->deleteLater();
}

Receiver::~Receiver()
{
}
