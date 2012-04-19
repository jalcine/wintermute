/***
 *  This file is part of the Wintermute project.
 *
 *  Copyright (C) 2012 Jacky Alciné <jacky.alcine@thesii.org>
 *
 *  Wintermute is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  Wintermute is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with Wintermute.
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jacky.alcine@thesii.org>
 * @date 04/18/12 7:02:17 PM
 */

#include <QDir>
#include <QFile>
#include "data-api/linguistics.hpp"
#include "data.hpp"
#include "model.hpp"
#include "dom.hpp"

using namespace Wintermute::Data::Linguistics::Lexical;

DomBackend::DomBackend (const QDomElement& p_ele) : AbstractBackend(), m_ele (p_ele) { }

DomStorage::DomStorage() : Storage() { }

const QString DomStorage::getPath (const Data& p_data)
{
    const QString bsPth = System::directory () + QString ("/") + p_data.locale() + QString ("/node/");
    QDir* bsDir = new QDir (bsPth);

    if (!bsDir->exists() && bsDir->mkpath (bsPth)) {
        qDebug() << "(data) [DomStorage::getPath()] Made path" << bsDir->absolutePath();
    }

    return bsPth + p_data.id() + QString (".node");
}

bool DomStorage::exists (const Data& p_data) const
{
    const QString pth = DomStorage::getPath (p_data);
    qDebug() << "(data) [DomStorage::exists()] Exists?"  << pth << QFile::exists (pth);
    return QFile::exists (pth);
}

bool DomStorage::loadTo (Data& p_data) const
{
    if (!DomStorage::exists (p_data))
        return false;

    QDomDocument dom ("Data");
    QFile* file = new QFile (getPath (p_data));
    dom.setContent (file);

    QDomElement elem = dom.documentElement ();
    DomLoadModel model (elem);
    return model.loadTo (p_data);
}

/// @todo Add a timestamp and system-user information to the generated file.
/// @todo When semantics become powerful, add the generating Wintermute's ID (and place of origin, if possible).
bool DomStorage::saveFrom (const Data& p_data)
{
    QDomDocument dom ("Data");
    QDomElement elem = dom.createElement ("Data");
    dom.appendChild (elem);

    DomSaveModel domSvMdl (elem);
    domSvMdl.saveFrom (p_data);
    const QString str = dom.toByteArray (4);

    QFile* file = new QFile (DomStorage::getPath (p_data));

    if (!file->exists ()) {
        file->open (QIODevice::WriteOnly | QIODevice::Truncate);
        file->write ("<!--Generated-->\n");
        file->close ();
    }

    file->open (QIODevice::WriteOnly | QIODevice::Append);
    file->write (str.toUtf8());
    file->close ();
    return true;
}

QString DomStorage::type () const
{
    return "Dom";
}

