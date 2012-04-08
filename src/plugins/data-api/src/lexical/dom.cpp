/**
 * @file dom.hpp
 * @author Jacky Alcine <jacky.alcine@thesii.org>
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

// Qt includes
#include <QDir>
#include <QFile>
#include <QDomElement>
#include <QDomDocument>

// local includes
#include "data.hpp"
#include "model.hpp"
#include "dom.hpp"
#include "../linguistics.hpp"

using namespace Wintermute::Data::Linguistics::Lexical;

DomBackend::DomBackend (QDomElement* p_ele) : m_ele (p_ele) { }

DomStorage::DomStorage() : Storage() { }

const QString DomStorage::getPath (const Data& p_dt)
{
    const QString bsPth = System::directory () + QString ("/") + p_dt.locale() + QString ("/node/");
    QDir* bsDir = new QDir (bsPth);

    if (!bsDir->exists())
        bsDir->mkpath (bsPth);

    return bsPth + p_dt.id() + QString (".node");
}

bool DomStorage::exists (const Data& p_dt) const
{
    const QString pth = DomStorage::getPath (p_dt);
    return QFile::exists (pth);
}

void DomStorage::loadTo (Data& p_dt) const
{
    if (!DomStorage::exists (p_dt))
        return;

    QDomDocument dom ("Data");
    QFile* file = new QFile (getPath (p_dt));
    dom.setContent (file);

    QDomElement elem = dom.documentElement ();
    DomLoadModel model (&elem);
    model.loadTo (p_dt);
}

/// @todo Add a timestamp and system-user information to the generated file.
/// @todo When semantics become powerful, add the generating Wintermute's ID (and place of origin, if possible).
void DomStorage::saveFrom (const Data& p_dt)
{
    QDomDocument dom ("Data");
    QDomElement elem = dom.createElement ("Data");
    dom.appendChild (elem);

    DomSaveModel domSvMdl (&elem);
    domSvMdl.saveFrom (p_dt);
    const QString str = dom.toByteArray (4);

    QFile* file = new QFile (DomStorage::getPath (p_dt));

    if (!file->exists ()) {
        file->open (QIODevice::WriteOnly | QIODevice::Truncate);
        file->write ("<!--Generated-->\n");
        file->close ();
    }

    file->open (QIODevice::WriteOnly | QIODevice::Append);
    file->write (str.toUtf8());
    file->close ();

}

const QString DomStorage::type () const
{
    return "Dom";
}

void DomStorage::generate()
{
    QDir d (System::directory ());
    d.setFilter (QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList lclLst = d.entryList ();

    foreach (const QString lcl, lclLst) {
        qDebug() << "(data) [DomStorage] Parsing locale" << lcl << "...";
        const QString pth = d.absolutePath () + "/" + lcl + "/node.xml";
        QDomDocument spawnDom ("Store");
        QFile* file = new QFile (pth);

        if (!file->exists ()) {
            qWarning() << "(data) [DomStorage] Can't access" << pth << ".";
            file->deleteLater ();
            continue;
        }

        if (!spawnDom.setContent (file)) {
            qWarning() << "(data) [DomStorage] Parse error in" << pth << ".";
            continue;
        }

        spawn (spawnDom);
    }
}

const QString DomStorage::obtainFullSuffix (const QString& p_lcl, const QString& p_sfx) const
{
    const Data dt (QString::null, p_lcl);
    const QDomDocument* spawnDoc = getSpawnDoc (dt);

    if (!spawnDoc) {
        qWarning() << "(ling) [DomStorage] Data not found for locale" << p_lcl;
        return QString::null;
    }

    const QDomElement dom = spawnDoc->documentElement ();

    const QDomNodeList domList = dom.elementsByTagName ("Mapping").at (0).toElement ().elementsByTagName ("Suffix");

    for (int i = 0; i < domList.count (); i++) {
        QDomElement ele = domList.at (i).toElement ();

        if (ele.attribute ("from") == p_sfx)
            return ele.attribute ("to");
        else
            continue;
    }

    return QString::null;
}

void DomStorage::spawn (const QDomDocument& p_dom)
{
    const QDomElement root = p_dom.documentElement ();
    const QString lcl = root.attribute ("locale");
    const QDomNodeList lst = root.elementsByTagName ("Data");
    qDebug () << "(data) [DomStorage] Spawning locale" << lcl << "...";

    for (int i = 0; i < lst.count (); i++) {
        QDomElement ele = lst.at (i).toElement ();

        if (ele.isNull ()) continue;

        DomLoadModel ldM (&ele);
        QDomDocument newDom ("Data");
        Data dt = ldM.load();
        dt.setLocale (lcl);

        QDomElement root = newDom.createElement ("Data");
        root.setAttribute ("locale", lcl);
        DomSaveModel svM (&root);
        svM.saveFrom (dt);
        newDom.appendChild (root);

        QFile* file = new QFile (DomStorage::getPath (dt));

        if (!file->open (QIODevice::WriteOnly | QIODevice::Truncate)) {
            file->setPermissions (QFile::WriteOther);
            //qWarning() << "(data) [DomStorage] Failed to spawn node" << dt.id() << ":" << file->errorString();
        }
        else {
            QDataStream stream (file);
            stream << "<!-- Generated -->" << endl
                   << newDom.toByteArray (4);
            file->close ();
            //qDebug() << "(data) [DomStorage] Wrote" << dt.id() << "with" << file->size() << "bytes.";
        }
    }

    qDebug () << "(data) [DomStorage] Locale" << lcl << "spawned.";
}

QDomDocument* DomStorage::getSpawnDoc (const Data& p_dt)
{
    QDir dir (System::directory ());
    const QString pth = dir.absolutePath () + "/" + p_dt.locale () + "/node.xml";
    QDomDocument* spawnDom = new QDomDocument ("Storage");
    QFile* file = new QFile (pth);

    if (!file->exists () || !dir.exists ()) {
        qWarning() << "(data) [DomStorage] Can't find node cache for" << p_dt.locale ();
        return 0;
    }

    if (!spawnDom->setContent (file)) {
        qWarning() << "(data) [DomStorage] Parse error for" << p_dt.locale ();
        return 0;
    }

    return spawnDom;
}

bool DomStorage::hasPseudo (const Data& p_dt) const
{
    QDomDocument* dom = getSpawnDoc (p_dt);
    QDomElement root = dom->documentElement ();
    QDomNodeList lst = root.elementsByTagName ("Pseudo");
    return !lst.isEmpty ();
}

void DomStorage::loadPseudo (Data& p_dt) const
{
    if (!hasPseudo (p_dt))
        return;

    QDomDocument* dom = getSpawnDoc (p_dt);
    QDomElement root = dom->documentElement (), psElem;
    QDomNodeList lst = root.elementsByTagName ("Pseudo");
    psElem = lst.at (0).toElement ();

    DomLoadModel ldMdl (&psElem);
    const QString sym = p_dt.symbol ();
    ldMdl.loadTo (p_dt);
    p_dt.setSymbol (sym);   // The loading process probably overwrote the symbol; depends on the definition.
}

int DomStorage::countFlags()
{
    const QStringList locales = System::locales();
    int count = 0;

    foreach (const QString locale, locales) {
        qDebug() << locale;
        Data data (QString::null, locale);
        const QStringList nodeIDs = Cache::allNodes (locale);
        foreach (const QString nodeID, nodeIDs) {
            data.setID (nodeID);
            Cache::read (data);
            count += data.flags ().count ();
        }
    }

    return count;
}

int DomStorage::countSymbols()
{
    const QStringList locales = System::locales();
    int cnt = 0;

    foreach (const QString locale, locales) {
        qDebug() << locale;
        const QStringList nodeIDs = Cache::allNodes (locale);
        cnt += nodeIDs.count ();
    }

    return cnt;
}

DomStorage::~DomStorage() { }

DomLoadModel::DomLoadModel (QDomElement* p_ele) : DomBackend (p_ele) { }

DomLoadModel::~DomLoadModel() { }

const Data DomLoadModel::load () const
{
    if (loadTo (this->AbstractModel::m_dt))
        return this->AbstractModel::m_dt;
    else {
        qWarning() << "(data) [DomLoadModel] Failed to load" << m_dt.id () << m_dt.symbol ();
        return Data::Empty;
    }

    return Data::Empty;
}

bool DomLoadModel::loadTo (Data& p_dt) const
{
    if (this->DomBackend::m_ele == 0 || this->DomBackend::m_ele->isNull ()) {
        qCritical () << "(data) [DomLoadModel] Backend's empty.";
        return false;
    }

    QVariantMap mp;
    QDomNodeList ndlst = this->DomBackend::m_ele->childNodes();

    if (!ndlst.isEmpty ()) {
        for (int i = 0; i < ndlst.count (); i++) {
            QDomElement elem = ndlst.at (i).toElement ();
            mp.insert (elem.attribute ("guid"), elem.attribute ("link"));
        }
    }

    p_dt.setSymbol (this->DomBackend::m_ele->attribute ("symbol").toLower());
    p_dt.setFlags (mp);
    //qDebug() << "(data) [DomLoadModel] Data loaded to: " << p_dt;
    return true;
}

DomSaveModel::DomSaveModel (QDomElement* p_ele) : DomBackend (p_ele) { }

DomSaveModel::~DomSaveModel() { }

void DomSaveModel::save()
{
    if (this->DomBackend::m_ele->isNull()) return;

    this->DomBackend::m_ele->setAttribute ("symbol" , this->AbstractModel::m_dt.symbol().toLower ());
    this->DomBackend::m_ele->setAttribute ("locale" , this->AbstractModel::m_dt.locale ());

    /*while (this->DomBackend::m_ele->hasChildNodes ())
        this->DomBackend::m_ele->removeChild (this->DomBackend::m_ele->firstChild ());*/

    QDomDocument dom = this->DomBackend::m_ele->ownerDocument ();

    if (!this->AbstractModel::m_dt.flags().empty()) {
        for (QVariantMap::ConstIterator itr = this->AbstractModel::m_dt.flags ().begin ();
                itr != this->AbstractModel::m_dt.flags ().end (); itr++) {
            QDomElement ele = dom.createElement ("Flag");
            ele.setAttribute ("guid", itr.key ());
            ele.setAttribute ("link", itr.value ().toString());
            this->DomBackend::m_ele->appendChild (ele);
        }
    }
    else {
        //qDebug() << "No flags for" << this->AbstractModel::m_dt.id();
    }

    //qDebug() << "(data) [DomSaveModel] Saved" << this->AbstractModel::m_dt.id ();
}

void DomSaveModel::saveFrom (const Data& p_dt)
{
    if (! (p_dt == Data::Empty))
        this->AbstractModel::m_dt = p_dt;

    save();
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
