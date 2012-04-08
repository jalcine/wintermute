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

#ifndef WNTRDATA_LEXICAL_DOM_HPP
#define WNTRDATA_LEXICAL_DOM_HPP

// local includes
#include "model.hpp"

class QDomElement;
class QDomDocument;

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

/**
 * @brief
 * @class DomBackend models.hpp "src/models.hpp"
 */
class DomBackend : public AbstractBackend
{
public:
    /**
     * @brief
     *
     * @fn DomBackend
     */
    DomBackend();
    /**
     * @brief
     *
     * @fn DomBackend
     * @param
     */
    DomBackend (QDomElement*);

protected:
    mutable QDomElement* m_ele; /**< Represents the Dom data of the object. */
};

/**
 * @brief
 *
 * @class DomLoadModel models.hpp "src/models.hpp"
 */
class DomLoadModel : public AbstractLoadModel, public DomBackend
{
public:
    DomLoadModel();
    DomLoadModel (QDomElement*);
    DomLoadModel (const DomLoadModel&);
    virtual ~DomLoadModel();
    virtual const Data load() const;
    virtual bool loadTo (Data&) const;

};

/**
 * @brief
 *
 * @class DomSaveModel models.hpp "src/models.hpp"
 */
class DomSaveModel : public AbstractSaveModel, public DomBackend
{
public:
    /**
     * @brief
     *
     * @fn DomSaveModel
     */
    DomSaveModel();
    /**
     * @brief
     *
     * @fn DomSaveModel
     * @param
     */
    DomSaveModel (QDomElement*);
    /**
     * @brief
     *
     * @fn DomSaveModel
     * @param
     */
    DomSaveModel (const DomSaveModel&);
    /**
     * @brief
     *
     * @fn ~DomSaveModel
     */
    virtual ~DomSaveModel();
    /**
     * @brief
     *
     * @fn save
     * @param
     */
    virtual void save();
    /**
     * @brief
     *
     * @fn saveFrom
     * @param
     */
    virtual void saveFrom (const Data&);
};

/**
 * @brief
 *
 * @class DomStorage models.hpp "src/models.hpp"
 */
class DomStorage : public Storage
{
    friend class DomLoadModel;
    friend class DomSaveModel;

private:
    /**
     * @brief
     *
     * @fn getPath
     * @param
     */
    static const QString getPath (const Data&);

    /**
     * @brief
     *
     * @fn spawnDoc
     * @param
     */
    static QDomDocument* getSpawnDoc (const Data&);
    /**
     * @brief
     *
     * @fn spawn
     * @param
     */
    static void spawn (const QDomDocument&);

public:
    /**
     * @brief
     *
     * @fn DomStorage
     */
    DomStorage();
    /**
     * @brief
     *
     * @fn DomStorage
     * @param
     */
    DomStorage (const DomStorage&);
    /**
     * @brief
     *
     * @fn ~DomStorage
     */
    virtual ~DomStorage();
    /**
     * @brief
     *
     * @fn exists
     * @param
     */
    virtual bool exists (const Wintermute::Data::Linguistics::Lexical::Data& p_dt) const;
    /**
     * @brief
     *
     * @fn type
     */
    virtual const QString type () const;
    /**
     * @brief
     *
     * @fn loadTo
     * @param
     */
    virtual void loadTo (Data&) const;
    /**
     * @brief
     *
     * @fn saveFrom
     * @param
     */
    virtual void saveFrom (const Data&);
    /**
     * @brief
     *
     * @fn generate
     */
    virtual void generate ();
    /**
     * @brief
     *
     * @fn hasPseudo
     * @param
     */
    virtual bool hasPseudo (const Wintermute::Data::Linguistics::Lexical::Data& p_dt) const;
    /**
     * @brief
     *
     * @fn loadPseduo
     * @param
     */
    virtual void loadPseudo (Data&) const;

    /**
     * @brief
     *
     * @fn obtainFullSuffix
     * @param
     * @param
     */
    virtual const QString obtainFullSuffix (const QString&, const QString&) const;

    /**
     * @brief
     *
     * @fn countFlags
     */
    static int countFlags();

    /**
     * @brief
     *
     * @fn countSymbols
     */
    static int countSymbols();
};

}
}
}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
