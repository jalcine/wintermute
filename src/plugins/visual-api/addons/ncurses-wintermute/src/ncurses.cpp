/**
 * @file ncurses.cpp
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

#include <QDebug>
#include "ncurses.hpp"
#include <curses.h>

using namespace Wintermute;

static WINDOW* s_mnWndw = NULL;
Curses* Curses::s_inst = NULL;

Text::Text() : QObject() { }

Text::Text (const Text& p_txtObj) : QObject (p_txtObj.parent()), m_txt (p_txtObj.m_txt),
    m_algn (p_txtObj.m_algn) { }

Text::Text (const QString& p_str) : QObject(), m_txt (p_str), m_algn (Text::Left) { }

Text::Text (const QStringList& p_strLst) : QObject(), m_txt (p_strLst.join ("\n")), m_algn (Text::Left) { }

void Text::setAlignment (const Alignment& p_algn)
{
    m_algn = p_algn;
}

void Text::setText (const QString& p_str)
{
    m_txt = p_str;
}

const Text::Alignment& Text::alignment() const
{
    return m_algn;
}

const QString& Text::text() const
{
    return m_txt;
}

Curses::Curses() : QObject(), m_pn (Curses::Welcome) { }

Curses::Curses (const Curses& p_crs) : QObject (p_crs.parent()), m_pn (p_crs.m_pn) { }

Curses* Curses::instance()
{
    if (s_inst == NULL)
        s_inst = new Curses();

    return s_inst;
}

const int Curses::windowWidth() { }

const int Curses::windowHeight() { }

void Curses::_update() { }

void Curses::start()
{
    qDebug() << "Starting...";
    resetty();
    initscr();
}

void Curses::stop()
{
    endwin();
}

void Curses::clear (const Panes& p_pn)
{
    ::clear();
}

void Curses::getText (QString& p_input) const { }

void Curses::setText (const Text& p_txtObj) { }

void Curses::setPane (const Panes& p_pn)
{
    m_pn = p_pn;
    _update();
}

void Curses::setStatus (const QString& p_stsTxt) { }

void Curses::setProgress (const double& p_prg) { }

const int Curses::showDialog (const QString& p_txt, const QMap<int, QString>& p_opts, const int p_dft)
{

}

const Curses::Panes& Curses::pane() const
{
    return m_pn;
}

Curses::~Curses() { }
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
