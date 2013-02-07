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
 * @date 05/14/12 4:26:11 PM
 */
#ifndef WINTERMUTE_CORE_HPP
#define WINTERMUTE_CORE_HPP

#include <QObject>
#include <QVariantMap>

#include <global.hpp>
#include <diagnoser.hpp>

class QApplication;
class QSocketNotifier;

WINTER_FORWARD_DECLARE_STRUCT (CorePrivate)

WINTER_BEGIN_NAMESPACE

/**
 * @brief The central management class of Wintermute.
 *
 * The Core holds the vital activities of Wintermute's startup. It
 * handles the actions from the outside system environment and, with a little
 * Qt magic and hours of coding, transfers control to the parts of Wintermute
 * that would be appropriate to be under control.
 *
 * @nonreentrant
 * @class Core wintermute.hpp "core.hpp"
 */
/// @todo Implement a means of having this class catch the signals of the system.
class Core : public QObject
{
    Q_OBJECT
    WINTER_DECLARE_PRIVATE_STRUCT (Core)
    WINTER_SINGLETON (Core)

public:

    /**
     * @brief Obtains a pointer to the current arguments.
     *
     * This map contains a normalized list of the arguments that were passed on the
     * command line. This is used mainly to save the ease of plug-ins having to check
     * the string list given by Qt.
     *
     * @note This list only contains the qualified arguments in a list. Support for
     *       arbitrary arguments is coming soon.
     * @todo Add support for arbitrary arguments.
     * @fn arguments
     */
    static QVariantMap arguments();

    /**
     * @brief Default constructor.
     *
     * @attention This constructor should NEVER be used, it's called by the main()
     * method of Wintermute, and needs never to be called again.
     *
     * @fn Core
     * @param argc The command line argument passed representing the number of given arguments.
     * @param argv The command line argument passed representing the value of each argument.
     * @internal
     */
    static void boot (int& p_argv, char** p_argc);

    /**
     * @brief Prepares the core of this instance of Wintermute for external communications.
     *
     */
    static void prepareAdaptor();

    /**
     * @brief Ends the program; with an optional exit code.
     * @param p_exitCode The exit code to report to the system.
     * @param p_closeRootApplication Whether or not ALL Wintermute instances should be closed.
     * @fn exit
     */
    static void exit (const int p_exitCode = 0,
                      const bool p_closeRootApplication = false);

    /**
     * @brief Ends the program with success code.
     */
    static void quit();

signals:
    /**
     * @brief Raised once the core's ready to go.
     *
     * This signal is emitted when the core's done loading prerequisites.
     * This is <b>after</b> all plug-ins have been loaded and the inter-
     * process communication system is active.
     *
     * @fn initialized
     * @see Wintermute::Core::Initialize()
     */
    void started() const;

    /**
     * @brief Raised once the core's ready to shut down.
     *
     * This signal is emitted when the core's ready to shutdown the system.
     * This is <b>before</b> any plug-ins are unloaded, but not before they're
     * deinitialized.
     *
     * @fn deinitialized
     */
    void stopped() const;

protected slots:

    /**
     * @brief Initializes the system.
     *
     * Does the first bit of initialization work for the core process of Wintermute by
     * loading the plug-ins and then the data system.
     *
     * @see Wintermute::Core::initialized
     * @fn Initialize
     */
    static void start();

    /**
     * @brief Deinitializes the system.
     *
     * Cleans up all of the work for the core processes and runs the approriate disconnection methods.
     *
     * @see Wintermute::Core::deinitialized
     * @fn Deinitialize
     */
    static void stop ();

private:
    QScopedPointer<CorePrivate> d_ptr;
};

WINTER_END_NAMESPACE

#endif /* WINTERMUTE_CORE_HPP */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