void DomStorage::generate()
{
    QDir d (System::directory ());
    d.setFilter (QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList localeLst = d.entryList ();

    foreach (const QString locale, localeLst) {
        qDebug() << "(data) [DomStorage] Parsing locale" << locale << "...";
        const QString pth = d.absolutePath () + "/" + locale + "/node.xml";
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

QString DomStorage::obtainFullSuffix (const QString& p_locale, const QString& p_suffix) const
{
    const Data dt (QString::null, p_locale);
    const QDomDocument spawnDoc = getSpawnDoc (dt);

    if (spawnDoc.isNull()) {
        qWarning() << "(ling) [DomStorage] Data not found for locale" << p_locale;
        return QString::null;
    }

    const QDomElement dom = spawnDoc.documentElement ();

    const QDomNodeList domList = dom.elementsByTagName ("Mapping").at (0).toElement ().elementsByTagName ("Suffix");

    for (int i = 0; i < domList.count (); i++) {
        QDomElement ele = domList.at (i).toElement ();

        if (ele.attribute ("from") == p_suffix)
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
        QDomElement currentElement = lst.at (i).toElement ();

        if (currentElement.isNull ()) continue;

        DomLoadModel loadModel (currentElement);
        QDomDocument newDom ("Data");
        QDomElement rootNode = newDom.createElement ("Data");
        newDom.appendChild (rootNode);

        Data dt = loadModel.load();
        dt.setLocale (lcl);
        DomSaveModel saveModel (rootNode);
        saveModel.saveFrom (dt);

        QFile* file = new QFile (DomStorage::getPath (dt));

        if (!file->open (QIODevice::WriteOnly | QIODevice::Truncate)) {
            file->setPermissions (QFile::WriteOther);
            qWarning() << "(data) [DomStorage] Failed to spawn node" << dt.id() << ":" << file->errorString();
        }
        else {
            QTextStream stream (file);
            stream << newDom.toByteArray (4);
            file->close ();
            qDebug() << "(data) [DomStorage] Wrote" << dt.id() << "with" << file->size() << "bytes.";
        }
    }

    qDebug () << "(data) [DomStorage] Locale" << lcl << "spawned.";
}

QDomDocument DomStorage::getSpawnDoc (const Data& p_data)
{
    QDomDocument spawnDom;

    if (p_data.isValid()) {
        QDir dir (System::directory ());
        const QString pth = dir.absolutePath () + "/" + p_data.locale () + "/node.xml";
        QFile* file = new QFile (pth);

        if (!file->exists () || !dir.exists ()) {
            qWarning() << "(data) [DomStorage] Can't find node cache for" << p_data.locale ();
        }
        else {
            if (!spawnDom.setContent (file)) {
                qWarning() << "(data) [DomStorage] Parse error for" << p_data.locale ();
            }
        }
    }

    return spawnDom;
}

bool DomStorage::hasPseudo (const Data& p_data) const
{
    QDomDocument dom = getSpawnDoc (p_data);

    if (!dom.isNull()) {
        QDomElement root = dom.documentElement ();
        QDomNodeList lst = root.elementsByTagName ("Pseudo");
        return !lst.isEmpty ();
    }

    return false;
}

bool DomStorage::loadPseudo (Data& p_data) const
{
    if (!hasPseudo (p_data))
        return false;

    const QDomDocument dom = getSpawnDoc (p_data);

    if (!dom.isNull()) {
        QDomElement rootNode = dom.documentElement ();
        QDomNodeList pseudoNodeList = rootNode.elementsByTagName ("Pseudo");
        QDomElement pseudoElement = pseudoNodeList.at (0).toElement ();

        DomLoadModel loadModel (pseudoElement);
        return loadModel.loadTo (p_data);
    }

    return false;
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
            data.setId (nodeID);
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
        const QStringList nodeIDs = Cache::allNodes (locale);
        cnt += nodeIDs.count ();
    }

    return cnt;
}

DomStorage::~DomStorage() { }

DomLoadModel::DomLoadModel (QDomElement& p_elem) : AbstractLoadModel(),
    DomBackend (p_elem)
{

}

DomLoadModel::~DomLoadModel() { }

Data DomLoadModel::load () const
{
    if (loadTo (m_dt))
        return m_dt;
    else {
        qWarning() << "(data) [DomLoadModel] Failed to load" << m_dt.id () << m_dt.symbol ();
        return Data::Empty;
    }

    return Data::Empty;
}

bool DomLoadModel::loadTo (Data& p_data) const
{
    if (m_ele.isNull ()) {
        qCritical () << "(data) [DomLoadModel] Backend's empty.";
        return false;
    }

    QVariantMap* mp = new QVariantMap;
    QDomNodeList ndlst = m_ele.childNodes();

    if (!ndlst.isEmpty ()) {
        for (int i = 0; i < ndlst.count (); i++) {
            QDomElement elem = ndlst.at (i).toElement ();

            if (!elem.isNull()) {
                mp->insert (elem.attribute ("guid"), elem.attribute ("link"));
            }
        }
    }

    p_data.setSymbol (m_ele.attribute ("symbol").toLower());
    p_data.setFlags (*mp);
    qDebug() << "(data) [DomLoadModel] Data loaded to: " << p_data.toJson();
    return true;
}

DomSaveModel::DomSaveModel (QDomElement& p_elem) : AbstractSaveModel(),
    DomBackend (p_elem)
{

}

DomSaveModel::~DomSaveModel() { }

void DomSaveModel::save()
{
    if (m_ele.isNull() || !m_dt.isValid()) return;

    m_ele.setAttribute ("symbol" , m_dt.symbol().toLower ());
    m_ele.setAttribute ("locale" , m_dt.locale ());

    while (m_ele.hasChildNodes ()) {
        m_ele.removeChild (m_ele.firstChild ());
    }

    QDomDocument dom = m_ele.ownerDocument ();

    if (!m_dt.flags().empty()) {
        QStringList keys = m_dt.flags().keys();

        Q_FOREACH (const QString key, keys) {
            QDomElement ele = dom.createElement ("Flag");
            m_ele.appendChild (ele);
            const QVariant value = m_dt.flags().value (key);
            ele.setAttribute ("guid", key);
            ele.setAttribute ("link", value.toString());
        }
    }

    qDebug() << dom.toString();
    qDebug() << "(data) [DomSaveModel] Saved" << m_dt.id () << m_dt.symbol();
}

void DomSaveModel::saveFrom (const Data& p_data)
{
    if (p_data.isValid())
        m_dt = p_data;

    save();
}

#include "lexical/dom.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
