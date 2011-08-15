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

#include <sstream>
#include "config.hpp"
#include "wintermute.hpp"
#include <QCoreApplication>

namespace Wintermute {
    QCoreApplication* Core::m_app = NULL;
    void Core::Configure ( int& argc, char** argv ) {
        if ( !m_app ) {
            ostringstream l_d;
            l_d << WINTERMUTE_VERSION;
            m_app = new QCoreApplication ( argc,argv );
            m_app->setApplicationName ( "Wintermute" );
            m_app->setApplicationVersion(QString(l_d.str ().c_str ()));
            m_app->setOrganizationDomain ( "org.thesii.Wntr" );
            m_app->setOrganizationName ( "Synthetic Intellect Institute" );
        }

        cout << qPrintable(QCoreApplication::applicationName ()) << " " << qPrintable(QCoreApplication::applicationVersion ()) << " (pid " << QCoreApplication::applicationPid () << ") :: "
             << "Artificial intelligence for common Man." << endl;

        variables_map vm;
        options_description desc ( "Options" );

        desc.add_options ()
        ( "locale"  ,"Defines the locale used by the system for parsing. (default: 'en')")
        ( "ipc"     ,"Defines the IPC module to run this process as. (default: 'master')" );
        Core::manageCmdLine(vm,desc);
    }

    void Core::Initialize() {
        Wintermute::Plugins::Factory::Startup ();
        if (IPC::currentModule() == "master")
            Wintermute::Data::Configuration::Initialize();
        else if (IPC::currentModule() == "network")
            Wintermute::Network::Initialize ();
    }

    void Core::Deinitialize() {
        Wintermute::Plugins::Factory::Shutdown ();
        if (IPC::currentModule() == "master")
            Wintermute::Data::Configuration::Deinitialize ();
        else if (IPC::currentModule() == "network")
            Wintermute::Network::Deinitialize ();
    }

    void Core::manageCmdLine ( variables_map &vm, options_description &desc ) {
        string ipcModule("master");
        desc.add_options()
        ( "help","show help screen" );

        boost::program_options::notify ( vm );
        boost::program_options::store ( boost::program_options::parse_command_line ( QCoreApplication::argc (), QCoreApplication::argv () , desc ), vm );

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
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;
