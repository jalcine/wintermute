/**
 * @file core.hpp
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
 *
 * @legalese
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
 * @endlegalese
 */

#ifndef CORE_HPP
#define CORE_HPP

#include "config.hpp"
#include <boost/any.hpp>
#include <QVariantMap>

namespace Wintermute {
    struct Core;

    /**
     * @brief The central management class of Wintermute.
     *
     * @c Wintermute::Core holds the vital activities of Wintermute's startup. It
     * handles the actions from the outside system environment and, with a little
     * Qt magic and hours of coding, transfers control to the parts of Wintermute
     * that would be appropriate to be under control.
     *
     * @nonreentrant
     * @class Core wintermute.hpp "include/wintermute/wintermute.hpp"
     */
    class Core : public QObject {
        Q_OBJECT

        public:
            /**
             * @brief Flags representing the user interface of Wintermute.
             * @enum UserInterface
             */
            enum UserInterface {
                Graphical = 0, /**< Wintermute was compiled with QtGui and, therefore, support a graphical user interface. */
                Textual /**< Wintermute was compiled without QtGui and, therefore, does not support a graphical user interface. It does, however, provide a textual user interface, using nCurses. */
            };

            /**
             * @brief Default constructor.
             *
             * @attention This constructor should NEVER be used, it's called by the main()
             * method of Wintermute, and needs never to be called again.
             *
             * @fn Core
             * @param argc The command line argument passed representing the number of given arguments.
             * @param argv The command line argument passed representing the value of each argument.
             * @badcode
             */
            explicit Core ( int&, char** );

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
             *
             * @fn arguments
             */
            const static QVariantMap* arguments();

            /**
             * @brief
             *
             * @fn endProgram
             */
            static void endProgram();

            /**
             * @brief Determines the kind of front-facing interface the Wintermute process is using.
             * @todo Consider using a bool instead of an enum, feels like a waste of memory to me, just adding onto the vtable.
             * @fn userInterface
             */
            inline const static UserInterface userInterface() {
        #ifndef WINTERMUTE_USING_GUI
                return Textual;
        #else
                return Graphical;
        #endif
            }

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
             * This method holds the instructions tos set-up some imperative command line arguments.
             * It then parses each argument and saves it to the global cache of arguments for other
             * utilities to use.
             *
             * @fn Configure
             * @param argc The command line argument passed representing the number of given arguments.
             * @param argv The command line argument passed representing the value of each argument.
             * @see Wintermute::Core::Initialize
             */
            static void Configure ( int& , char ** );

            /**
             * @brief Initializes the system.
             *
             * Does the first bit of initialization work for the core process of Wintermute by
             * loading the plug-ins and then the data system.
             *
             * @todo Consider testing whether or not Wintermute's running as a daemon and invoke a sub process.
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


        public slots:
            /**
             * @brief Starts the nCurses interface.
             *
             * @fn doCurses
             */
            static void startCurses();

            /**
             * @brief
             *
             * @fn stopCurses
             */
            static void stopCurses();

        private:
            static WNTR_APPLICATION* s_app; /**< Holds the object representing the current Q(Core)Application. */
            static QVariantMap* s_args; /**< Holds the map containing the arguments passed to Wintermute in a normalized format. */
            static Core* s_core; /**< The internal object that represents the core of Wintermute. */
            /**
             * @brief Processes the command line arguments.
             *
             * This method handles the nitty-gritty work of converting all of those command-line arguments
             * to someone less C-style-ish and more C++ workable.
             *
             * @fn configureCommandLine
             */
            static void configureCommandLine();

        private slots:
            void doDeinit() const;
            void unixSignal(int signal) const;
    };

    class Thread {
        friend class Core;

        public:
            void run();
    };
}

#endif /* CORE_HPP */
// kate: indent-mode cstyle; space-indent on; indent-width 4;
