/**
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
 * @legalese
 * This library is free software; you can redistribute it and/or
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
#ifndef NCURSES_HPP
#define NCURSES_HPP

#include "config.hpp"
#include <QObject>

namespace Wintermute {
    struct Curses;

    /**
     * @brief Provides the front-facing terminal user interface for Wintermute.
     *
     * In an effort to provide another less resource-intenstive interface, aside
     * from the QtGui module, Wintermute::Curses is here to implement a terminal
     * interface for Wintermute.
     *
     * @warning This class is not implemented at all. There's a chance that it may
     *          even be the cause of a build-time error. This is because (perhaps)
     *          the <QTimer> class has an issue with @variable stdscr for some absurb
     *          reason.
     *
     * @bug <QTimer> or some object with Qt's MOC system is inhibiting the use of nCurses. (Apply bug number here once reported).
     * @todo Move from the C ncurses library to the C++ cwidget library.
     *
     * @class Curses ncurses.cpp "src/ncurses.cpp"
     */
    class Curses : public QObject {
        Q_OBJECT
        private:
            static Curses s_crs;
            /**
             * @brief
             *
             * @fn Curses
             */
            Curses();
            /**
             * @brief
             *
             * @fn Curses
             * @param
             */
            Curses(const Curses&);

        public:
            /**
             * @brief
             *
             * @fn ~Curses
             */
            virtual ~Curses();
            /**
             * @brief
             *
             * @fn start
             */
            static void start();

            /**
             * @brief
             *
             * @fn stop
             */
            static void stop();
    };
}
#endif
