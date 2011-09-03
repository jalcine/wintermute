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

#include "ncurses.hpp"
#include <QString>
#include <QStringList>

namespace Wintermute {
    static WINDOW* s_mnWndw = NULL;

    Curses::Curses() { }

    Curses::Curses ( const Curses &p_crs ) { }

    Curses::~Curses () { }

    void Curses::start() {
        _init();
        const QString l_status = QString("ALERT: Wintermute's nCurses is currently non-functional.\nWe recommend that you use") +
                QString(" the graphical frontend when compiled in GUI mode. Run 'wintermute --gui true' to") +
                QString(" enable GUI mode.");

        printText(QString("\nWintermute v.") + QString::number (WINTERMUTE_VERSION) +
                  QString("\n** Visit us at <http://www.thesii.org> or on Freenode (IRC) in ##sii-general. **\n"));
        printStatus(l_status);
        _refresh();
        getch();
        stop();
    }

    void Curses::printText(const QString& p_txt, const int p_x, const int p_y){
        int l_row, l_col;
        getmaxyx (stdscr,l_row,l_col);
        if (p_x == -1) l_row /= 2;
        else l_row = p_x;

        const QStringList l_txtLst = p_txt.split ("\n");

        for (int i = 0; i < l_txtLst.length (); i++){
            const QString l_txt = l_txtLst.at (l_txtLst.length () - i - 1);
            int l_myCol = l_col;
            if (p_y == -1) l_myCol = (l_myCol - l_txt.length()) / 2;
            else l_myCol = p_y;

            _print(l_row - i - 1,l_myCol,l_txt);
        }
    }

    void Curses::printStatus (const QString &p_status){
        const QStringList l_txtLst = p_status.split("\n");
        int l_row, l_col;
        getmaxyx (stdscr,l_row,l_col);

        for (int i = 0; i < l_txtLst.length (); i++){
            const QString l_txt = l_txtLst.at (l_txtLst.length () - i - 1);
            _print(l_row - i - 1, 0, l_txt);
        }
    }

    void Curses::_init () {
        initscr ();
        raw(); keypad(stdscr,true); noecho();
    }

    void Curses::_print(const int p_row, const int p_col, const QString& p_txt){
        mvprintw (p_row,p_col,p_txt.toStdString().c_str());
        _refresh();
    }

    void Curses::_refresh() {
        refresh();
    }

    void Curses::stop () {
        _refresh();
        endwin();
    }

}
// kate: indent-mode cstyle; space-indent on; indent-width 4;
