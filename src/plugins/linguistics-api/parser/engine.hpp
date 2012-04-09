/**
 * @file wntrling.hpp
 * This file is part of Wintermute Linguistics
 *
 * Copyright (C) 2011 - Wintermute Development <wntr-devel@thesii.org>
 *
 * Wintermute Linguistics is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute Linguistics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute Linguistics; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef WNTRLING_ENGINE_HPP
#define WNTRLING_ENGINE_HPP

// Qt includes
#include <QObject>
#include <QStringList>

// WntrData includes
#include <data-api/linguistics.hpp>

// local includes
#include "syntax/node.hpp"
#include "meanings.hpp"

namespace Wintermute
{
namespace Linguistics
{

/**
  * @brief Encapsulates the primary object used to cast a simple string representing a bit of language into machine-interpretable ontological information.
  *
  * The Parser object handles the pre- and post-requirements of the natural
  * language processing process. It's broken down into the following fashion:
  * @dot
  *            digraph L {
  *                    graph [layout=dot rankdir=TB]
  *
  *                    {"Santization"}  -> {"Detection"}
  *                    {"Detection"}    -> {"Expansion"}
  *                    {"Expansion"}    -> {"Transversing"}
  *                    {"Transversing"} -> {"Interpreting"}
  *                    {"Interpreting"} -> {"Execution"}
  *            }
  * @enddot
  *
  * @section N01 The Process
  * @subsection N02 Santizating the Text
  * Wintermute goes through a partly intricate means of ensuring the real
  * meaning of text is preserved while it's still made <b>legible</b> to the
  * system for detection. Towards the close of santizating, a collection of
  * Token objects are created and are passed in a TokenList for ease of use.
  * @see Token, TokenList
  *
  * @subsection N03 Detecting Words in the Text
  * Now, after santizating has been done; we now look at each token in the TokenList
  * and determine the possible Node that's been represented by the token. Note that
  * we aren't looking for the <i>meaning</i> of the word just yet; we're merely
  * attempting to <i>recognize</i> the word being displayed. This is then packed
  * into a NodeList and is used for expansion.
  * @see Node, NodeList
  *
  * @subsection N04 Expanding into Paths
  * In some languages, one word can have multiple values. Some languages don't have
  * this issue, but other languages (like English or other Romance languages) tend
  * to have words that when put in a sentence, hold an ambigious meaning. Such ambiguity
  * is hard to iron out and is currently (but partly) tackled by <b>expansion</b>.
  * Expansion is the act of Wintermute detecting the number of potential nodes in a word.
  * For example, the English sentence, "<b>We are boys</b>", can be represented as:
  * @dot
  *            digraph L {
  *                    graph [layout=dot rankdir=RL]
  *
  *                    {"We"} -> {"are"} -> {"boys"}
  *            }
  *
  * @enddot
  * However, the words 'We' and 'are' have more than one potential meaning in English,
  * as shown below; thus creating an ambiguity in the sentence. Is it referring to the
  * third person as a collection of individuals as an object (We)? Is it referring to
  * the second person or the third person (are)?
  * @dot
  *            digraph L {
  *                    graph [layout=dot rankdir=TB]
  *
  *                    {"We"} -> {"We-1"}
  *                    {"We"} -> {"We-2"}
  *                    {"We"} -> {"We-3"}
  *                    {"We"} -> {"We-4"}
  *
  *                    {"are"} -> {"are-1"}
  *                    {"are"} -> {"are-2"}
  *           }
  * @enddot
  * This now caused our one-path sentence to potentially have <i>eight</i> different interpretations.
  * This is shown internally as so.
  * @dot
  *            digraph L {
  *                    graph [layout=dot rankdir=TB]
  *
  *                    {"We"} -> {"We-1"}
  *                    {"We"} -> {"We-2"}
  *                    {"We"} -> {"We-3"}
  *                    {"We"} -> {"We-4"}
  *
  *                    {"are"} -> {"are-1"}
  *                    {"are"} -> {"are-2"}
  *
  *                    {"We-1"} -> {"are-1"}
  *                    {"We-2"} -> {"are-1"}
  *                    {"We-3"} -> {"are-1"}
  *                    {"We-4"} -> {"are-1"}
  *
  *                    {"We-1"} -> {"are-2"}
  *                    {"We-2"} -> {"are-2"}
  *                    {"We-3"} -> {"are-2"}
  *                    {"We-4"} -> {"are-2"}
  *
  *                    {"are-1"} -> {"boys"}
  *                    {"are-2"} -> {"boys"}
  *          }
  * @enddot
  * Wintermute now goes through each path and pushes the path (not its potential Meaning) to the
  * back of a NodeTree. These trees are used for the transversion.
  * @see Node, NodeList, NodeTree
  *
  * @subsection N05 Transversing a Tree
  * The act of Wintermute transversing over a tree is simply the back-to-back interpretation of
  * similar sentences and then determining which sentence is more approriate for the current context.
  * Currently, context-detection isn't in order, so only the sentences that are grammatically correct
  * and return an equivalency of its originating form are pushed to the back of a MeaningList and
  * passed over to the interpretation handle.
  * @see Meaning, Binding, Rule, BindingList, RuleList, MeaningList
  *
  * @subsection N06 Interpretating Information
  * After the lexicosyntactic representation (a Meaning object) of each valid sentence
  * has been found, we transverse once more over this list to determine the potential
  * <b>interpretation</b> of said objects. This is done by rendering the objects as
  * ontological concepts and from there, generating a list of valid ontologic objects
  * to be executed.
  *
  * @subsection N07 Execution
  * Execution is done very similarly to how a typical computer program would be done.
  * Currently, Wintermute would go about exeuction by discovering all of the required
  * ontological concepts to be used by the concept (most, if not all, of which should be
  * already loaded during the interpretation phase) and saturating the concept to hold
  * its inherited attributes, in a fashion to how a program would external symbols from
  * shared libraries at load-time. From there, the head-link of the Meaning object is used to go about the execution
  * process. For example, if a sentence said, "My name is Jacky"; it'd discover the
  * means of finding out if first the statement is logically true, on a scale, using fuzzy logic
  * (http://en.wikipedia.org/Fuzzy_logic), of 0 to 1 (0% to 100%). The higher the rating,
  * the more confidient Wintermute is about the statement. If confidient enough, it then
  * generates an approriate response.
  *
  * @endsection
  * There's a pantheon of errors, mistakes and complications that haven't been calculated
  * currently into the coding that could cause subtle exceptions in interpretation, execution
  * or even santization that'd mess up the potential output of Wintermute.
  *
  * @attention Using Big O notation, the parser avoids running into memory intenstive operations. The typical execution size is O(n1) * ...  * O(n(x)).
  *            Typically words have only one to three definitions. But if a sentence has words with 6 different meangins and there's 40 words; things get hairy <b>fast</b>.
  * @class Parser parser.hpp "src/parser.hpp"
  */
