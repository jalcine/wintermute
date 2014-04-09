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

#include "Wintermute/logging.hpp"
#include "Wintermute/Procedure/receiver.hpp"

namespace Wintermute
{
namespace Procedure
{
class ReceiverPrivate
{
public:
	static ReceiverList receivers;

	static bool isReceiverKnown ( Receiver* receiver ) {
		Q_FOREACH ( Receiver * aReceiver, ReceiverPrivate::receivers ) {
			if ( aReceiver->metaObject()->className() ==
			     receiver->metaObject()->className() ) {
				return true;
			}
		}
		return false;
	}

	static bool addReceiver ( Receiver* receiver ) {
		if ( !receiver ) {
			return false;
		}
		if ( isReceiverKnown ( receiver ) ) {
			wdebug ( receiver, "Already added into receiver pool." );
			return false;
		}
		ReceiverPrivate::receivers << receiver;
		wdebug ( receiver,
		         QString ( "%1 added to receiver pool." )
		         .arg ( receiver->metaObject()->className() ) );
		return true;
	}

	static bool removeReceiver ( Receiver* receiver ) {
		if ( !isReceiverKnown ( receiver ) ) {
			return false;
		}
		ReceiverPrivate::receivers.removeAll ( receiver );
		wdebug ( receiver,
		         QString ( "%1 removed from receiver pool." )
		         .arg ( receiver->metaObject()->className() ) );
		return true;
	}
};
}
}
