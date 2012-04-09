/**
 * @file model.hpp
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
#include <QDebug>
#include <QDomElement>
#include <QDomDocument>

// local includes
#include "data.hpp"
#include "model.hpp"
#include "dom.hpp"
#include "../linguistics.hpp"

using namespace Wintermute::Data::Linguistics::Lexical;

Cache::StorageList Cache::s_stores;

AbstractModel::AbstractModel() : QObject() { }

AbstractModel::AbstractModel (Data& p_dt) : QObject (p_dt.parent()),
    m_dt (p_dt) { }

AbstractModel::AbstractModel (const AbstractModel& p_mdl) : QObject (p_mdl.parent()),
    m_dt (p_mdl.m_dt) { }

const Data& AbstractModel::data()
{
    return m_dt;
}

void AbstractModel::setData (const Data& p_dt)
{
    m_dt = p_dt;
}

AbstractModel::~AbstractModel() { }

AbstractLoadModel::AbstractLoadModel() : AbstractModel() { }

AbstractLoadModel::AbstractLoadModel (const AbstractLoadModel& p_mdl) : AbstractModel (p_mdl) { }

AbstractLoadModel::AbstractLoadModel (const AbstractModel& p_mdl) : AbstractModel (p_mdl) { }

AbstractLoadModel::~AbstractLoadModel() { }

AbstractSaveModel::AbstractSaveModel() : AbstractModel() { }

AbstractSaveModel::AbstractSaveModel (Data& p_dt) : AbstractModel (p_dt) { }

AbstractSaveModel::AbstractSaveModel (const AbstractSaveModel& p_mdl) : AbstractModel (p_mdl) { }

AbstractSaveModel::AbstractSaveModel (const AbstractModel& p_mdl) : AbstractModel (p_mdl) { }

AbstractSaveModel::~AbstractSaveModel() { }

Storage::Storage() { }

bool Storage::operator== (const Storage& p_store) const
{
    return type() == p_store.type();
}

const QString Cache::obtainFullSuffix (const QString& p_lcl, const QString& p_sfx)
{
    foreach (Storage * str, Cache::s_stores) {
        const QString fl = str->obtainFullSuffix (p_lcl, p_sfx);

        if (!fl.isEmpty ())
            return fl;
        else continue;
    }

    return QString::null;
}

/// @todo Consider allowing the developer to specify where they'd like to save information.
void Cache::write (const Data& p_dt)
{
    if (!Cache::s_stores.empty()) {
        foreach (Storage * str, Cache::s_stores)
        str->saveFrom (p_dt);
    }
    else {
        DomStorage* domStr = new DomStorage;
        domStr->saveFrom (p_dt);
        delete domStr;
    }
}

/// @note Wintermute considers its local data to be stored to disk. Currently, DomStorage represents the local nodes.
int Cache::countFlags()
{
    return DomStorage::countFlags();
}

/// @note Wintermute considers its local data to be stored to disk. Currently, DomStorage represents the local nodes.
int Cache::countSymbols()
{
    return DomStorage::countSymbols();
}

/// @todo Find a way to call all of the storages in parallel and then kill all of the other ones when none (or one has) found information.
bool Cache::exists (const Data& p_dt)
{
    if (p_dt.id() == "d41d8cd98f00b204e9800998ecf8427e")
        qDebug() << "(data) [Cache] Suspicious data" << p_dt;

    foreach (Storage * str, Cache::s_stores) {
        if (str->exists (p_dt))
            return true;
    }
    return false;
}

/// @todo Find a way to call all of the storages in parallel and then kill all of the other ones when none (or one has) found information.
bool Cache::read (Data& p_dt)
{
    foreach (Storage * str, Cache::s_stores) {
        if (str->exists (p_dt)) {
            str->loadTo (p_dt);
            return true;
        }
        else continue;
    }
    return false;
}

/// @todo Find a way to call all of the storages in parallel and then kill all of the other ones when none (or one has) found information.
void Cache::pseudo (Data& p_psDt)
{
    foreach (Storage * str, Cache::s_stores) {
        if (str->hasPseudo (p_psDt)) {
            str->loadPseudo (p_psDt);
            return;
        }
        else continue;
    }
}

/// @todo Find a way to call all of the storages in parallel and then kill all of the other ones when none (or one has) found information.
bool Cache::isPseudo (const Data& p_dt)
{
    foreach (Storage * str, Cache::s_stores) {
        if (str->hasPseudo (p_dt)) {
            Data dt;
            str->loadPseudo (dt);
            return (dt.id () == p_dt.id () && dt.flags () == p_dt.flags ());
        }
        else continue;
    }

    return false;
}

/// @todo Allow this to be configurable (adding to plug-in settings). Default would be 'DomStorage'.
Storage* Cache::addStorage (Storage* p_str)
{
    if (!hasStorage (p_str->type ())) {
        s_stores << p_str;
        qDebug() << "(data) [Cache] Added lexical cache backend" << p_str->type() << ".";
    }

    return p_str;
}

/// @todo Find a way to call all of the storages in parallel and then kill all of the other ones when none (or one has) found information.
void Cache::clearStorage()
{
    foreach (Storage * str, s_stores)
    delete str;

    s_stores.clear ();
}

/// @todo Find a way to call all of the storages in parallel and then kill all of the other ones when none (or one has) found information.
bool Cache::hasStorage (const QString& p_strName)
{
    foreach (Storage * str, s_stores) {
        if (str->type () == p_strName)
            return true;
    }

    return false;
}

/// @todo Find a way to call all of the storages in parallel and then kill all of the other ones when none (or one has) found information.
Storage* Cache::storage (const QString& p_strName)
{
    foreach (Storage * str, s_stores) {
        if (str->type () == p_strName)
            return str;
    }

    return 0;
}

const QStringList Cache::allNodes (const QString& p_lcl)
{
    QDir dir (Wintermute::Data::Linguistics::System::directory () + "/" + p_lcl + "/node");
    dir.setFilter (QDir::Files);
    dir.setNameFilters (QString ("*.node").split (","));
    return dir.entryList ().replaceInStrings (".node", QString::null);
}

/// @todo Find a way to call all of the storages in parallel and then kill all of the other ones when none (or one has) found information.
void Cache::generate()
{
    qDebug() << "(data) [Cache] Dumping all data storages...";

    foreach (Storage * str, Cache::s_stores) {
        qDebug() << "(data) [Cache] Dumping" << str->type ();
        str->generate();
    }

    qDebug() << "(data) [Cache] Dumped data.";
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
