/**
 * @file ncurses.hpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 *
 * @section lcns Licensing
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
 *
 */
#ifndef NCURSES_HPP
#define NCURSES_HPP

#include "config.hpp"
#include <QObject>
#include <QMap>
#include <QStringList>
#include <ncurses.h>

namespace Wintermute
{
struct Curses;
struct Text;

/**
 * @brief Represents a block of text to be used with Curses.
 * @todo Add operator>> for easy Text construction.
 * @todo Add cast conversions from Text to QString.
 * @note No white-space formatting is done by Text; that's handled by Curses.
 * @class Curses ncurses.cpp "src/ncurses.cpp"
 */
class Text : public QObject
{
    Q_OBJECT

    Q_ENUMS (Alignment)
    Q_PROPERTY (const Alignment& Alignment READ alignment WRITE setAlignment)
    Q_PROPERTY (const QString& Text READ text WRITE setText)

public:
    enum Alignment {
        Left = 0,
        Right,
        Center
    };

    Text();
    Text (const Text&);
    Text (const QString&);
    Text (const QStringList&);

    void setAlignment (const Alignment& = Left);
    void setText (const QString& = QString::null);

    const Alignment& alignment() const;
    const QString& text() const;

private:
    Alignment m_algn;
    QString m_txt;
};

/**
 * @brief Provides the front-facing terminal user interface for Wintermute.
 *
 * In an effort to provide another less resource-intenstive interface, aside
 * from the QtGui module, Wintermute::Curses is here to implement a terminal
 * interface for Wintermute.
 *
 * @class Curses ncurses.cpp "src/ncurses.cpp"
 */
class Curses : public QObject
{
    Q_OBJECT
    Q_ENUMS (Panes)
    Q_PROPERTY (const Panes& Pane READ pane WRITE setPane)
    Q_DISABLE_COPY (Curses)

public:
    enum Panes {
        Help = 0,
        Settings,
        TEaR,
        Ling,
        Onto,
        Welcome,
        Dialog
    };

    Curses();
    ~Curses();
    static Curses* instance();
    static const int windowWidth();
    static const int windowHeight();

    void getText (QString&) const;
    void setText (const Text&);
    void setStatus (const QString&);
    void setProgress (const double&);
    const int showDialog (const QString&, const QMap<int, QString>&, const int = 0);
    const Panes& pane() const;
    void setPane (const Panes&);

public slots:
    void start();
    void stop();
    void clear (const Panes& = Welcome);

private:
    void _update();
    static Curses* s_inst;
    Panes m_pn;
};
}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
