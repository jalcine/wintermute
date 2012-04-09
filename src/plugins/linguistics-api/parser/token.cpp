/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 03/04/12 6:03:56 AM
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
 * @endlegalese
 */

#include <QDebug>
#include <QStringList>
#include "token.hpp"

using namespace Wintermute::Linguistics;

Token::Token() : m_prfx(), m_sffx(), m_data() { }
Token::Token (const Token& p_tok) : m_prfx (p_tok.m_prfx), m_sffx (p_tok.m_sffx), m_data (p_tok.m_data) { }
Token::Token (const QString& p_tokStr) : m_prfx(), m_sffx(), m_data()
{
    __init (p_tokStr);
}

/// @todo Implement a means of increasing the precision when dissecting strings.
void Token::__init (const QString& p_tokStr)
{
    int mode = 0;
    foreach (const QChar chr, p_tokStr) {
        switch (mode) {
        case 0: {
            /// Prefix symbol
            if (!chr.isLetterOrNumber())
                m_prfx += chr;
            else {
                mode = 1;
                m_data += chr;
            }
        }
        break;

        case 1: {
            /// Root symbol
            if (chr.isLetterOrNumber())
                m_data += chr;
            else {
                mode = 2;
                m_sffx += chr;
            }
        }
        break;

        case 2: {
            /// Suffix symbol
            m_sffx += chr;
        }
        break;
        }
    }

    //qDebug() << "Token:" << m_prfx << m_data << m_sffx;
}

const QString Token::symbol() const
{
    return m_data;
}
const QString Token::prefix() const
{
    return m_prfx;
}
const QString Token::suffix() const
{
    return m_sffx;
}

const TokenList Token::form (const QString& p_tknStr)
{
    TokenList tknLst;

    Q_FOREACH (const QString tkn, p_tknStr.split (" ", QString::SkipEmptyParts)) {
        const QChar firstChar = tkn.at (0);

        if (firstChar.isLetterOrNumber() || firstChar.isPunct())
            tknLst << new Token (tkn);
        else
            qDebug() << "(ling) [Token] Invalid token" << tkn;
    }

    return tknLst;
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
