/**
 * @file core.hpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 *
 * @section lcns Licensing
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#ifndef CORE_HPP
#define CORE_HPP

// Local
#include "config.hpp"
#include "diagnoser.hpp"

// Qt
#include <QObject>
#include <QVariantMap>

class QApplication;
class QSocketNotifier;

namespace Wintermute {

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
class Core : public QObject {
    Q_OBJECT
    Q_CLASSINFO ( "objectName","Wintermute's Core" )

public:

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
    explicit Core ( int& p_argv, char** p_argc );

    /**
     * @brief Obtains an instance of the Core.
     *
     * Provided that a developer wishes to connect to the signals exposed by the core,
     * this returns a pointer to the Core's only instance.
     *
     * @return A constant pointer to the Core.
     * @fn instance
     */
    static Core* instance();

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
    const static QVariantMap* arguments();

    /**
     * @brief Ends the program; with an optional exit code.
     * @fn exit
     */
    static void exit ( const int = 0, const bool = false );

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

protected:
    /**
     * @brief Configures Wintermute's core.
     *
     * This method holds the instructions to set-up some imperative command line arguments.
     * It then parses each argument and saves it to the global cache of arguments for other
     * utilities to use.
     *
     * @fn Configure
     * @param argc The command line argument passed representing the number of given arguments.
     * @param argv The command line argument passed representing the value of each argument.
     * @see Wintermute::Core::Initialize()
     */
    static void Configure ( int& , char ** );

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
    static QApplication* s_app; /**< Holds the object representing the current Q(Core)Application. */
    static QVariantMap* s_args; /**< Holds the map containing the arguments passed to Wintermute in a normalized format. */
    static Core* s_core; /**< The internal object that represents the core of Wintermute. */

    /**
     * @brief Processes the command line arguments.
     *
     * This method handles the nitty-gritty work of converting all of those command-line arguments
     * to something less C-style-ish and more C++ workable.
     *
     * @fn configureCommandLine
     */
    static void configureCommandLine();
};
} // namespace

#endif /* CORE_HPP */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
