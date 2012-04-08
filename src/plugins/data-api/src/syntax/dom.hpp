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

#ifndef WNTRDATA_SYNTAX_DOM_HPP
#define WNTRDATA_SYNTAX_DOM_HPP

// Qt includes
#include <QDomElement>
#include <QDomDocument>

// local includes
#include "config.hpp"
#include "bond.hpp"
#include "model.hpp"

namespace Wintermute
{
namespace Data
{
namespace Linguistics
{
namespace Syntax
{

class DomBackend;
class DomSaveModel;
class DomLoadModel;
class DomStorage;

/**
 * @brief
 *
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
    DomBackend (const DomBackend&);
    /**
     * @brief
     *
     * @fn DomBackend
     * @param
     */
    explicit DomBackend (QDomElement*);
    /**
     * @brief
     *
     * @fn ~DomBackend
     */
    ~DomBackend();

protected:
    QDomElement* m_elem;
};

/**
 * @brief
 *
 * @class DomLoadModel models.hpp "src/models.hpp"
 */
class DomLoadModel : public AbstractLoadModel, public DomBackend
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @fn DomLoadModel
     */
    DomLoadModel();
    /**
     * @brief
     *
     * @fn DomLoadModel
     * @param
     */
    DomLoadModel (const DomLoadModel&);
    /**
     * @brief
     *
     * @fn DomLoadModel
     * @param
     */
    DomLoadModel (QDomElement*);
    /**
     * @brief
     *
     * @fn ~DomLoadModel
     */
    virtual ~DomLoadModel();
    /**
     * @brief
     *
     * @fn load
     */
    virtual const Chain* load () const;
    /**
     * @brief
     *
     * @fn loadTo
     * @param
     */
    virtual void loadTo (Chain&) const;
private:
    /**
     * @brief
     *
     * @fn obtainType
     * @param QDomElement
     */
    virtual const QString obtainType (const QDomElement*) const;
    /**
     * @brief
     *
     * @fn obtainBonds
     * @param
     * @param QDomElement
     */
    virtual void obtainBonds (BondList*, const QDomElement*) const;
};

/**
 * @brief
 *
 * @class DomSaveModel models.hpp "src/models.hpp"
 */
class DomSaveModel : public AbstractSaveModel, public DomBackend
{
    Q_OBJECT
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
    DomSaveModel (const DomSaveModel&);
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
     * @fn ~DomSaveModel
     */
    virtual ~DomSaveModel();
    /**
     * @brief
     *
     * @fn save
     */
    virtual void save ();
    /**
     * @brief
     *
     * @fn saveFrom
     * @param
     */
    virtual void saveFrom (const Chain&);

private:
    /**
     * @brief
     *
     * @fn obtainType
     * @param QDomElement
     */
    virtual void setType (const QString&);
    /**
     * @brief
     *
     * @fn obtainBonds
     * @param
     * @param QDomElement
     */
    virtual void setBonds (const BondList&);

};

/**
 * @brief
 *
 * @class DomStorage models.hpp "src/models.hpp"
 */
class DomStorage : public AbstractStorage
{
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
    DomStorage (const AbstractStorage&);
    /**
     * @brief
     *
     * @fn ~DomStorage
     */
    virtual ~DomStorage();
    /**
     * @brief
     *
     * @fn loadTo
     * @param
     */
    virtual void loadTo (Chain&) const;
    /**
     * @brief
     *
     * @fn saveFrom
     * @param
     */
    virtual void saveFrom (const Chain&);
    /**
     * @brief
     *
     * @fn exists
     * @param
     */
    virtual bool exists (const QString& p_flg, const QString& p_lcl) const;
    /**
     * @brief
     *
     * @fn type
     */
    virtual QString type () const;

private:
    mutable double m_min; /**< Represents the strength of matching. */
    /**
     * @brief
     *
     * @fn matches
     * @param
     * @param
     */
    static double matches (const QString&, const QString&);
    /**
     * @brief
     *
     * @fn findElement
     * @param
     * @param QDomElement
     */
    QDomElement findElement (const Chain&, QDomElement) const;
    /**
     * @brief
     *
     * @fn findElement
     * @param
     * @param QString
     */
    QDomElement findElement (const Chain&, QDomElement , QString) const;
    /**
     * @brief
     *
     * @fn getPath
     * @param
     */
    static const QString getPath (const Chain&);
    /**
     * @brief
     *
     * @fn loadDom
     * @param
     */
    static QDomDocument* loadDom (const Chain&);
};

}
}
}
}

#endif /* WNTRDATA_SYNTAX_DOM_HPP */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
