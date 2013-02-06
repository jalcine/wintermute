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
 *  License along with Wintermute.
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <me@jalcine.me>
 * @date 04/22/12 5:41:05 AM
 */

#ifndef WINTER_DIAGNOSER_HPP_
#define WINTER_DIAGNOSER_HPP_

#include <QObject>
#include <QString>
#include <QMap>

namespace Wintermute
{
namespace Diagnostics
{

/**
 * @brief Base diagnoser class
 * This class represents one of major Wintermute's components: diagnostics performer
 * & reporter. Its role is to provide statistics about system's vitality and
 * its used for reporting and self-assessment.
 */
class Diagnoser : public QObject
{
    Q_OBJECT

private:
    static QMap<QString, Diagnoser*> diagnosers; /// Other diagnosers, responsible mainly for plug-ins.

protected:
    QString label;

public:
    Diagnoser (QObject* = 0);

    /**
     * @brief Construct a diagnoser using label
     * @param newLabel Diagnoser's label.
     * @param autoRegister If true, automatically registers itself.
     * @param parent QObject parent.
     */
    Diagnoser (QString& p_newLabel, bool p_autoRegister = true, QObject* p_parent = 0);

    virtual ~Diagnoser();

    /**
     * @brief Get diagnoser's label
     * @returns Label of a diagnoser.
     */
    QString getLabel();

    /**
     * @brief Register new diagnoser
     * @param pDiag Pointer to the diagnoser.
     */
    static void registerDiagnoser (Diagnoser*);

    /**
     * @brief Remove diagnoser
     * @param label Label of a diagnoser you want to remove.
     */
    static void removeDiagnoser (QString&);

    /**
     * @brief Get pointer to a diagnoser with given label
     * @param label Label of the wanted diagnoser.
     * @returns Pointer to the diagnoser with the given label.
     */
    static Diagnoser* getDiagnoser (QString&);
};
} // namespaces
}
#endif // WINTER_DIAGNOSER_HPP_
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
