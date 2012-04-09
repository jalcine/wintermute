/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
 *
 *  SpeechControl is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  SpeechControl is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with SpeechControl.  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>

#include <QVariantMap>
#include <QLibraryInfo>
#include <QApplication>

#include <boost/program_options.hpp>

#include "app/global.hpp"
#include "app/core.hpp"
#include "app/core.hxx"

using std::cout;
using std::string;
using std::exception;
using namespace Wintermute;

namespace po = boost::program_options;

using boost::program_options::variables_map;
using boost::program_options::variable_value;
using boost::program_options::options_description;

void CorePrivate::configureCommandLine()
{
    variables_map vm;
    args = QVariantMap();
    int argc = app->argc ();
    char** argv = app->argv ();
    options_description publicOptions ("General Options"),
                        configOptions ("Configuration"),
                        hiddenOptions ("Hidden Options"),
                        allOptions ("All Options");

    publicOptions.add_options()
    ("help,h"       , po::value<string>()->default_value ("ignore") , "Show meaning of command line arguments. (valid values are 'standard', 'config', and 'all')")
    ("copyright,c"  , "Prints copyright information and exits.")
    ("version,v"    , "Prints version number and exits.")
    ;

    configOptions.add_options ()
    ("locale,l" , po::value<string>()->default_value (WNTR_LOCALE) ,
     "Defines the locale used by the system for parsing.")
    ("data-dir,datadir,l" , po::value<string>()->default_value (WNTR_DATA_DIR) ,
     "Defines the directory where Wintermute's data is stored.")
    ;

    hiddenOptions.add_options ()
    ("plugin,p"  , po::value<string>()->default_value ("root") ,
     "Loads a plug-in; used for module 'Plugin'. (default: 'root' [the manager])")
    ("ipc,i"     , po::value<string>()->default_value ("master") ,
     "Defines the IPC module to run this process as.")
    ("daemon"    , po::value<string>()->default_value ("false"),
     "Determines whether or not this process runs as a daemon.")
    ;

    allOptions.add (publicOptions).add (configOptions);
    allOptions.add (hiddenOptions);

    try {
        po::store (po::parse_command_line (argc, argv , allOptions), vm);
        po::notify (vm);
    }
    catch (exception& e) {
        qDebug() << "Command-line parsing error: " << e.what();
    }

    if (!vm.empty ()) {
        for (variables_map::const_iterator itr = vm.begin (); itr != vm.end (); itr++) {
            const QString key = QString::fromStdString (itr->first);
            const variable_value val = itr->second;
            args.insert (key, QString::fromStdString (val.as<string>()));
        }

        if (vm.count ("help") && vm.at ("help").as<string>() != "ignore") {
            cout << "\"There's no help for those who lack the valor of mighty men!\"" << endl;

            if (vm.at ("help").as<string>() == "all")
                cout << allOptions;
            else if (vm.at ("help").as<string>() == "config")
                cout << configOptions;
            else if (vm.at ("help").as<string>() == "standard")
                cout << publicOptions;

            cout << endl << endl
                 << "If you want more help and/or information, visit <http://www.thesii.org> to" << endl
                 << "learn more about Wintermute or visit us on IRC (freenode) in #sii for general info." << endl;
            exit (0);
        }
        else if (vm.count ("version")) {
            cout << endl << "Wintermute " << QApplication::applicationVersion ().toStdString () << " "
                 << "using Qt v" << QT_VERSION_STR << ", build " << QLibraryInfo::buildKey ().toStdString ()
                 << ", on " << QLibraryInfo::buildDate ().toString ().toStdString () << "." << endl
                 << "Boost v" << BOOST_VERSION << endl << endl;
            exit (0);
        }
        else if (vm.count ("copyright")) {
            cout << "Copyright (C) 2010 - 2011 Synthetic Intellect Institute <contact@thesii.org>" << endl
                 << "Copyright (C) 2010 - 2011 Wintermute Developers <dev@thesii.org> " << endl
                 << "Copyright (C) 2010 - 2011 Wintermute Robo-Psychologists <psych@thesii.org> " << endl << endl
                 << "\tThis program is free software; you can redistribute it and/or modify "  << endl
                 << "\tit under the terms of the GNU General Public License as published by " << endl
                 << "\tthe Free Software Foundation; either version 3 of the License, or" << endl
                 << "\t(at your option) any later version." << endl << endl;
            exit (0);
        }
    }
    else
        cout << "(core) [Core] Run this application with '--help' to get more information." << endl;
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
