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

#include "config.hpp"
#include "core.hpp"
#include "ipc.hpp"
#include "plugins.hpp"
#include <iostream>
#include <wntrdata.hpp>
#include <wntrntwk.hpp>
#include <boost/program_options.hpp>

using namespace std;
using namespace Wintermute;
namespace po = boost::program_options;


using boost::program_options::variables_map;
using boost::program_options::options_description;
using std::cout;
using std::endl;

namespace Wintermute {
    QCoreApplication* Core::m_app = NULL;

    void Core::Configure ( int& argc, char** argv ) {
        if ( !m_app ) {
            m_app = new QCoreApplication ( argc , argv );
            m_app->setApplicationName ( "Wintermute" );
            m_app->setApplicationVersion( QString::number (WINTERMUTE_VERSION) );
            m_app->setOrganizationDomain ( "org.thesii.Wntr" );
            m_app->setOrganizationName ( "Synthetic Intellect Institute" );
        }

        cout << qPrintable(QCoreApplication::applicationName ()) << " " << qPrintable(QCoreApplication::applicationVersion ()) << " (pid " << QCoreApplication::applicationPid () << ") :: "
             << "Artificial intelligence for common Man." << endl;

        variables_map vm;
        options_description desc ( "Options" );

        desc.add_options ()
        ( "locale,lcl" , po::value<string>(), "Defines the locale used by the system for parsing. (default: 'en')")
        ( "ipc,module" , po::value<string>(),"Defines the IPC module to run this process as. (default: 'master')" );

        string ipcModule("master");
        desc.add_options()
        ( "help","show help screen" );

        po::notify ( vm );
        po::store ( po::parse_command_line ( QCoreApplication::argc (), QCoreApplication::argv () , desc ), vm );

        if ( !vm.empty () ) {
            if ( vm.count ( "help" ) ) {
                /// @todo Render a set of text to be used for the help screen.
                cout << "\"There's no help for those who lack the valor of mighty men!\"" << endl
                     << desc << endl << endl
                     << "If you want more help and/or information, visit <http://www.thesii.org> to" << endl
                     << "learn more about Wintermute or visit us on IRC (freenode) in ##sii-general." << endl;

                exit (0);
            }

            if ( vm.count ( "ipc" ) ) {
                if ( vm["ipc"].as<string>() == "master" || vm["ipc"].as<int>() == 0 )		  ipcModule = "master";
                else if ( vm["ipc"].as<string>() == "network" || vm["ipc"].as<int>() == 1 )   ipcModule = "network";
                else if ( vm["ipc"].as<string>() == "plugin" || vm["ipc"].as<int>() == 2 )    ipcModule = "plugin";
            } else ipcModule = "master";

            if ( vm.count ("locale") )
                Wintermute::Data::Linguistics::Configuration::setLocale (vm["locale"].as<string>());
        } else
            cout << "(core) [Core] Run this application with '--help' to get help information." << endl;

        IPC::Initialize(ipcModule);
    }

    void Core::Initialize() {
        if (IPC::currentModule() == "master")
            Wintermute::Data::Configuration::Initialize();
        else if (IPC::currentModule() == "network")
            Wintermute::Network::Initialize ();
        Wintermute::Plugins::Factory::Startup ();
    }

    void Core::Deinitialize() {
        if (IPC::currentModule() == "master")
            Wintermute::Data::Configuration::Deinitialize ();
        else if (IPC::currentModule() == "network")
            Wintermute::Network::Deinitialize ();
        Wintermute::Plugins::Factory::Shutdown ();
    }
}
