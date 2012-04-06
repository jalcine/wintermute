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

#ifndef WNTRDATA_SYNTAX_MODEL_HPP
#define WNTRDATA_SYNTAX_MODEL_HPP

// Qt includes
#include <QObject>

// local includes
#include "dbus/syntaxadaptor.hpp"
#include "syntax/bond.hpp"
#include "syntax/chain.hpp"

namespace Wintermute {
namespace Data {
namespace Linguistics {
namespace Syntax {

/**
 * @brief
 * @class Model models.hpp "src/models.hpp"
 */
class AbstractModel : public QObject {
    Q_OBJECT
    Q_PROPERTY ( Chain Chain READ chain WRITE setChain )

protected:
    Chain m_chn; /**< Represents the information used to form a binding. */
    /**
     * @brief Constructor.
     * @fn Model
     * @param
     */
    explicit AbstractModel ( const Bond& );

public:
    /**
     * @brief Null constructor.
     * @fn Model
     */
    AbstractModel();

    /**
     * @brief Copy constructor.
     * @fn Model
     * @param p_mdl The Model to be copied.
     */
    AbstractModel ( const AbstractModel& );

    /**
     * @brief Deconstructor.
     * @fn ~Model
     */
    virtual ~AbstractModel();

    /**
     * @brief Obtains the Chain representing this Model.
     * @fn chain The stored Chain.
     */
    const Chain chain() const;

    /**
     * @brief Changes the internal Chain to a different Chain.
     * @fn setChain
     * @param p_chn The Chain to be saved.
     */
    void setChain ( const Chain& );
};

/**
 * @brief
 * @class SaveModel models.hpp "src/models.hpp"
 */
class AbstractSaveModel : public AbstractModel {
    Q_OBJECT

signals:
    /**
     * @brief
     * @fn saved
     */
    void saved();

public:
    /**
     * @brief
     * @fn save
     */
    virtual void save() = 0;
    /**
     * @brief
     *
     * @fn saveFrom
     * @param
     */
    virtual void saveFrom ( const Chain& ) = 0;

protected:
    /**
     * @brief
     * @fn SaveModel
     */
    AbstractSaveModel();
    /**
     * @brief
     * @fn SaveModel
     * @param p_model
     */
    AbstractSaveModel ( const AbstractModel& );
    /**
     * @brief
     * @fn ~SaveModel
     */
    virtual ~AbstractSaveModel() = 0;

    /**
     * @brief
     *
     * @fn obtainType
     */
    virtual void setType ( const QString& ) = 0;

    /**
     * @brief
     *
     * @fn obtainBonds
     */
    virtual void setBonds ( const BondList& ) = 0;
};

/**
 * @brief
 * @class LoadModel models.hpp "src/models.hpp"
 */
class AbstractLoadModel : public AbstractModel {
    Q_OBJECT

signals:
    /**
     * @brief
     * @fn loaded
     */
    void loaded();

public:
    /**
     * @brief
     * @fn load
     */
    virtual const Chain* load() const = 0;
    /**
     * @brief
     *
     * @fn loadTo
     * @param
     */
    virtual void loadTo ( Chain& ) const = 0;

protected:
    /**
     * @brief
     * @fn LoadModel
     */
    AbstractLoadModel();
    /**
     * @brief
     * @fn LoadModel
     * @param p_model
     */
    AbstractLoadModel ( const AbstractModel& );
    /**
     * @brief
     *
     * @fn ~LoadModel
     */
    virtual ~AbstractLoadModel() = 0;
};

class AbstractBackend { };

/**
 * @brief
 * @class Storage models.hpp "src/models.hpp"
 */
class AbstractStorage : public virtual AbstractBackend {
public:
    /**
     * @brief
     *
     * @fn Storage
     */
    AbstractStorage();
    /**
     * @brief
     *
     * @fn Storage
     * @param
     */
    AbstractStorage ( const AbstractStorage& );
    /**
     * @brief
     *
     * @fn Storage
     * @param
     */
    AbstractStorage ( const AbstractBackend& );
    /**
     * @brief
     *
     * @fn Storage
     * @param
     * @param
     */
    AbstractStorage ( const QString&, const QString& );
    /**
     * @brief
     *
     * @fn operator ==
     * @param
     */
    bool operator== ( const AbstractStorage& );
    /**
     * @brief
     *
     * @fn loadTo
     * @param
     */
    virtual void loadTo ( Chain& ) const = 0;
    /**
     * @brief
     *
     * @fn saveFrom
     * @param
     */
    virtual void saveFrom ( const Chain& ) = 0;
    /**
     * @brief
     *
     * @fn exists
     * @param
     */
    virtual const bool exists ( const QString, const QString ) const = 0;
    /**
     * @brief
     *
     * @fn type
     */
    virtual const QString type() const = 0;
    /**
     * @brief
     *
     * @fn flag
     */
    virtual const QString flag() const;
    /**
     * @brief
     *
     * @fn locale
     */
    virtual const QString locale() const;
    /**
     * @brief
     *
     * @fn ~Storage
     */
    virtual ~AbstractStorage();
private:
    QString m_flg;
    QString m_lcl;
};

/**
 * @brief
 *
 * @class Cache models.hpp "src/models.hpp"
 */
class Cache {
    /**
     * @brief
     *
     * @typedef StorageList
     */
    typedef QList<AbstractStorage*> StorageList;
    friend class Wintermute::Data::SyntaxAdaptor;

private:
    Cache();
    static StorageList s_stores; /**< Holds the storage. */
    /**
     * @brief
     *
     * @fn addStorage
     * @param
     */

public:
    static AbstractStorage* addStorage ( AbstractStorage* );
    /**
     * @brief
     *
     * @fn storage
     * @param
     */
    static AbstractStorage* storage ( const QString& );
    /**
     * @brief
     *
     * @fn hasStorage
     * @param
     */
    static const bool hasStorage ( const QString& );
    /**
     * @brief
     *
     * @fn clearStorage
     */
    static void clearStorage();

    /**
     * @brief
     *
     * @fn write
     * @param
     */
    static void write ( const Chain& );
    /**
     * @brief
     *
     * @fn exists
     * @param
     */
    static const bool exists ( const QString&, const QString& );
    /**
     * @brief
     *
     * @fn read
     * @param
     */
    static const bool read ( Chain& );
};

}
}
}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
