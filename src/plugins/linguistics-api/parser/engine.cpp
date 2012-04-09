/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 03/04/12 6:06:41 AM
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

// stdc++
#include <algorithm>

// WntrData includes
#include <data-api/lexical/model.hpp>

// local include
#include "engine.hpp"
#include "token.hpp"
#include "syntax/node.hpp"
#include "meanings.hpp"

using namespace Wintermute::Linguistics;
using namespace std;

Engine::Engine (const QString& p_string)
{

}

const Meaning Engine::formMeaning (const NodeList& p_nodeList)
{

}

const QString Engine::locale () const
{
    return m_lcl;
}

void Engine::setLocale (const QString& p_lcl)
{
    m_lcl = p_lcl;
}

QStringList Engine::getTokens (const QString& p_str)
{
    QStringList strLst;
    foreach (const Token * tkn, Token::form (p_str)) {
        const QString fullSuffix = Lexical::Cache::obtainFullSuffix (locale(), tkn->suffix());
        strLst << tkn->symbol();

        if (!fullSuffix.isEmpty())
            strLst << fullSuffix;
    }

    return strLst;
}
#if 0
/// @todo Remove this method (but salvage the algorithm for saving a node). [Use it in ncurses-wintermute.]
void Parser::generateNode (Node* p_nd)
{
    cout << "(ling) [Parser] Encountered unrecognizable word (" << p_nd->symbol ().toStdString () << "). " << endl
         << setw (5) << right << setfill (' ')
         << "Add to system? ( yes / [n]o): ";

    QTextStream iStrm (stdin);

    if (iStrm.readLine () == "yes") {
        const Lexical::Data* dt = p_nd->data();
        cout << "(ling) Enter lexical flags in such a manner; ONTOID LEXIDATA. Press <ENTER> twice to complete the flag entering process." << endl;
        QString oid, flg, ln = iStrm.readLine ();
        QVariantMap dtmp;

        while (!ln.isNull() && !ln.isEmpty ()) {
            QStringList objs = ln.split (" ");
            oid = objs[0];
            flg = objs[1];

            dtmp.insert (oid, flg);
            ln = iStrm.readLine ();
        }

        Lexical::Data nwDt (Lexical::Data::idFromString (p_nd->symbol()), locale(), p_nd->symbol(), dtmp);
        Lexical::Cache::write (nwDt);
        p_nd = new Node (nwDt);
        qDebug() << "(ling) [Parser] Node generated." << endl;
    }
    else {
        qDebug() << "(ling) [Parser] Node creation cancelled." << endl;
        p_nd = 0;
    }
}
#endif
NodeList Engine::formNodes (QStringList const& p_tokens)
{
    NodeList theNodes;

    foreach (QString token, p_tokens) {
        Node* node = formNode (token);

        if (node)
            theNodes.push_back (node);
    }

    return theNodes;
}

Node* Engine::formNode (const QString& p_symbol)
{
    const QString theID = Lexical::Data::idFromString (p_symbol);
    Node* theNode = const_cast<Node*> (Node::obtain (m_lcl, theID));

    if (!Node::exists (m_lcl, theID)) {
        theNode = Node::buildPseudo (m_lcl, p_symbol);
        emit foundPseduoNode (theNode);
    }

    if (theNode)
        theNode->setProperty ("OriginalToken", p_symbol);

    return theNode;
}

void Engine::doUnwindingProgressStep()
{
    m_prg += 1;
}

/// @todo Find a means of reporting progress from this method; this method can end up becoming extremely time-consuming.
NodeTree Engine::expandNodes (NodeTree& p_tree, const int& p_size, const int& p_level)
{
    if (p_level == p_tree.size ()) {
        qDebug() << "(ling) Cannot unwind anymore.";
        return (NodeTree());
    }

    const NodeList curBranch = p_tree.at (p_level);

    const bool atEnd = (p_level + 1 == p_tree.size ());

    if (curBranch.isEmpty ()) {
        qDebug() << "(ling) [Parser] WARNING: Null data detected at level" << p_level << ".";
        return (NodeTree());
    }

    const int maxSize = p_size / curBranch.size ();

    NodeTree childBranches, foundStems = expandNodes (p_tree , maxSize , p_level + 1);

    foreach (Node * curNodeForm, curBranch) {
        if (!atEnd) {
            foreach (NodeList curLst, foundStems) {
                NodeList tmpLst; // creates the current vector [1 of x, x = curBranch.size()];
                tmpLst << curNodeForm << curLst;
                childBranches << tmpLst; // add this current branch to list.
                doUnwindingProgressStep();
            }
        }
        else {   // the end of the line!
            NodeList tmpLst;
            tmpLst << curNodeForm;
            childBranches << tmpLst; // add this current branch to list.
            doUnwindingProgressStep();
        }
    }

    qDebug() << "(ling) [Parser] Tier" << (p_tree.size () * p_level) << ( (childBranches.size () != p_size) ? (QString ("generated") + QString::number (childBranches.size()) + QString ("of its") + QString (p_size)) : (QString ("all of its"))).toStdString ().c_str () << "expected branches.";
    return childBranches;
}

