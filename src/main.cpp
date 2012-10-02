/***
 *  This file is part of the project project.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
 *
 *  project is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with project .
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 04/18/12 4:08:58 PM
 */

#include <QApplication>
#include "wintermute.hpp"

WINTER_USE_NAMESPACE

int main (int p_argc, char** p_argv)
{
    qInstallMsgHandler (Logging::catchQDebugMessage);
    Core::boot(p_argc, p_argv);
    return QApplication::exec();
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
