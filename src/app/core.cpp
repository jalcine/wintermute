/**
 * @author Wintermute Development <wntr-devel@thesii.org>
 *
 * @section lcns Licensing
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
 *
 */

// Local
#include "core.hpp"
#include "config.hpp"
#include "ipc.hpp"
#include "plugins.hpp"

// Standard
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>

// Qt
#include <QtDebug>
#include <QtDebug>
#include <QProcess>
#include <QSettings>
#include <QDateTime>
#include <QVariantMap>
#include <QLibraryInfo>
#include <QApplication>
#include <QSocketNotifier>

// Boost
#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

using boost::program_options::variables_map;
using boost::program_options::variable_value;
using boost::program_options::options_description;

using namespace Wintermute;

QApplication* Core::s_app = 0;
QVariantMap* Core::s_args = 0;
Core* Core::s_core = 0;

Core::Core ( int &p_argc, char **p_argv ) {
    Core::s_core = this;
    Core::Configure ( p_argc, p_argv );
    Core::start();
}

void Core::Configure ( int& p_argc, char** p_argv ) {
    QString ipcMod = "master";
    s_app = new QApplication ( p_argc, p_argv );
    s_app->setApplicationName ( "Wintermute" );
    s_app->setApplicationVersion ( QString::number ( WNTR_VERSION ) );
    s_app->setOrganizationDomain ( "thesii.org" );
    s_app->setOrganizationName ( "Synthetic Intellect Institute" );
    connect ( s_app, SIGNAL ( aboutToQuit() ), s_core, SLOT ( stop() ) );
    s_core->setParent ( s_app );

    configureCommandLine();

    if ( s_args->count ( "ipc" ) != 0 )
        ipcMod = s_args->value ( "ipc" ).toString();
}

const QVariantMap* Core::arguments() {
    return s_args;
}

/// @todo Allow arbitrary arguments to be added into the system
void Core::configureCommandLine() {
    variables_map vm;
    s_args = new QVariantMap;
    int argc = s_app->argc ();
    char** argv = s_app->argv ();
    options_description publicOptions ( "General Options" ),
                        configOptions ( "Configuration" ),
                        hiddenOptions ( "Hidden Options" ),
                        allOptions ( "All Options" );

    publicOptions.add_options()
    ( "help,h"       , po::value<string>()->default_value ( "ignore" ) , "Show meaning of command line arguments. (valid values are 'standard', 'config', and 'all')" )
    ( "copyright,c"  , "Prints copyright information and exits." )
    ( "version,v"    , "Prints version number and exits." )
    ;

    configOptions.add_options ()
    ( "locale,l" , po::value<string>()->default_value ( WNTR_LOCALE ) ,
      "Defines the locale used by the system for parsing." )
    ( "data-dir,datadir,l" , po::value<string>()->default_value ( WNTR_DATA_DIR ) ,
      "Defines the directory where Wintermute's data is stored." )
    ;

    hiddenOptions.add_options ()
    ( "plugin,p"  , po::value<string>()->default_value ( "root" ) ,
      "Loads a plug-in; used for module 'Plugin'. (default: 'root' [the manager])" )
    ( "ipc,i"     , po::value<string>()->default_value ( "master" ) ,
      "Defines the IPC module to run this process as." )
    ( "daemon"    , po::value<string>()->default_value ( "false" ),
      "Determines whether or not this process runs as a daemon." )
    ;

    allOptions.add ( publicOptions ).add ( configOptions );
    allOptions.add ( hiddenOptions );

    try {
        po::store ( po::parse_command_line ( argc, argv , allOptions ), vm );
        po::notify ( vm );
    } catch ( exception &e ) {
        qDebug() << "Command-line parsing error: " << e.what();
    }

    if ( !vm.empty () ) {
        for ( variables_map::const_iterator itr = vm.begin (); itr != vm.end (); itr++ ) {
            const QString key = QString::fromStdString ( itr->first );
            const variable_value val = itr->second;
            s_args->insert ( key,QString::fromStdString ( val.as<string>() ) );
        }

        if ( vm.count ( "help" ) && vm.at ( "help" ).as<string>() != "ignore" ) {
            cout << "\"There's no help for those who lack the valor of mighty men!\"" << endl;

            if ( vm.at ( "help" ).as<string>() == "all" )
                cout << allOptions;
            else if ( vm.at ( "help" ).as<string>() == "config" )
                cout << configOptions;
            else if ( vm.at ( "help" ).as<string>() == "standard" )
                cout << publicOptions;

            cout << endl << endl
                 << "If you want more help and/or information, visit <http://www.thesii.org> to" << endl
                 << "learn more about Wintermute or visit us on IRC (freenode) in #sii for general info." << endl;
            exit ( 0 );
        } else if ( vm.count ( "version" ) ) {
            cout << endl << "Wintermute " << QApplication::applicationVersion ().toStdString () << " "
                 << "using Qt v" << QT_VERSION_STR << ", build " << QLibraryInfo::buildKey ().toStdString ()
                 << ", on " << QLibraryInfo::buildDate ().toString ().toStdString () << "." << endl
                 << "Boost v" << BOOST_VERSION << endl << endl;
            exit ( 0 );
        } else if ( vm.count ( "copyright" ) ) {
            cout << "Copyright (C) 2010 - 2011 Synthetic Intellect Institute <contact@thesii.org>" << endl
                 << "Copyright (C) 2010 - 2011 Wintermute Developers <dev@thesii.org> " << endl
                 << "Copyright (C) 2010 - 2011 Wintermute Robo-Psychologists <psych@thesii.org> " << endl << endl
                 << "\tThis program is free software; you can redistribute it and/or modify "  << endl
                 << "\tit under the terms of the GNU General Public License as published by " << endl
                 << "\tthe Free Software Foundation; either version 3 of the License, or" << endl
                 << "\t(at your option) any later version." << endl << endl;
            exit ( 0 );
        }
    } else
        cout << "(core) [Core] Run this application with '--help' to get more information." << endl;
}

