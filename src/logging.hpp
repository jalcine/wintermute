/***
 *  This file is part of the Wintermute project.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
 *
 *  Wintermute is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  Wintermute is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with Wintermute .
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 04/22/12 5:42:48 AM
 */

#ifndef WINTERMUTE_LOGGING_HPP
#define WINTERMUTE_LOGGING_HPP

#include <QObject>
#include <QDebug>
#include <QTime>

namespace Wintermute
{
class Logging;

/// @todo Allow triggering the saving of such logs to disc.
/// @todo Allow dynamic printing of certain flags (overriding build commands).
/// @todo Add a event filter for this into the application.
class Logging : public QObject
{
    Q_OBJECT
    Q_CLASSINFO ("objectName", "Logging Singleton")
    Q_DISABLE_COPY (Logging)

signals:
    void linePrinted (const QString& p_line);
    void warningEncountered (const QString& p_warning);
    void criticalErrorEncountered (const QString& p_error);
    void fatalErrorEncountered (const QString& p_fatalError);

public:
    static void catchQDebugMessage (QtMsgType p_messageType, const char* p_messageText);
    static Logging* instance();

private:
    Logging();
    static Logging* s_inst;
};
} // namespace

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