class Engine : public QObject
{
    Q_OBJECT

    Q_PROPERTY (const QString locale READ locale WRITE setLocale)

public:
    /**
      * @brief Copy constructor.
      * @fn Parser
      * @param p_prsr The Parser to be copied.
      */
    Engine (const Engine& p_prsr) : m_lcl (p_prsr.m_lcl) {}

    /**
      * @brief Default constructor.
      * @fn Parser
      * @param p_lcl The locale for the parser to use (default is Wintermute's default locale).
      */
    explicit Engine (const QString& = Wintermute::Data::Linguistics::System::locale ());

    /**
      * @brief Deconstructor.
      * @fn ~Parser
      */
    ~Engine() { }

    /**
      * @brief Returns the locale of the Parser.
      * @fn locale
      */
    const QString locale() const;

    /**
      * @brief Changes the locale of the Parser.
      * @fn setLocale
      * @param p_lcl The locale to be used by the Parser.
      * @todo Prevent this value from being changed while parsing's active; it can cause malformed data to be generated.
      */
    void setLocale (const QString& = Wintermute::Data::Linguistics::System::locale ());

    /**
      * @brief Parses user text into a semantic representation of its underlying meaning.
      * @fn parse
      * @param p_txt The text to be parsed.
      */
    void parse (const QString&);

protected:
    mutable QString m_lcl;
    int m_prg;
    int m_prgMax;

private:

    /**
      * @brief Processes the text for parsing.
      * @fn process
      * @param p_txt The text to be parsed.
      */
    const Meaning* process (const QString&);

    /**
      * @brief Breaks up the text into tokens to be used by the parser.
      * @fn getTokens
      * @param p_txt The text to be broken up.
      * @todo Return TokenList instead of QStringList.
      */
    QStringList getTokens (const QString&);

    /**
      * @brief Forms a Node from the text specified.
      * @fn formNode
      * @param p_ndTxt The symbolic representation of a Node.
      * @todo Use Token instead of QString.
      */
    Node* formNode (const QString&);

    /**
      * @brief Generates a list of Nodes from the list of tokens specified.
      * @fn formNodes
      * @param p_txtLst The list of tokens to be Node-ified.
      * @see formNode
      * @todo Use TokenList instead of QStringList.
      */
    NodeList formNodes (const QStringList&);

    /**
      * @brief
      * @fn expandNodes
      * @param
      */
    NodeTree expandNodes (NodeList const&);

    /**
      * @brief
      * @fn expandNodes
      * @param
      * @param
      * @param
      */
    NodeTree expandNodes (NodeTree& , const int& = 0, const int& = 0);
    /**
      * @brief
      * @fn formMeaning
      * @param
      */
    const Meaning formMeaning (const NodeList&);
    /**
      * @brief
      * @fn formShorthand
      * @param
      * @param
      */
    static const QString formShorthand (const NodeList& , const Node::FormatVerbosity& = Node::FULL);

    void doUnwindingProgressStep();

signals:
    /**
      * @brief
      * @fn foundPseduoNode
      * @param
      */
    void foundPseduoNode (Node* = 0);
    /**
      * @brief
      * @fn finishedTextAnalysis
      */
    void finishedTextAnalysis();
    /**
      * @brief
      * @fn unwindingProgress
      * @param
      */
    void unwindingProgress (const double & = 0.0);
    /**
      * @brief
      * @fn finishedUnwinding
      */
    void finishedUnwinding();
    /**
      * @brief
      * @fn finishedMeaningForming
      */
    void finishedMeaningForming();
};

}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
