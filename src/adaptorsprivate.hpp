/***
 *  This file is part of Wintermute.
 *
 *  Copyright (C) 2012 Jacky Alciné <jacky.alcine@thesii.org>
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
 *  You should have received a copy of the GNU Library General Public License
 *  along with Wintermute .  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <QTimer>
#include <global.hpp>

WINTER_BEGIN_NAMESPACE

class AbstractAdaptor;

/**
 * @class AbstractAdaptorPrivate
 * Handles the internal interactions of the AbstractAdaptor.
 */
struct AbstractAdaptorPrivate {
    Q_DECLARE_PUBLIC (AbstractAdaptor)
    AbstractAdaptorPrivate (AbstractAdaptor* p_qPtr);
    virtual ~AbstractAdaptorPrivate();

    /**
     * @fn detect
     * Detects if the core instance of Wintermute is alive.
     * @return void
     */
    void detect() const;

    AbstractAdaptor* q_ptr;
    QTimer* timer;
    mutable bool isCore;
};

WINTER_END_NAMESPACE

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
