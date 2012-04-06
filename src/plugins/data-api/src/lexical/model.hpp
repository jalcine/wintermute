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

#ifndef WNTRDATA_LEXICAL_MODEL_HPP
#define WNTRDATA_LEXICAL_MODEL_HPP

// Qt includes
#include <QString>
#include <QObject>

// local includes
#include "data.hpp"

namespace Wintermute {
namespace Data {
class NodeAdaptor;
namespace Linguistics {
class System;
namespace Lexical {

class AbstractModel;
class AbstractSaveModel;
class AbstractLoadModel;
class Storage;
class AbstractBackend;
class Cache;

/**
 * @brief The most basic model for data obtaining for lexical information
 * storage.
 *
 * The Model class provides the most simplest means of loading and
 * saving lexical information for Wintermute. This class merely serves
 * as a base of the more defined classes (LoadModel and SaveModel, typically
 * used instead of this) for storage purposes.
 *
 * @see LoadModel, SaveModel
 * @class Model models.hpp "src/models.hpp"
 */
class AbstractModel : public QObject {
    Q_OBJECT
    Q_PROPERTY ( Data data READ data WRITE setData )

protected:
    mutable Data m_dt; /**< The variable holding the internal Data. */

public:
    /**
     * @brief Default constructor.
     * @fn ~Model
     */
    virtual ~AbstractModel();

    /**
     * @brief Null constructor.
     * @fn Model
     */
    AbstractModel();

    /**
     * @brief Constructor, using lexical data.
     * @fn Model
     * @param p_info The data to fill itself with.
     */
    AbstractModel ( Data& );

    /**
     * @brief Copy constructor.
     * @fn Model
     * @param p_mdl The Model to be copied.
     */
    AbstractModel ( const AbstractModel& );

    /**
     * @brief Obtains the data stored in this Model.
     * @fn getLexicalMap
     * @return Data
     */
    const Data& data();

    /**
     * @brief Changes the internal Data to p_dt.
     * @fn setData
     * @param p_dt The Data to be used, or typically Data::Null.
     */
    void setData ( const Data& = Data::Empty );
};

/**
 * @brief Represents a model for saving data to the lexical information
 * storage.
 *
 * The SaveModel class is typically dervived for the simplest of
 * saving information to whatever it's dervied source of lexical
 * information may be. Typically, you'd use this class if you
 * don't know where your information is being saved to; but the
 * way that the system is built; you'll never need to programatically
 * use this base.
 *
 * @see DomSaveModel
 * @class SaveModel models.hpp "src/models.hpp"
 */
class AbstractSaveModel : public AbstractModel {
    Q_OBJECT
protected:
    /**
     * @brief Default constructor.
     * @fn SaveModel
     */
    AbstractSaveModel();

    /**
     * @brief Constructor.
     * @fn SaveModel
     * @param p_lxin The Data to be saved.
     */
    AbstractSaveModel ( Data& );

    /**
     * @brief Base copy constructor.
     * @fn SaveModel
     * @param p_mod The Model to be copied.
     */
    AbstractSaveModel ( const AbstractModel& );

    /**
     * @brief Copy constructor.
     * @fn SaveModel
     * @param p_smod The SaveModel to be copied.
     */
    AbstractSaveModel ( const AbstractSaveModel& );

    /**
     * @brief Deconstructor.
     * @fn ~SaveModel
     */
    ~AbstractSaveModel();

public:
    /**
     * @brief Saves the data in this SaveModel to the specified
     *        lexical information storage.
     * @fn save
     */
    virtual void save() = 0;

    /**
     * @brief Saves p_dt to the specified lexical information
     *        storage.
     * @fn saveFrom
     * @param p_dt The Data to be saved.
     */
    virtual void saveFrom ( const Data& ) = 0;

signals:

    /**
     * @brief Emitted when a save operation has been completed
     *        successfully. Typically emitted if the operation
     *        is asynchronous.
     * @fn saved
     */
    void saved();
};

/**
 * @brief Represents a model for saving data to the lexical information
 * storage.
 *
 * The LoadModel class is typically dervived for the simplest of
 * loading information to whatever it's dervied source of lexical
 * information may be. Typically, you'd use this class if you
 * don't know where your information is being loaded from; but the
 * way that the system is built; you'll never need to programatically
 * use this base.
 *
 * @class LoadModel models.hpp "src/models.hpp"
 */
class AbstractLoadModel : public AbstractModel {
    Q_OBJECT

protected:
    /**
     * @brief Null constructor.
     * @fn LoadModel
     */
    AbstractLoadModel();

    /**
     * @brief Copy constructor.
     * @fn LoadModel
     * @param p_mdl The LoadModel to be copied.
     */
    AbstractLoadModel ( const AbstractLoadModel& );

    /**
     * @brief Base copy constructor.
     * @fn LoadModel
     * @param p_mdl The Model to be copied.
     */
    AbstractLoadModel ( const AbstractModel& );

