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
#include <ncurses.h>

namespace Wintermute {
    struct Curses;

    /**
     * @brief Provides the front-facing terminal user interface for Wintermute.
     *
     * In an effort to provide another less resource-intenstive interface, aside
     * from the QtGui module, Wintermute::Curses is here to implement a terminal
     * interface for Wintermute. The nCurses interface can be used with or without
     * QtGui installed. To use it, run 'wintermute --gui false' to have the actions
     * of Wintermute redirected to the nCurses interface.
     *
     * @class Curses ncurses.cpp "src/ncurses.cpp"
     */
    class Curses {
        private:
            Curses();
            Curses(const Curses&);
            static void _init();
            static void _print(const int, const int, const QString& );
            static void _refresh();

        public:
            virtual ~Curses();
            static void printText(const QString& , const int = -1, const int = - 1);
            static void printStatus(const QString&);
            static void start();
            static void stop();
    };
}
#endif
// kate: indent-mode cstyle; space-indent on; indent-width 0;
