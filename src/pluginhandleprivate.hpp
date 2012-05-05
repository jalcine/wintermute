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
 *  License along with Wintermute.
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/03/12 8:28:34 PM
 */

#include <global.hpp>

WINTER_FORWARD_DECLARE_CLASS(PluginHandle)

WINTER_BEGIN_NAMESPACE

struct PluginHandlePrivate {
    Q_DECLARE_PUBLIC(PluginHandle)
    PluginHandlePrivate(PluginHandle* p_qPtr);
    ~PluginHandlePrivate();
    PluginHandle* q_ptr;
    QString uuid;
    QProcess* process;
    QSettings* settings;

};

WINTER_END_NAMESPACE