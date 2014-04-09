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

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QVariant>
#include <QtCore/QVariantList>
#include <QtCore/QVariantMap>
#include <Wintermute/Logging>
#include <Wintermute/Procedure/Module>
#include <Wintermute/Procedure/LambdaCall>
#include <Wintermute/Procedure/MethodCall>
#include <Wintermute/Procedure/ReplyCall>
#include "globals.hpp"

namespace Wintermute
{
namespace Heartbeat
{
class PulseModule;
class PulseModulePrivate
{
public:
	QTimer timer;
	PulseModule* q_ptr;
	quint64 count;
	Q_DECLARE_PUBLIC ( PulseModule );

	PulseModulePrivate ( PulseModule* q ) :
		timer ( q ), q_ptr ( q ), count ( 0 ) {
		timer.setInterval ( WINTERMUTE_HEARTBEAT_INTERVAL );
		timer.setSingleShot ( true );
		QObject::connect ( &timer, SIGNAL(timeout()), q, SLOT(tick()) );
	}

	void
	mountCalls() {
		Q_Q ( PulseModule );
		q->mountCall ( new Procedure::LambdaCall ( "module", q,
		[&] (QVariant args, const Procedure::MethodCall& call) -> void {
			const QVariant module = getModuleInfo(args);
      Procedure::ReplyCall reply = call.craftReply(module);
      reply.queueForDispatch();
		} ) );
		q->mountCall ( new Procedure::LambdaCall ( "modules", q,
		[&] (QVariant , const Procedure::MethodCall& call) -> void {
			const QVariant modules = getAllModulesInfo();
      Procedure::ReplyCall reply = call.craftReply(modules);
      reply.queueForDispatch();
		} ) );
	}

	QVariantMap
	getModuleInfo ( const QVariant& aDef ) {
		QPointer<Procedure::Module> module = 
      Procedure::Module::findModule ( aDef.value<Procedure::Module::Definition>() );
		QVariantMap values;
		if ( !module.isNull() ) {
			values.insert ( "calls",   module->calls() );
		}
		return values;
	}

	QVariantHash
	getAllModulesInfo() {
		QVariantHash modules;
		for (QPointer<Procedure::Module> mod: Procedure::Module::knownModules()) {
			modules.insert ( mod->definition(), 
          getModuleInfo ( QVariant::fromValue(mod->definition() ) ) );
		}
		return modules;
	}
};
}
}