/**
  *  @note This method uses the formula for determining the maximum amount of paths.
  *      The formula for determining the length is:
  *      f(w) = (W1) * (W2) * ... (Wn)
  *
  *      where w is the number of words passed,
  *            W1, W2, .. , Wn is the total number of path that are represented by that node and,
  *            f(w) represents the resulting number of paths.
  *
  *      i.e: "We are boys" => 2,3,2.
  *
  *      p = W1 * W2 * W3
  *        =  2 *  3 *  2
  *        = 12
  */
NodeTree Engine::expandNodes (const NodeList& p_baseNodeVtr)
{
    qDebug() << "(ling) [Parser] Expanding node list...";
    int totalPaths = 1;
    NodeTree metaTree;

    if (!p_baseNodeVtr.isEmpty ()) {
        for (NodeList::ConstIterator itr = p_baseNodeVtr.begin (); itr != p_baseNodeVtr.end (); itr++) {
            const Node* curNode = *itr;
            NodeList curNodeForms = Node::expand (curNode);
            const unsigned int size = curNodeForms.size ();

            qDebug() << "(ling) [Parser]" << curNode << "provided " << size << "forms";

            if (size == 0) {
                qDebug() << "(ling) [Parser] No alternate forms provided by" << curNode;
                Q_ASSERT (size >= 1);
                return NodeTree();
            }

            if (itr != p_baseNodeVtr.begin())
                totalPaths *= size;

            metaTree << curNodeForms;
        }

        qDebug() << "(ling) [Parser] Expecting" << totalPaths << "path(s).";

        emit unwindingProgress (0.0);
        NodeTree tree = expandNodes (metaTree , totalPaths , 0);
        emit unwindingProgress (1.0);

        qDebug() << "(ling) [Parser] Found" << tree.size() << "path(s).";

        emit finishedUnwinding();
        return tree;
    }
    else {
        qDebug() << "(ling) [Parser] No nodes to generate paths from found.";
        return NodeTree();
    }

    return NodeTree();
}

/// @todo Determine a means of generating unique signatures for each set of Nodes (to reduce the unwinding time).
const QString Engine::formShorthand (const NodeList& p_ndVtr, const Node::FormatVerbosity& p_sigVerb)
{
    QString ndShrthnd;

    for (NodeList::const_iterator itr = p_ndVtr.begin (); itr != p_ndVtr.end (); ++itr) {
        const Node* nd = *itr;
        ndShrthnd += nd->toString (p_sigVerb);
    }

    return ndShrthnd;
}

/// @todo When parsing multiple sentences back-to-back; we need to implement a means of maintaining context. [Can't be done without implementing context ;)]
void Engine::parse (const QString& p_txt)
{
    QTextStream strm (p_txt.toLocal8Bit (), QIODevice::ReadOnly);
    MeaningList mngVtr;

    while (!strm.atEnd ()) {
        QString str = strm.readLine ();
        QStringList sentences = str.split (QRegExp ("[.!?;]\\s"), QString::SkipEmptyParts);

        foreach (QString sentence, sentences) {
            if (sentences.front () != sentence)
                qDebug() << "Parsing next sentence...";

            Meaning* mng = const_cast<Meaning*> (process (sentence));
#if 0

            /// @todo Connect this meaning to the last meaning. (Implement context?)
            if (!mngVtr.isEmpty ())
                mng->connectWith (mngVtr.last ());

#endif

            if (mng) {
                mngVtr.push_back (mng);
            }
        }
    }
}

/// @todo Obtain the one meaning that represents the entire parsed text.
const Meaning* Engine::process (const QString& p_txt)
{
    QStringList tokens = getTokens (p_txt);
    NodeList theNodes = formNodes (tokens);
    NodeTree nodeTree = expandNodes (theNodes);

    MeaningList meaningVtr;

    for (NodeTree::const_iterator itr = nodeTree.begin (); itr != nodeTree.end (); itr++) {
        const NodeList ndVtr = *itr;
        qDebug() << "(ling) [Parser] Forming meaning #" << (meaningVtr.size () + 1) << "...";
        Meaning* meaning = const_cast<Meaning*> (Meaning::form (0, ndVtr));

        if (meaning != 0)
            meaningVtr.push_back (meaning);
    }

    unique (meaningVtr.begin(), meaningVtr.end ());
    qDebug() << "(ling) [Parser]" << nodeTree.size () << "paths formed" << meaningVtr.size () << "meanings.";
    qDebug() << QString (20, '=');

    for (MeaningList::const_iterator itr2 = meaningVtr.begin (); itr2 != meaningVtr.end (); itr2++) {
        const Meaning* mngItr = *itr2;
        mngItr->toText ();
    }

    if (!meaningVtr.empty ()) return meaningVtr.front ();
    else return 0;
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
