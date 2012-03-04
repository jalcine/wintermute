/**
 * @author Wintermute Development <wntr-devel@thesii.org>
 * @file logging.hpp
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

#include "logging.hpp"
#include "config.hpp"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <QApplication>

using namespace std;
using std::string;

namespace Wintermute {
Logging* Logging::s_inst = new Logging;

Logging::Logging() : QObject() { }

Logging* Logging::instance() {
    return s_inst;
}

void Logging::catchQDebugMessage ( QtMsgType p_typ, const char *p_msg ) {
    const QTime l_tm = QTime::currentTime();
    string l_str = "[ T+" + l_tm.toString().toStdString() + ":" + QString::number ( l_tm.msec() ).toStdString()  +" ] <p:" + QString::number ( QApplication::applicationPid() ).toStdString() + "> ";

    switch ( p_typ ) {
    case QtDebugMsg:
        l_str += "DEBUG: ";
        break;

    case QtWarningMsg:
        l_str += "WARN:  ";
        emit instance()->warningEncountered ( p_msg );
        break;

    case QtCriticalMsg:
        l_str += "CRIT:  ";
        qApp->beep();
        emit instance()->criticalErrorEncountered ( p_msg );
        break;

    case QtFatalMsg:
        l_str += "FATAL: ";
        qApp->beep();
        emit instance()->fatalErrorEncountered ( p_msg );
        break;
    }

    l_str += p_msg;
    fprintf ( stderr, "%s\n", l_str.c_str() );
}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
