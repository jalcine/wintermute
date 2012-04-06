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

#include <QDebug>

// local includes
#include "bond.hpp"
#include "chain.hpp"
#include "model.hpp"
#include "dom.hpp"

using namespace Wintermute::Data::Linguistics::Syntax;

Cache::StorageList Cache::s_stores;

AbstractModel::AbstractModel() { }

AbstractModel::AbstractModel ( const AbstractModel &p_mod ) : m_chn ( p_mod.m_chn ) { }

AbstractLoadModel::AbstractLoadModel() : AbstractModel() { }

AbstractLoadModel::AbstractLoadModel ( const AbstractModel &p_mdl ) : AbstractModel ( p_mdl ) { }

AbstractLoadModel::~AbstractLoadModel() { }

AbstractSaveModel::AbstractSaveModel() : AbstractModel() { }

AbstractSaveModel::AbstractSaveModel ( const AbstractModel &p_mdl ) : AbstractModel ( p_mdl ) { }

AbstractSaveModel::~AbstractSaveModel() { }

AbstractStorage::AbstractStorage() { }

AbstractStorage::AbstractStorage ( const QString& p_lcl, const QString& p_flg ) : m_flg ( p_flg ), m_lcl ( p_lcl ) { }

AbstractStorage::AbstractStorage ( const AbstractStorage &p_str ) : m_flg ( p_str.m_flg ), m_lcl ( p_str.m_lcl ) { }

bool AbstractStorage::operator == ( const AbstractStorage& p_str ) {
    return type() == this->type ();
}

const QString AbstractStorage::flag () const {
    return m_flg;
}

const QString AbstractStorage::locale() const {
    return m_lcl;
}

AbstractStorage::~AbstractStorage () { }

void AbstractModel::setChain ( const Chain &p_chn ) {
    m_chn = p_chn;
}

const Chain AbstractModel::chain () const {
    return m_chn;
}

AbstractModel::~AbstractModel () { }

const bool Cache::read ( Chain &p_chn ) {
    foreach ( AbstractStorage* str, Cache::s_stores ) {
        if ( str->exists ( p_chn.locale (),p_chn.type () ) ) {
            str->loadTo ( p_chn );
            return true;
        } else continue;
    }

    return false;
}

void Cache::write ( const Chain& p_chn ) {
    AbstractStorage* fdStr;
    foreach ( AbstractStorage* str, Cache::s_stores ) {
        if ( str->exists ( p_chn.locale (),p_chn.type () ) )
            fdStr = str;
        else continue;
    }

    if ( fdStr )
        fdStr->saveFrom ( p_chn );
    else {
        // save this locally. We consider the DOM AbstractStorage to be local.
        fdStr = new DomStorage;
        fdStr->saveFrom ( p_chn );
    }

}

const bool Cache::exists ( const QString& p_lcl, const QString& p_flg ) {
    foreach ( AbstractStorage* str, Cache::s_stores ) {
        if ( str->exists ( p_lcl,p_flg ) )
            return true;
    }

    return false;
}

AbstractStorage* Cache::addStorage ( AbstractStorage *p_str ) {
    if ( !hasStorage ( p_str->type () ) ) {
        s_stores << p_str;
        qDebug() << "(data) [Cache] Added rules cache backend" << p_str->type() << ".";
    }

    return p_str;
}

void Cache::clearStorage() {
    foreach ( AbstractStorage* str, s_stores )
    delete str;

    s_stores.clear ();
}


const bool Cache::hasStorage ( const QString& p_strName ) {
    foreach ( AbstractStorage* str, s_stores ) {
        if ( str->type () == p_strName )
            return true;
    }

    return false;
}

AbstractStorage* Cache::storage ( const QString& p_strName ) {
    foreach ( AbstractStorage* str, s_stores ) {
        if ( str->type () == p_strName )
            return str;
    }

    return 0;
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
