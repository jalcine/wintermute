/*
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

/**
 * @file dom.hpp
 * @author Jacky Alcine <jacky.alcine@thesii.org>
 * @date Sun, 30 Oct 2011 21:54:16
 */
#ifndef WNTRDATA_LEXICAL_DOM_HPP
#define WNTRDATA_LEXICAL_DOM_HPP

#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>
#include <data-api/lexical/model.hpp>

namespace Wintermute
{
namespace Data
{
namespace Linguistics
{
namespace Lexical
{

class Data;
class DomLoadModel;
class DomSaveModel;
class DomStorage;
class DomBackend;

class DomBackend : public AbstractBackend
{
public:
    DomBackend (const QDomElement& p_ele);

protected:
    QDomElement m_ele; /**< Represents the Dom data of the object. */
};

class DomLoadModel : public AbstractLoadModel, public DomBackend
{
    Q_OBJECT
public:
    DomLoadModel();
    DomLoadModel (QDomElement& p_elem);
    DomLoadModel (const DomLoadModel& p_other);
    virtual ~DomLoadModel();
    virtual Data load() const;
    virtual bool loadTo (Data& p_data) const;

};

class DomSaveModel : public AbstractSaveModel, public DomBackend
{
    Q_OBJECT
public:
    DomSaveModel();
    DomSaveModel (QDomElement& p_elem);
    DomSaveModel (const DomSaveModel& p_other);
    virtual ~DomSaveModel();
    virtual void save();
    virtual void saveFrom (const Data& p_data);
};

class DomStorage : public Storage
{
    friend class DomLoadModel;
    friend class DomSaveModel;

private:
    static const QString getPath (const Data& p_data);
    static QDomDocument getSpawnDoc (const Data& p_data);
    static void spawn (const QDomDocument& p_dom);

public:
    DomStorage();
    DomStorage (const DomStorage& p_other);
    virtual ~DomStorage();
    virtual void generate ();
    virtual bool exists (const Data& p_data) const;
    virtual bool loadTo (Lexical::Data& p_data) const;
    virtual bool saveFrom (const Data& p_data);
    virtual bool hasPseudo (const Data& p_data) const;
    virtual bool loadPseudo (Lexical::Data& p_data) const;
    virtual QString type () const;
    virtual QString obtainFullSuffix (const QString& p_locale, const QString& p_suffix) const;
    static int countFlags();
    static int countSymbols();
};

}
}
}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
