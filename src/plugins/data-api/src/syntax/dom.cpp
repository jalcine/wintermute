/**
 * @file rules.hpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 * @date Sun, 30 Oct 2011 21:54:16
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

// stdc++ includes
#include <algorithm>

// Qt includes
#include <QFile>

// local includes
#include "config.hpp"
#include "bond.hpp"
#include "model.hpp"
#include "dom.hpp"
#include "../linguistics.hpp"

using namespace std;
using namespace Wintermute::Data::Linguistics::Syntax;
using Wintermute::Data::Linguistics::System;

DomBackend::DomBackend() : AbstractBackend() { }

DomBackend::DomBackend (const DomBackend& p_bnd) : m_elem (p_bnd.m_elem) { }

DomBackend::DomBackend (QDomElement* p_ele) : m_elem (p_ele) { }

DomBackend::~DomBackend () { }

DomLoadModel::DomLoadModel() { }

DomLoadModel::DomLoadModel (const DomLoadModel& p_mdl) : AbstractLoadModel (p_mdl), DomBackend (p_mdl.m_elem) { }

DomLoadModel::DomLoadModel (QDomElement* p_ele) : DomBackend (p_ele) { }

const Chain* DomLoadModel::load () const
{
    Chain* chn = new Chain;
    BondList bndVtr;
    obtainBonds (&bndVtr, m_elem);
    unique (bndVtr.begin (), bndVtr.end ());

    chn->setType (obtainType (m_elem));
    chn->setBonds (bndVtr);

    return chn;
}

void DomLoadModel::obtainBonds (BondList* p_bndVtr, const QDomElement* p_elem) const
{
    if (p_elem->nodeName () != "Rule")
        return;

    QDomNodeList lst = p_elem->elementsByTagName ("Bind");

    for (int i = 0; i < lst.count (); i++) {
        QDomElement elem = lst.at (i).toElement ();
        Bond* bnd = new Bond;
        QDomNamedNodeMap attrs = elem.attributes ();

        for (int i = 0; i < attrs.length (); i++) {
            QDomAttr attr = attrs.item (i).toAttr ();
            const QString nm = attr.name ();
            QString vl = attr.value ();
            bnd->setAttribute (nm, vl);
        }

        p_bndVtr->push_back (bnd);
    }

    if (!p_elem->parentNode ().isNull ()) {
        QDomElement prntNode = p_elem->parentNode ().toElement ();
        obtainBonds (p_bndVtr, &prntNode);
    }
}

const QString DomLoadModel::obtainType (const QDomElement* p_elem) const
{
    QString type;
    QDomElement elem = *p_elem;

    while (!p_elem->isNull()) {
        type.prepend (elem.attribute ("type"));
        elem = elem.parentNode().toElement ();
    }

    return type;
}

/// @todo Allow rules to be inherited upon load.
void DomLoadModel::loadTo (Chain& p_chn) const
{
    BondList bndVtr;
    obtainBonds (&bndVtr, m_elem);

    unique (bndVtr.begin (), bndVtr.end ());

    p_chn.setType (obtainType (m_elem));
    p_chn.setBonds (bndVtr);
}

DomLoadModel::~DomLoadModel () { }

DomSaveModel::DomSaveModel() { }

DomSaveModel::DomSaveModel (QDomElement* p_ele) : DomBackend (p_ele) { }

DomSaveModel::DomSaveModel (const DomSaveModel& p_mdl) : AbstractSaveModel (p_mdl), DomBackend (p_mdl.m_elem) { }

void DomSaveModel::save () { }

void DomSaveModel::saveFrom (const Chain& p_chn)
{
    const BondList bndVtr = p_chn.bonds();
    const QString bndTyp = p_chn.type();

    setBonds (bndVtr);
    setType (bndTyp);
    save();
}

void DomSaveModel::setBonds (const BondList& p_bndVtr) { }

void DomSaveModel::setType (const QString& p_bndTyp) {}

DomSaveModel::~DomSaveModel () { }

DomStorage::DomStorage() : m_min (DOMSTORAGE_MAXSTR) { }

DomStorage::DomStorage (const AbstractStorage& p_str) : AbstractStorage (p_str) { }

const QString DomStorage::getPath (const Chain& p_chn)
{
    return System::directory () + "/" + p_chn.locale () + "/grammar.xml";
}

QDomDocument* DomStorage::loadDom (const Chain& p_chn)
{
    DomStorage str;

    if (!str.exists (p_chn.locale (), p_chn.type ())) {
        qWarning() << "(data) [DomStorage] Can't find grammar for" << p_chn.locale ();
        return 0;
    }

    const QString pth = getPath (p_chn);

    QDomDocument* dom = new QDomDocument;

    {
        QString errorString;
        int errorLine, errorColumn;

        if (!dom->setContent ( (new QFile (pth)), &errorString, &errorLine, &errorColumn)) {
            qWarning() << "(data) [DataStorage] Error loading grammar:" << errorLine << "at l." << errorLine << ", col." << errorColumn;
            return 0;
        }
    }

    return dom;
}

bool DomStorage::exists (const QString& p_flg, const QString& p_lcl) const
{
    Chain chn (p_lcl,p_flg);
    const QString pth = getPath (chn);
    return QFile::exists (pth);
}

/// @todo We need to figure out a more approriate minimum value.
QDomElement DomStorage::findElement (const Chain& p_chn, QDomElement p_elem) const
{
    QDomElement elem = findElement (p_chn, p_elem, QString::null);
    const double minimum = (1.0 / (double) p_chn.type ().length ());

    while (elem.isNull ()) {
        m_min -= 0.01; // Decrease it by 1%.
        elem = findElement (p_chn, p_elem, QString::null);

        if (!elem.isNull ())
            break;

        if (m_min == minimum)
            break;
    }

    qDebug() << "(data) [DomStorage] Minimum matching:" << minimum * 100 << "%";
    m_min = DOMSTORAGE_MAXSTR;
    return elem;
}

QDomElement DomStorage::findElement (const Chain& p_chn, QDomElement p_elem, QString p_prefix) const
{
    if (p_elem.hasAttribute ("type") && p_elem != p_elem.ownerDocument ().documentElement ()) {
        const QString data = p_elem.attribute ("type");
        const QStringList lst = data.split (",");

        foreach (const QString part, lst) {
            const QString prefix = p_prefix + part;
            const double match = Bond::matches (p_chn.type (), prefix);

            if (match == 1.0) {
                qDebug() << "(data) [DomStorage] findElement: " << p_chn.type () << prefix << match;
                return p_elem;
            }
        }

        p_prefix.append (data);
    }

    QDomNodeList lst = p_elem.elementsByTagName ("Rule");

    for (uint i = 0; i < lst.length (); i++) {
        QDomElement elem = lst.at (i).toElement (), rtn;
        rtn = findElement (p_chn, elem, p_prefix);

        if (!rtn.isNull ())
            return rtn;
        else continue;
    }

    return * (new QDomElement);
}

void DomStorage::loadTo (Chain& p_chn) const
{
    QDomDocument* dom = loadDom (p_chn);
    QDomElement elem = findElement (p_chn, dom->documentElement());

    if (elem.isNull()) {
        qWarning() << "(data) [DataStorage] No rule can satisfy.";
        return;
    }

    DomLoadModel ldMdl (&elem);
    ldMdl.loadTo (p_chn);
}

void DomStorage::saveFrom (const Chain& p_chn)
{
    QDomDocument* dom = loadDom (p_chn);
    QDomElement elem = findElement (p_chn, dom->documentElement());

    if (elem.isNull()) {
        qWarning() << "(data) [DataStorage] No rule can satisfy.";
        return;
    }

    DomSaveModel svMdl (&elem);
    svMdl.saveFrom (p_chn);
}

QString DomStorage::type () const
{
    return "Dom";
}

DomStorage::~DomStorage() { }
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
