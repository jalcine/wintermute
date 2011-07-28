/**
 * @author Jacky Alcine <jackyalcine@gmail.com>
 *
 * @legalese
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
 * @endlegalese
 * @todo Consider splitting up the master Wintermute process into a master process and multiple sub processes (one master process, one child process managing the network, one child managing the plugin system, etc).
 */

#include <QDebug>
#include "wintermute.hpp"

namespace Wintermute {
	void Core::Initialize(){
		// Wintermute::Network::Initialize();
		Wintermute::Data::Configuration::Initialize();
		Wintermute::Plugins::Factory::Startup ();
	}

	void Core::Deinitialize(){
		// Wintermute::Network::Deinitialize();
		Wintermute::Data::Configuration::Deinitialize();
		Wintermute::Plugins::Factory::Shutdown ();
	}

	void Core::manageCmdLine(variables_map &vm, options_description &desc){
		desc.add_options()
			("help","show help screen")
			("debug","run in debug mode");

		boost::program_options::notify(vm);

		if (!vm.empty ()){
			if (vm.count("help")){
				/// @todo Generate help screen text.
				cout << "\"There's no help for those who lack the valor of mighty men!\"" << endl
					 << desc << endl;
			} else if (vm.count("debug")){
				/// @todo Toggle a DEBUG flag within Wintermute. May need to implement a logging system of sorts.
			}
		}
		else {
			cout << " ** Run this application with '--help' to get help information." << endl;
		}
	}
}
