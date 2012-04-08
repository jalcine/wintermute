/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 03/04/12 6:02:16 AM
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

#ifndef WTNRLING_TOKEN_HPP
#define WNTRLING_TOKEN_HPP

#include <QList>
#include <QObject>
#include <QMetaType>

namespace Wintermute
{
namespace Linguistics
{
class Token;
/**
 * @brief Represents a QList of Token objects.
 * @see Token
 * @typedef TokenList
 */
typedef QList<Token*> TokenList;

/**
 * @brief A wrapper class over word-symbols for use of the Parser.
 *
 * Token objects are used by the Parser to generate more accurate
 * interpretations of text. Originally, the Parser would break up text
 * whenever it saw a space, and dropped all other formatting characters.
 * With Token::form(), it can generate a list of tokens that would have a
 * English sentence like
 * @code
 * My name's Tom.
 * @endcode
 * or even a French sentence, like
 * @code
 * Je m'appelle Tom.
 * @endcode
 * converted into sentenes with approriate linkings as shown.
 *
 * @dot
    digraph L {
            graph [layout=dot rankdir=LR]

            My
            {"name's"} -> {name "'" s}
            {"Tom."} -> {Tom "."}
            My -> {"name's"} -> {"Tom."}
    }
 * @enddot
 * @dot
    digraph K {
            graph [layout=dot rankdir=LR]

            Je
            {"m'appelle"} -> {m "'" appelle}
            {"Tom."} -> {Tom "."}
            Je -> {"m'appelle"} -> {"Tom."}
    }
 * @enddot
 * The Parser can then expand these tokens into their proper Node forms.
 * @internal
 */
class Token : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Null constructor.
     * @fn Token
     */
    Token();

    /**
     * @brief Copy constructor.
     * @fn Token
     * @param The Token to be copied.
     */
    Token (const Token&);

    /**
     * @brief Default constructor.
     * @fn Token
     * @param p_tok The QString that represents a token.
     */
    explicit Token (const QString&);

    /**
     * @brief Obtains the symbol of this Token.
     * @fn symbol
     */
    const QString symbol() const;

    /**
     * @brief Obtains the prefix of this Token.
     * @fn prefix
     */
    const QString prefix() const;

    /**
     * @brief Obtains the suffix of this Token.
     * @fn suffix
     */
    const QString suffix() const;

    /**
     * @brief Generates a list of Tokens (a TokenList) from a QString.
     * @fn form
     * @param p_str The QString to be tokenized.
     */
    static const TokenList form (const QString&);

private:
    void __init (const QString&);
    QString m_prfx;
    QString m_sffx;
    QString m_data;
};


}
}
Q_DECLARE_METATYPE (Wintermute::Linguistics::Token)
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 

#endif
