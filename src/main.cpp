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
 */

#include <string>
#include <iostream>
#include "wintermute.hpp"

using namespace std;
using namespace Wintermute;
using namespace Wintermute::Data::Ontology;

using std::cout;
using std::cin;
using std::endl;
using std::string;
using Wintermute::Core;
using Wintermute::Data::Ontology::Store;
using Wintermute::Linguistics::Parser;

/// @todo Add manipulation switches for IPC (via D-Bus/QtBus) here.
/// @todo Add a hang here waiting for user input. But what should it be waiting for? Most likely for the linguistics module. For now, it'lll just do something similar to the WntrLing CLI.
int main (int argc, char** argv) {
	string ontoName("COSMO"), ipcModule, preline;
	variables_map vm;
	options_description desc("Wintermute Options");
	desc.add_options ()
			("ontology","the core ontology to be loaded (default: 'core')")
			("locale"  ,"the initial locale to use (default: 'en')")
			("parse"   ,"a sample sentence to analyze")
			("ipc"     ,"the IPC module to run this process as. (default: 'master')");
	Core::manageCmdLine(vm,desc);
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	if (vm.count ("ontology") && !vm.empty ())
		ontoName = vm["ontology"].as<string>();

	if (vm.count ("ipc")){
		if (vm["ipc"].as<string>() == "master" || vm["ipc"].as<int>() == 0) // master module
			ipcModule = "master";
		if (vm["ipc"].as<string>() == "network" || vm["ipc"].as<int>() == 1) // networking module
			ipcModule = "network";
		if (vm["ipc"].as<string>() == "gui" || vm["ipc"].as<int>() == 2) // gui, front-end.
			ipcModule = "gui";
	} else ipcModule = "master";

	if (vm.count ("parse"))
		preline = vm["parse"].as<string>();

	IPC::Process(ipcModule);

	/// @note Method may be modified depending on what IPC module it's running.
	Core::Initialize ();

	//Store* aNewStore = Store::obtain(ontoName);

	Linguistics::Parser aParser("en");

	if (!preline.empty ()){
		cout << "(core) Parsing '" << preline << "'..." << endl;
		aParser.process (preline);
	}

	QTextStream qin(stdin);
	QString line;
	cout << "** Enter '*quit*' to exit linguistics parsing. **" << endl;
	while (line != "*quit*"){
		cout << "** Statement: ?] ";
		line = qin.readLine ();
		aParser.process (line.toStdString ());
		//cout << "** " << aParser.getLexicalCount () << " lexicons have been used in total." << endl;
		cout.flush ();
	}
	return 0;
}
