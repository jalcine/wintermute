/**
 * @file wntrling.hpp
 * This file is part of Wintermute Linguistics
 *
 * Copyright (C) 2011 - Wintermute Development <wntr-devel@thesii.org>
 *
 * Wintermute Linguistics is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute Linguistics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute Linguistics; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

// Qt includes
#include <QDebug>

// WntrData includes
#include <data/system.hpp>

// local includes
#include "parser/engine.hpp"
#include "system.hpp"

using Wintermute::Linguistics::System;

System* System::s_sys = 0;

System::System() : QObject(), m_prs ( (new Engine))
{
    if (System::s_sys)
        System::s_sys->deleteLater ();

    connect (this, SIGNAL (started()), Wintermute::Data::System::instance(), SLOT (start()));
    connect (this, SIGNAL (stopped()), Wintermute::Data::System::instance(), SLOT (stop()));

    System::s_sys = this;

}

System::System (const System& p_other) : QObject (p_other.parent())
{

}

System::~System ()
{
    m_prs->deleteLater ();
}

System* System::instance ()
{
    if (!System::s_sys)
        System::s_sys = new System;

    return s_sys;
}

void System::start ()
{
    emit s_sys->started ();
}

void System::stop ()
{
    emit s_sys->stopped ();
}

void System::tellSystem (const QString& p_txt)
{
    qDebug() << "(ling) [System] Parsing" << p_txt << "..";
    s_sys->m_prs->parse (p_txt);
    emit s_sys->responseFormed (p_txt);
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
