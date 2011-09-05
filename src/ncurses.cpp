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
#include "plugins.hpp"
#include <wntrdata.hpp>
#include <wntrling.hpp>
#include <QString>
#include <QtDebug>
#include <QStringList>
#include <QTextStream>
#include <iostream>

using namespace Wintermute::Data;
using namespace Wintermute::Plugins;
using namespace std;

namespace Wintermute {
    static WINDOW* s_mnWndw = NULL;

    Curses::Curses() { }

    Curses::Curses ( const Curses &p_crs ) { }

    Curses::~Curses () { }

    void Curses::start() {
        cout << "(core) [Curses] Starting...";
        usleep(100);
        _init();

        printText(QString("\nWintermute v.") + QString::number (WINTERMUTE_VERSION) +
                  QString("\n** Visit us at <http://www.thesii.org> or on Freenode (IRC) in ##sii-general. **\n") +
                  QString("Press 'h' for more information."));

        _doCommands();
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

    void Curses::getText (const QString &p_prompt, QString &p_result){
        printStatus(p_prompt);
        _read (p_result);
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
        keypad(stdscr,true); echo();
    }

    void Curses::_doCommands() {
        char l_chr ;
        while (l_chr != 'q'){
            printStatus ("Command: ");
            l_chr = getch(); clear();

            printStatus ("Command: " + QString(l_chr));
            QByteArray *l_data = new QByteArray;
            switch (l_chr){
                case 'h': {
                    QTextStream l_dataStrm(l_data);
                    l_dataStrm << "Wintermute doesn't do much currently in it's nCurses" << endl
                               << "interface. We're working to get this fixed, but it's really"  << endl
                               << "easier said than done." << endl << endl
                               << "Commands:" << endl
                               << "d: Dump internal data (locale, directories, etc)." << endl
                               << "g: Start X11/GUI interface" << endl;

                    QString l_str(*l_data);
                    printText(l_str);
                } break;

                case 'd': {
                    QTextStream l_dataStrm(l_data);
                    l_dataStrm << "Locale:" << Data::Linguistics::Configuration::locale() << "(default: " << WNTRDATA_DEFAULT_LOCALE << ")"<< endl
                               << "Root data directory:" << Data::Configuration::directory () << "(default: " << WNTRDATA_DATA_DIR << ")" << endl;

                    QString l_str(*l_data);
                    printText(l_str);
                } break;

                case 'g': {
                    printStatus("Starting graphical user interface..");
                    stop();
                    Factory::loadPlugin("/usr/lib/libwntrgui.so");
                    return;
                } break;

                case 'q': {
                    clear();
                    printStatus("Exitting...\n");
                    stop();
                    WNTR_APPLICATION::quit ();
                } break;

                case 'l': {
                    printStatus("Starting linguistics parser...");
                    QString l_str;
                    Linguistics::Parser l_prsr;
                    getText ("Input: ",l_str);

                    while (!l_str.isEmpty ()){
                        l_prsr.parse(l_str);
                        getText ("Input: ",l_str);
                    }

                    clear();
                } break;

                default: {
                    printText ("Unknown command: '" + QString(l_chr) + "'.");
                } break;
            }

            _refresh ();
        }
    }

    void Curses::_print(const int p_row, const int p_col, const QString& p_txt){
        mvprintw (p_row,p_col,p_txt.toStdString().c_str());
        _refresh();
    }

    void Curses::_read(QString& l_str){
        char* l_data;
        getstr(l_data);

        if (!QString(l_data).isEmpty ())
            l_str = l_data;
        else
            l_str.clear ();
    }

    void Curses::_refresh() {
        refresh();
    }

    void Curses::stop () {
        _refresh();
        endwin();
        clear();
        fixterm();
        resetterm();
        qDebug() << "(core) [Curses] Halted.";
    }

}
// kate: indent-mode cstyle; space-indent on; indent-width 4;
