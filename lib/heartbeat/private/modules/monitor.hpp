/**
 * @author Jacky Alciné <me@jalcine.me>
 * @copyright © 2013, 2014 Jacky Alciné <me@jalcine.me>
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

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <Wintermute/Logging>
#include <Wintermute/Procedure/LambdaCall>
#include <Wintermute/Procedure/ReplyCall>

namespace Wintermute
{
namespace Heartbeat
{
class MonitorModulePrivate
{
public:
	QMap<quint64, QVariantMap> processes;
	MonitorModule* q_ptr;
	Q_DECLARE_PUBLIC ( MonitorModule );

	MonitorModulePrivate ( MonitorModule* q ) :
		processes ( QMap<quint64, QVariantMap>() ), q_ptr ( q ) {
		winfo(q, "Prepped to monitor this system.");
	}

	void
	mountCalls() {
    q_ptr->mountCall( new Procedure::LambdaCall ( "greet",
		  q_ptr, [&] (QVariant args, const Procedure::MethodCall& call) -> void {
        wdebug(q_ptr, "We got someone saying hello.");
        QVariantList theArgs(args.toList());
        theArgs.push_back(call.sendingModule().pid);
        QVariant greetResult = greet(theArgs);
        Procedure::ReplyCall reply = call.craftReply(greetResult);
        reply.queueForDispatch();
		} ) );
    q_ptr->mountCall(new Procedure::LambdaCall ( "record", 
        q_ptr, [&] (QVariant args, const Procedure::MethodCall& call) -> void {
			wdebug(q_ptr, "Entering a new beating record.");
      QVariant recordResult = record(args.toList());
      Procedure::ReplyCall reply = call.craftReply(recordResult);
      reply.queueForDispatch();
		} ) );
	}

	void
	killOtherMonitors() {
		// TODO: Find other monitor instances on this local machine.
		// TODO: Kill each instance.
	}

	QVariant greet ( const QVariantList& arguments ) {
		QVariantMap newEntry;
		newEntry.insert ( "name", arguments[0] );
		newEntry.insert ( "pid",  arguments[1] );
		processes.insert ( arguments[0].toUInt(), newEntry );
		winfo ( q_ptr, QString ( "Hello %1 from %2!" ).
		        arg ( arguments[0].toString(), arguments[1].toString() ) );
		return arguments[0].toUInt();
	}

	/**
	 * @fn record
	 * @param QVariantList arguments
	 *        - count: A count of the number of total pings sent.
	 *        - type:  The type of ping this is.
	 */
	QVariant record ( const QVariantList& arguments ) {
		quint64 pid = arguments[2].toUInt();
		if ( processes.contains ( pid ) ) {
			processes[pid]["count"] = arguments[0].toInt();
		} else {
			QVariantMap entry;
			entry.insert ( "count",   arguments[0].toInt() );
			entry.insert ( "signal",  arguments[1].toInt() );
			processes.insert ( pid, entry );
		}
		return processes.contains ( pid );
	}
};
}
}
