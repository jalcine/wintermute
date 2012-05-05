/*
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
/**
 * @author Wintermute Development <wntr-devel@thesii.org>
 * @file logging.hpp
 */

#include "logging.hpp"
#include "global.hpp"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <QApplication>

using namespace std;
using std::string;

namespace Wintermute
{
Logging* Logging::s_inst = new Logging;

Logging::Logging() : QObject() { }

Logging* Logging::instance()
{
    return s_inst;
}

void Logging::catchQDebugMessage (QtMsgType p_messageType, const char* p_messageText)
{
    string str;
#if WINTER_DEBUG_TIMESTAMPS
    const QTime tm = QTime::currentTime();
    str = "[ T+" + tm.toString (Qt::ISODate).toStdString() + ":" + QString::number (tm.msec()).toStdString()  + " ] <p:" + QString::number (QApplication::applicationPid()).toStdString() + "> ";
#endif

    switch (p_messageType) {
    case QtDebugMsg:
        str += "D: ";
        break;

    case QtWarningMsg:
        str += "W:  ";
        emit instance()->warningEncountered (p_messageText);
        break;

    case QtCriticalMsg:
        str += "C:  ";
        qApp->beep();
        emit instance()->criticalErrorEncountered (p_messageText);
        break;

    case QtFatalMsg:
        str += "F: ";
        qApp->beep();
        emit instance()->fatalErrorEncountered (p_messageText);
        break;
    }

    str += p_messageText;

    fprintf (stderr, "%s\n", str.c_str());
    emit instance()->linePrinted(p_messageText);
}
}

#include "logging.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
