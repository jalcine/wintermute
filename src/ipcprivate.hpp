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
 * @date 05/03/12 7:05:23 PM
 */

#include <global.hpp>

class QDBusConnection;

WINTER_FORWARD_DECLARE_CLASS(IPC)
WINTER_FORWARD_DECLARE_CLASS(AbstractAdaptor)

WINTER_BEGIN_NAMESPACE

struct IPCPrivate {
    Q_DECLARE_PUBLIC (IPC)
    IPCPrivate (IPC* p_qPtr);
    ~IPCPrivate();

    /**
     * @brief Obtains the currently running bus.
     * @fn connection
     */
    QDBusConnection* bus();

    IPC* q_ptr;
    QDBusConnection* connection;
    AbstractAdaptor* adaptor;

};

WINTER_END_NAMESPACE
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
