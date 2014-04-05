/**
 * Copyright (C) 2011 - 2014 Jacky Alciné <me@jalcine.me>
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

#include <QtCore/QMap>
#include <QtCore/QPointer>
#include "Wintermute/logging.hpp"
#include "Wintermute/application.hpp"
#include "Wintermute/Procedure/module.hpp"
#include "Wintermute/Procedure/module_call.hpp"
#include "Wintermute/private/application.hpp"

namespace Wintermute
{
class ApplicationPrivate;
namespace Procedure
{
class Module;
class ModuleCall;
class ModulePrivate
{
	Q_DECLARE_PUBLIC ( Module );

public:
	static QMap<QString, QPointer<Module>> modules;
	QPointer<Module> q_ptr;
	QString package;
	QString domain;
	QMap<QString, QPointer<ModuleCall>> calls;

	ModulePrivate ( Module* q ) : q_ptr ( q ), package ( QString::null ),
		domain ( QString::null ), calls ( ) {
		winfo ( q_ptr, "Module setting up..." );
	}

	void checkQualifiedName() {
		if ( domain.isEmpty() || package.isEmpty() ) {
			return;
		}
		QPointer<Module> module = Module::findModule ( q_ptr->qualifiedName() );
		if ( module.isNull() ) {
			ModulePrivate::modules.insert ( q_ptr->qualifiedName(), q_ptr );
			//Q_EMIT wntrApp->addedModule ( q_ptr->qualifiedName() );
		}
	}

	virtual ~ModulePrivate () {
		ModulePrivate::modules.remove ( q_ptr->qualifiedName() );
		//Q_EMIT wntrApp->removedModule ( q_ptr->qualifiedName() );
	}
};
} /* Procedure */
} /* Wintermute  */
