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

#include "config.hpp"
#include "core.hpp"
#include "ipc.hpp"
#include "plugins.hpp"
#include "ncurses.hpp"
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
using boost::program_options::variable_value;
using boost::program_options::options_description;
using Wintermute::Linguistics::Parser;
using std::cout;
using std::endl;

namespace Wintermute {
    WNTR_APPLICATION* Core::s_app = NULL;
    QVariantMap* Core::s_args = NULL;
    Core* Core::s_core = NULL;

    Core::Core ( int &p_argc, char **p_argv ) {
        Core::s_core = this;
        Core::Configure ( p_argc,p_argv );
    }

    void Core::Configure ( int& p_argc, char** p_argv ) {
        string ipcModule = "master";
        s_app = new WNTR_APPLICATION ( p_argc,p_argv );
        s_app->setApplicationName ( "Wintermute" );
        s_app->setApplicationVersion ( QString::number ( WINTERMUTE_VERSION ) );
        s_app->setOrganizationDomain ( "org.thesii.Wintermute" );
        s_app->setOrganizationName ( "Synthetic Intellect Institute" );
        connect ( s_app , SIGNAL ( aboutToQuit() ) , s_core , SLOT ( doDeinit() ) );
        s_core->setParent (s_app);

        cout << qPrintable ( s_app->applicationName () ) << " "
             << qPrintable ( s_app->applicationVersion () )
             << " (pid " << s_app->applicationPid () << ") :: "
             << "Artificial intelligence for common Man. (Licensed under the GPL3+)" << endl;

#ifdef WINTERMUTE_USING_GUI
        cout << "(core) Built with graphical user interface capabilities." << endl;
#endif

        configureCommandLine();

        if ( s_args->count ( "ipc" ) != 0 )
            ipcModule = s_args->value ( "ipc" ).toString ().toStdString ();

        IPC::Initialize ( ipcModule );
    }

    const QVariantMap* Core::arguments () {
        return s_args;
    }

    void Core::configureCommandLine () {
        variables_map l_vm;
        s_args = new QVariantMap;
        int l_argc = s_app->argc ();
        char** l_argv = s_app->argv ();
        options_description l_publicOptions ( "General Options" ),
        l_configOptions ( "Configuration" ),
        l_hiddenOptions ( "Hidden Options" ),
        l_allOptions ( "All Options" );

        l_publicOptions.add_options()
        ( "help,h"       , "show help screen" )
        ( "verbose-help" , "show all options" )
        ( "copyright,C"  , "show copyright information" )
        ( "version,V"    , "output version number." )
        ;

        l_configOptions.add_options ()
        ( "locale,l" , po::value<string>()->default_value ( WNTRDATA_DEFAULT_LOCALE ) ,
          "Defines the locale used by the system for parsing." )
        ( "data-dir,datadir,l" , po::value<string>()->default_value ( WNTRDATA_DATA_DIR ) ,
          "Defines the directory where Wintermute's data is stored." )
        ;

        l_hiddenOptions.add_options ()
        ( "plugin,p"  , po::value<string>() ,
          "Loads a plug-in. (default: none specified)" )
        ( "ipc,i"     , po::value<string>()->default_value ( "master" ) ,
          "Defines the IPC module to run this process as." )
        ( "ncurses,n" ,
 #ifdef WINTERMUTE_USING_GUI
           po::value<bool>()->default_value ( false ),
 #else
           po::value<bool>()->default_value ( true ),
 #endif
         "Toggles whether or not the nCurses interface is being used. This automatically disables the GUI.")
        ( "gui,g"     ,
#ifdef WINTERMUTE_USING_GUI
          po::value<bool>()->default_value ( true ),
#else
          po::value<bool>()->default_value ( false ),
#endif
          "Toggles whether or not the graphical user interface is loaded. This automatically disables nCurses." )
        ;

        l_allOptions.add ( l_publicOptions ).add ( l_configOptions );
        l_allOptions.add ( l_hiddenOptions );

        try {
            po::store ( po::parse_command_line ( l_argc, l_argv , l_allOptions ), l_vm );
            po::notify ( l_vm );
        } catch ( exception &e ) {
            qDebug() << "Command-line parsing error: " << e.what();
        }

        if ( !l_vm.empty () ) {
            if ( l_vm.count ( "help" ) || l_vm.count ( "verbose-help" ) ) {
                /// @todo Render a set of text to be used for the help screen.
                cout << "\"There's no help for those who lack the valor of mighty men!\"" << endl;

                if ( l_vm.count ( "help" ) )
                    cout << l_publicOptions;
                else if ( l_vm.count ( "verbose-help" ) )
                    cout << l_allOptions;

                cout << endl << endl
                     << "If you want more help and/or information, visit <http://www.thesii.org> to" << endl
                     << "learn more about Wintermute or visit us on IRC (freenode) in ##sii-general." << endl;

                exit ( 0 );
            } else if ( l_vm.count ( "version" ) ) {
                cout << endl << "Wintermute " << WNTR_APPLICATION::applicationVersion ().toStdString () << " "
                     << "using Qt v" << QT_VERSION_STR << ", build " << QLibraryInfo::buildKey ().toStdString ()
                     << ", on " << QLibraryInfo::buildDate ().toString ().toStdString () << "." << endl
                     << "Boost v" << BOOST_VERSION << endl << endl;

                exit ( 0 );
            } else if ( l_vm.count ( "copyright" ) ) {
                cout << "Copyright (C) 2010 Synthetic Intellect Institute <contact@thesii.org> <sii@lists.launchpad.net>" << endl
                     << "Copyright (C) 2010 Wintermute Developers <wintermute-devel@lists.launchpad.net> " << endl
                     << "Copyright (C) 2010 Wintermute Robo-Psychologists <wintermute-psych@lists.launchpad.net> " << endl << endl
                     << "\tThis program is free software; you can redistribute it and/or modify "  << endl
                     << "\tit under the terms of the GNU General Public License as published by " << endl
                     << "\tthe Free Software Foundation; either version 3 of the License, or" << endl
                     << "\t(at your option) any later version." << endl << endl;
                exit ( 0 );
            }

            for (variables_map::const_iterator l_itr = l_vm.begin (); l_itr != l_vm.end (); l_itr++){
                const QString l_key = QString::fromStdString (l_itr->first);
                const variable_value l_val = l_itr->second;
                const QString l_typeName = l_val.value ().type ().name ();

                if (l_typeName == "Ss")
                    s_args->insert (l_key,QString::fromStdString(l_val.as<string>()));
                else if (l_typeName == "b")
                    s_args->insert (l_key,l_val.as<bool>());
            }

            if (s_args->value ("gui").toBool() && !s_args->value ("ncurses").toBool ())
                s_args->insert ("ncurses",false);
            else if (s_args->value ("ncurses").toBool())
                s_args->insert ("gui",false);

        } else
            cout << "(core) [Core] Run this application with '--help' to get more information." << endl;
    }

