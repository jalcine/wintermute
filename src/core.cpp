/**
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
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

#ifndef WINTERMUTE_USING_GUI
#include "ncurses.hpp"
#endif
#include "config.hpp"
#include "core.hpp"
#include "ipc.hpp"
#include "plugins.hpp"
#include <iostream>
#include <QtDebug>
#include <wntrdata.hpp>
#include <wntrntwk.hpp>
#include <wntrling.hpp>
#include <QProcess>
#include <QtDebug>
#include <QVariantMap>
#include <boost/program_options.hpp>

using namespace std;
using namespace Wintermute;
namespace po = boost::program_options;


using boost::program_options::variables_map;
using boost::program_options::options_description;
using Wintermute::Linguistics::Parser;
using std::cout;
using std::endl;

namespace Wintermute {
    WNTR_APPLICATION* Core::s_app = NULL;
    QVariantMap* Core::s_args = NULL;
    Core* Core::s_core = NULL;

    Core::Core(int &p_argc, char **p_argv) {
        Core::s_core = this;
        Core::Configure (p_argc,p_argv);
        ::QObject(s_app);

        s_core->connect(WNTR_APPLICATION::instance (),SIGNAL(aboutToQuit()), Core::instance (),SLOT(doDeinit()));
    }

    void Core::Configure ( int& p_argc, char** p_argv ) {
        int l_argc = p_argc;
        string ipcModule = "master";
        s_app = new WNTR_APPLICATION(p_argc,p_argv);
        s_app->setApplicationName ( "Wintermute" );
        s_app->setApplicationVersion( QString::number (WINTERMUTE_VERSION) );
        s_app->setOrganizationDomain ( "org.thesii.Wintermute" );
        s_app->setOrganizationName ( "Synthetic Intellect Institute" );

        cout << qPrintable(s_app->applicationName ()) << " "
             << qPrintable(s_app->applicationVersion ())
             << " (pid " << s_app->applicationPid () << ") :: "
             << "Artificial intelligence for common Man." << endl
             << "Licensed under the GPL v3 license." << endl;

#ifdef WINTERMUTE_USING_GUI
            cout << "(core) [Core] Compiled with graphical user interface enabled." << endl;
#endif

        configureCommandLine();

        if (s_args->count ("ipc") != 0)
            ipcModule = s_args->value ("ipc").toString ().toStdString ();

        IPC::Initialize(ipcModule);
    }

    void Core::configureCommandLine () {
        variables_map l_vm;
        s_args = new QVariantMap;
        int l_argc = s_app->argc ();
        char** l_argv = s_app->argv ();
        options_description l_publicOptions ( "General Options" ),
                            l_configOptions ( "Configuration" ),
                            l_hiddenOptions ( "Hidden Options" ),
                            l_allOptions( "All Options" );

        l_publicOptions.add_options()
            ( "help"     , "show help screen" )
            ( "help-all" , "show all options")
            ( "version" , "output version number.");

        l_configOptions.add_options ()
            ( "locale,l" , po::value<string>()->default_value(WNTRDATA_DEFAULT_LOCALE) ,
                "Defines the locale used by the system for parsing.");
            ( "linguistics-dir,lingdir,l" , po::value<string>()->default_value(WNTRDATA_DEFAULT_LOCALE) ,
                "Defines the directory where the linguistics information is stored.");

        l_hiddenOptions.add_options ()
            ( "plugin,p" , po::value<string>() ,
                "Loads a plug-in. (default: none specified)" )
            ( "ipc,i"    , po::value<string>()->default_value("master") ,
                "Defines the IPC module to run this process as. (default: 'master')" );

        l_configOptions.add (l_hiddenOptions);

        l_allOptions.add(l_publicOptions).add(l_configOptions);

        string ipcModule("master");

        try {
            po::store ( po::parse_command_line ( l_argc, l_argv , l_allOptions ), l_vm );
            po::notify ( l_vm );
        } catch (exception &e){
            qDebug() << "Command-line parsing error: " << e.what();
        }

        if ( !l_vm.empty () ) {
            if ( l_vm.count ( "help" ) || l_vm.count ("help-all")) {
                /// @todo Render a set of text to be used for the help screen.
                cout << "\"There's no help for those who lack the valor of mighty men!\"" << endl;

                if (l_vm.count ("help"))
                     cout << l_publicOptions;
                else if (l_vm.count ("help-all"))
                     cout << l_allOptions;

                cout << endl << endl
                     << "If you want more help and/or information, visit <http://www.thesii.org> to" << endl
                     << "learn more about Wintermute or visit us on IRC (freenode) in ##sii-general." << endl;

                exit (0);
            }

            if ( l_vm.count ( "ipc" ) )
                s_args->insert ("ipc" , QString::fromStdString (l_vm["ipc"].as<string>()));

            if ( l_vm.count ("locale") )
                s_args->insert ("locale" , QString::fromStdString (l_vm["locale"].as<string>()));

            if ( l_vm.count ("linguistics-dir") )
                s_args->insert ("linguistics-dir" , QString::fromStdString (l_vm["linguistics-dir"].as<string>()));

            if ( l_vm.count ("plugin") )
                s_args->insert ("plugin" , QString::fromStdString (l_vm["plugin"].as<string>()));

            qDebug() << *s_args;

        } else
            cout << "(core) [Core] Run this application with '--help' to get help information." << endl;
    }

    const Core* Core::instance () { return s_core; }

    void Core::Initialize() {
        Data::Configuration::Initialize ();
        Data::Linguistics::Configuration::setLocale (s_args->value ("locale").toString ().toStdString ());
        Data::Linguistics::Configuration::setDirectory (s_args->value ("linguistics-dir").toString ().toStdString ());
        Plugins::Factory::Startup ();
        emit s_core->initialized ();
        Thread l_thrd;
        l_thrd.run();
#ifndef WINTERMUTE_USING_GUI
        Core::startCurses();
#endif
    }

    void Core::Deinitialize() {
        Data::Configuration::Deinitialize ();
        emit s_core->deinitialized ();
        Plugins::Factory::Shutdown ();
#ifndef WINTERMUTE_USING_GUI
        Core::stopCurses();
#endif
    }

    void Core::doDeinit () const {
        Core::Deinitialize ();
    }

#ifndef WINTERMUTE_USING_GUI
    void Core::startCurses() {
        Curses::start();
    }

    void Core::stopCurses(){
        Curses::stop();
    }

    void Thread::run() {
        Wintermute::Linguistics::Parser l_prsr;
        QTextStream l_strm(stdin);

        while (!l_strm.atEnd ()){
            cout << "(main) Statement: ] ";
            QString l_ln = l_strm.readLine ();
            l_prsr.parse (l_ln.toStdString ());
            l_strm << endl;
        }
    }

#endif
}