    /**
     * @brief Deconstructor.
     * @fn ~LoadModel
     */
    ~AbstractLoadModel();

public:

    /**
     * @brief Loads the Data from its specified lexical
     *        information storage.
     * @fn load
     * @return The Data obtained, or Data::Null.
     */
    virtual const Data load( ) const = 0;

    /**
     * @brief Loads the lexical information storage to p_dt.
     * @fn loadTo
     * @param p_dt The Data to load the information to.
     */
    virtual bool loadTo ( Data& ) const = 0;

signals:
    /**
     * @brief Emitted when the load operation behind this
              LoadModel has completed. Typically, this is used
              for asynchronous operations.
     * @fn loaded
     */
    void loaded() const;
};

/**
 * @brief Represents the infrastructual backend of all storage classes.
 * @class Backend models.hpp "src/models.hpp"
 */
class AbstractBackend { };

/**
 * @brief Represents the foundational front-end means of loading
 *        and saving lexical information from any storage.
 *
 * The Storage class is the <b>recommended</b> class to use for
 *
 * @class Storage models.hpp "src/models.hpp"
 */
class Storage : public virtual AbstractBackend {
public:
    /**
     * @brief Null constructor.
     * @fn Storage
     */
    Storage();

    /**
     * @brief Copy constructor.
     * @fn Storage
     * @param Storage The object to be copied.
     */
    Storage ( const Storage& );

    /**
     * @brief Builds a @see Storage object from a @see Backend
     * @fn Storage
     * @param
     */
    explicit Storage ( const AbstractBackend& );

    /**
     * @brief Equality operator.
     * @fn operator ==
     * @param
     */
    bool operator== ( const Storage& ) const;

    /**
     * @brief Reports the kind of data storage this @see Storage use.
     * @fn type
     * @return A @see QString delinating the kind of data storage uses.
     */
    virtual const QString type() const = 0;

    /**
     * @brief Determines if a specific @see Data is available.
     * @fn exists
     * @param @see Data The @see Data in question.
     */
    virtual const bool exists ( const Data& ) const = 0;

    /**
     * @brief
     * @fn loadTo
     * @note This method <b>edits</b> the Data passed to this method.
     * @param
     */
    virtual void loadTo ( Data& ) const = 0;

    /**
     * @brief
     *
     * @fn saveFrom
     * @param
     */
    virtual void saveFrom ( const Data& ) = 0;
    /**
     * @brief
     *
     * @fn generate
     */
    virtual void generate() = 0;
    /**
     * @brief
     *
     * @fn hasPseudo
     * @param
     */
    virtual const bool hasPseudo ( const Data& ) const = 0;
    /**
     * @brief
     *
     * @fn loadPseudo
     * @param
     */
    virtual void loadPseudo ( Data& ) const = 0;

    /**
     * @brief
     *
     * @fn obtainFullSuffix
     * @param
     * @param
     */
    virtual const QString obtainFullSuffix ( const QString&, const QString& ) const = 0;
};

/**
 * @brief
 *
 * @class Cache models.hpp "src/models.hpp"
 */
class Cache {
    friend class Storage;
    friend class Wintermute::Data::NodeAdaptor;
    friend class Wintermute::Data::Linguistics::System;
    typedef QList<Storage*> StorageList;

private:
    static StorageList s_stores; /** Represents a listing of all of the Storages for the Lexical system. */

    /**
     * @brief
     *
     * @fn addStorage
     * @param
     */
    static Storage* addStorage ( Storage* );
    /**
     * @brief
     *
     * @fn clearStorage
     */
    static void clearStorage();
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
     * @fn storage
     * @param
     */
    static Storage* storage ( const QString& );

public:
    ~Cache();
    /**
     * @brief
     *
     * @fn read
     * @param
     */
    static const bool read ( Data & );
    /**
     * @brief
     *
     * @fn write
     * @param
     */
    static void write ( const Data & );
    /**
     * @brief
     *
     * @fn exists
     * @param
     */
    static const bool exists ( const Data& );
    /**
     * @brief
     *
     * @fn pseudo
     * @param
     */
    static void pseudo ( Data & );
    /**
     * @brief
     *
     * @fn isPseudo
     * @param
     */
    static const bool isPseudo ( const Data & );
    /**
     * @brief
     *
     * @fn generate
     */
    static void generate();

    /**
     * @brief
     *
     * @fn countFlags
     */
    static const int countFlags();

    /**
     * @brief
     *
     * @fn countSymbols
     */
    static const int countSymbols();

    /**
     * @brief
     *
     * @fn allNodes
     * @param
     */
    static const QStringList allNodes ( const QString& );

    /**
     * @brief
     *
     * @fn obtainFullSuffix
     * @param
     * @param
     */
    static const QString obtainFullSuffix ( const QString&, const QString& );
};

}
}
}
}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
