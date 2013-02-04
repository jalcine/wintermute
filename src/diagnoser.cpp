/***
 *  This file is part of the Wintermute project.
 *
 *  Copyright (C) 2012 Jacky Alciné <me@jalcine.me>
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
 * @author Jacky Alciné <me@jalcine.me>
 * @date 04/22/12 5:40:03 AM
 */

#include <QString>
#include "diagnoser.hpp"

using namespace Wintermute::Diagnostics;

QMap<QString, Diagnoser*> Diagnoser::diagnosers = QMap<QString, Diagnoser*>();

Diagnoser::Diagnoser (QObject* parent) : QObject (parent)
{
}

Diagnoser::Diagnoser (QString& p_newLabel, bool p_autoRegister, QObject* p_parent)
    : QObject (p_parent), label (p_newLabel)
{
    if (p_autoRegister)
        Diagnoser::registerDiagnoser (this);
}

QString Diagnoser::getLabel()
{
    return label;
}

void Diagnoser::registerDiagnoser (Diagnoser* pDiag)
{
    Diagnoser::diagnosers.insert (pDiag->getLabel(), pDiag);
}

void Diagnoser::removeDiagnoser (QString& label)
{
    Diagnoser* pDiag = Diagnoser::diagnosers.value (label);
    Diagnoser::diagnosers.remove (label);
    // By specification no diagnoser can exist beyond this register.
    delete pDiag;
}

Diagnoser* Diagnoser::getDiagnoser (QString& label)
{
    return Diagnoser::diagnosers.value (label);
}

Diagnoser::~Diagnoser()
{
}

#include "diagnoser.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
