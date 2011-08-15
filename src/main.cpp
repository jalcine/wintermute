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
#include <QCoreApplication>
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
/// @todo Add a hang here waiting for user input. But what should it be waiting for? Most likely for the linguistics module. For now, it'll just do something similar to the WntrLing CLI.
/// @bug Issue attempting to pass an argument 'parse' to Wintermute to have it interpret a sentence. The Boost command line interface may be haphazardly set up.
int main ( int argc, char** argv ) {
    Core::Configure ( argc , argv );
    Core::Initialize ();

    if (IPC::currentModule () == "master"){
        Linguistics::Parser aParser("en");
        QTextStream qin ( stdin );
        QString line;

        cout << "** Enter '*quit*' to exit linguistics parsing. **" << endl;

        while ( line != "*quit*" ) {
            cout << "** Statement: ?] ";
            line = qin.readLine ();
            aParser.process ( line.toStdString () );
            cout.flush ();
        }
    }
    return 0;
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;