    const Core* Core::instance () {
        return s_core;
    }

    void Core::unixSignal (int p_sig) const {
        qDebug() << "(core) Caught signal " << p_sig;
        switch (p_sig){

        }
    }

/// @todo This method doesn't update the default locale and directory from the command-line.
    void Core::Initialize() {
        Data::Configuration::Initialize();
        Data::Linguistics::Configuration::setLocale ( s_args->value ( "locale" ).toString ().toStdString () );
        Plugins::Factory::Startup ();
        emit s_core->initialized ();

        if ( Core::arguments ()->value ( "ncurses" ).toBool () )
            Core::startCurses();
        else {
            (new Thread)->run ();
        }
    }

    void Core::Deinitialize() {
        Core::stopCurses();
        Data::Configuration::Deinitialize ();
        emit s_core->deinitialized ();
        Plugins::Factory::Shutdown ();
    }

    void Core::doDeinit () const {
        qDebug() << "(core) Cleaning up..";
        Core::Deinitialize ();
        qDebug() << "(core) All clean!";
    }

    void Core::startCurses() {
        if (s_args->value ("ncurses").toBool ())
            Curses::start();
        else
            qDebug() << "(core) nCurses is disabled, not starting.";
    }

    void Core::stopCurses() {
        if (s_args->value ("ncurses").toBool ())
            Curses::stop();
        else
            qDebug() << "(core) nCurses is disabled, not stopping.";
    }

    void Thread::run() {
        if ( !Core::arguments ()->value ( "gui" ).toBool () ) {
            Wintermute::Linguistics::Parser l_prsr;
            QTextStream l_strm ( stdin );

            while ( !l_strm.atEnd () ) {
                cout << "(main) Statement: ] ";
                QString l_ln = l_strm.readLine ();
                if ( l_ln == "--" ) {
                    cout << "(main) Statement parsing stopped." << endl;
                    break;
                } else {
                    l_prsr.parse ( l_ln.toStdString () );
                    l_strm << endl;
                }
            }
        }

        WNTR_APPLICATION::quit();
    }

}
// kate: indent-mode cstyle; space-indent on; indent-width 4;