Core* Core::instance() {
    return s_core;
}

/// @todo Consider testing whether or not Wintermute's running as a daemon and invoke a sub process.
void Core::start() {
    if ( Core::arguments()->value ( "ipc" ).toString() == "master" ) {
        cout << qPrintable ( s_app->applicationName () ) << " "
             << qPrintable ( s_app->applicationVersion () )
             << " (pid " << s_app->applicationPid () << ") :: "
             << "Artificial intelligence for Common Man. (Licensed under the GPL3+)" << endl;
    }

    IPC::System::start();

    if ( IPC::System::module() == "master" ) {
        QSettings* settings = new QSettings ( "Synthetic Intellect Institute","Wintermute" );
        QDateTime lstDate = settings->value ( "Statistics/StartupDate",QDateTime::currentDateTime() ).toDateTime();
        settings->setValue ( "Statistics/StartupDate",QDateTime::currentDateTime() );
        qDebug() << "(core) Last startup was at" << lstDate.toLocalTime().toString();
    }

    emit s_core->started();
    qDebug() << "(core) [Core] Started.";
}

void Core::exit ( int p_exitCode, bool p_killSystem ) {
    qDebug() << "(core) [" << IPC::System::module () << "] Exitting...";

    if ( ( IPC::System::module () != "master" && arguments ()->value ( "help" ) == "ignore" ) && p_killSystem ) {
        qDebug() << "(core) [" << IPC::System::module () << "] Closing root appplication...";
        QDBusMessage msg = QDBusMessage::createMethodCall ( "org.thesii.Wintermute","/Master", "org.thesii.Wintermute.Master","quit" );
        QDBusMessage reply = IPC::System::bus ()->call ( msg,QDBus::Block );
        if ( reply.type () == QDBusMessage::ErrorMessage )
            qDebug() << "(core) [" << IPC::System::module () << "] Can't terminate master module of Wintermute:" << reply.errorName();
    }

    qDebug() << "(core) [" << IPC::System::module () << "] Exitted.";
    QApplication::exit ( p_exitCode );
}

void Core::quit() {
    Core::exit ( 0 );
}

void Core::stop () {
    qDebug() << "(core) [" << IPC::System::module() << "] Stopping...";
    IPC::System::stop ();
    emit s_core->stopped ();
    qDebug() << "(core) [" << IPC::System::module() << "] Process stopped.";
}

#include "core.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
