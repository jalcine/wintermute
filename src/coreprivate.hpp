/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
 *
 *  SpeechControl is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  SpeechControl is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with SpeechControl.  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <QVariantMap>
#include <QApplication>
#include <global.hpp>

WINTER_FORWARD_DECLARE_CLASS(Core)

WINTER_BEGIN_NAMESPACE

struct CorePrivate {
    Q_DECLARE_PUBLIC(Core)
    CorePrivate (Core* p_qPtr);
    ~CorePrivate();
    Core* q_ptr;            ///< The internal object that represents the core of Wintermute.
    QApplication* app;      ///< Holds the object representing the current Q(Core)Application.
    QVariantMap args;       ///< Holds the map containing the arguments passed to Wintermute in a normalized format.

    void parseCommandLineArguments();
    void setDefaultArguments();
    /**
     * @brief Configures Wintermute's core.
     *
     * This method holds the instructions to set-up some imperative command line arguments.
     * It then parses each argument and saves it to the global cache of arguments for other
     * utilities to use.
     *
     * @fn Configure
     * @param p_argc The command line argument passed representing the number of given arguments.
     * @param p_argv The command line argument passed representing the value of each argument.
     * @see Wintermute::Core::Initialize()
     */
    void configure (int& p_argc, char** p_argv);

};

WINTER_END_NAMESPACE

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
