/**
 * @file model.hpp
 * @author Jacky Alcine <jacky.alcine@thesii.org>
 * @date Sun, 30 Oct 2011 21:54:16
 */
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

#ifndef WNTRDATA_LEXICAL_MODEL_HPP
#define WNTRDATA_LEXICAL_MODEL_HPP

#include <QString>
#include <QObject>

#include <data-api/lexical/data.hpp>

namespace Wintermute
{
namespace Data
{
class NodeAdaptor;
namespace Linguistics
{
class System;
namespace Lexical
{

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
class AbstractModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY (Data data READ data WRITE setData)

protected:
    mutable Data m_dt; /**< The variable holding the internal Data. */

public:
    virtual ~AbstractModel();
    AbstractModel();

    /**
     * @brief Constructor, using lexical data.
     * @fn Model
     * @param p_info The data to fill itself with.
     */
    AbstractModel (Data& p_data);

    /**
     * @brief Copy constructor.
     * @fn Model
     * @param p_mdl The Model to be copied.
     */
    AbstractModel (const AbstractModel& p_other);

    /**
     * @brief Obtains the data stored in this Model.
     * @fn getLexicalMap
     * @return Data
     */
    Data data() const;

    /**
     * @brief Changes the internal Data to p_dt.
     * @fn setData
     * @param p_dt The Data to be used, or typically Data::Null.
     */
    void setData (const Data& p_data = Data::Empty);
};

/**
 * @brief Represents a model for saving data to the lexical information
 * storage.
 *
 * The SaveModel class is typically derived for the simplest of
 * saving information to whatever it's dervied source of lexical
 * information may be. Typically, you'd use this class if you
 * don't know where your information is being saved to; but the
 * way that the system is built; you'll never need to programatically
 * use this base.
 *
 * @see DomSaveModel
 * @class SaveModel models.hpp "src/models.hpp"
 */
class AbstractSaveModel : public AbstractModel
{
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
    AbstractSaveModel (Data& p_data);

    /**
     * @brief Base copy constructor.
     * @fn SaveModel
     * @param p_mod The Model to be copied.
     */
    AbstractSaveModel (const AbstractModel& p_other);

    /**
     * @brief Copy constructor.
     * @fn SaveModel
     * @param p_smod The SaveModel to be copied.
     */
    AbstractSaveModel (const AbstractSaveModel& p_other);

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
    virtual void saveFrom (const Data& p_data) = 0;

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
 * The LoadModel class is typically derived for the simplest of
 * loading information to whatever it's dervied source of lexical
 * information may be. Typically, you'd use this class if you
 * don't know where your information is being loaded from; but the
 * way that the system is built; you'll never need to programatically
 * use this base.
 *
 * @class LoadModel models.hpp "src/models.hpp"
 */
class AbstractLoadModel : public AbstractModel
{
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
    AbstractLoadModel (const AbstractLoadModel& p_other);

    /**
     * @brief Base copy constructor.
     * @fn LoadModel
     * @param p_mdl The Model to be copied.
     */
    AbstractLoadModel (const AbstractModel& p_other);

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
    virtual Data load() const = 0;

    /**
     * @brief Loads the lexical information storage to p_dt.
     * @fn loadTo
     * @param p_dt The Data to load the information to.
     */
    virtual bool loadTo (Data& p_data) const = 0;

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
struct AbstractBackend { };

/**
 * @brief Represents the foundational front-end means of loading
 *        and saving lexical information from any storage.
 *
 * The Storage class is the <b>recommended</b> class to use for
 *
 * @class Storage models.hpp "src/models.hpp"
 */
class Storage : public virtual AbstractBackend
{
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
    Storage (const Storage& p_other);

    /**
     * @brief Builds a @see Storage object from a @see Backend
     * @fn Storage
     * @param
     */
    explicit Storage (const AbstractBackend& p_other);

    /**
     * @brief Equality operator.
     * @fn operator ==
     * @param
     */
    bool operator== (const Storage& p_other) const;

    /**
     * @brief Reports the kind of data storage this @see Storage use.
     * @fn type
     * @return A @see QString delinating the kind of data storage uses.
     */
    virtual QString type() const = 0;

    /**
     * @brief Determines if a specific @see Data is available.
     * @fn exists
     * @param @see Data The @see Data in question.
     */
    virtual bool exists (const Data& p_data) const = 0;

    /**
     * @brief
     * @fn loadTo
     * @note This method <b>edits</b> the Data passed to this method.
     * @param
     */
    virtual bool loadTo (Data& p_data) const = 0;

    virtual bool saveFrom (const Data& p_data) = 0;
    virtual bool hasPseudo (const Data& p_data) const = 0;
    virtual bool loadPseudo (Data& p_data) const = 0;
    virtual void generate() = 0;
    virtual QString obtainFullSuffix (const QString&, const QString&) const = 0;
};

class Cache
{
    friend class Storage;
    friend class Wintermute::Data::NodeAdaptor;
    friend class Wintermute::Data::Linguistics::System;
    typedef QList<Storage*> StorageList;

private:
    static StorageList s_stores; /** Represents a listing of all of the Storages for the Lexical system. */

    static Storage* addStorage (Storage*);
    static void clearStorage();
    static bool hasStorage (const QString&);
    static Storage* storage (const QString&);

public:
    ~Cache();
    static void write (const Data& p_data);
    static void generate();
    static void pseudo (Data& p_data);
    static bool read (Data& p_data);
    static bool exists (const Data& p_data);
    static bool isPseudo (const Data& p_data);
    static int countFlags();
    static int countSymbols();
    static const QStringList allNodes (const QString&);
    static const QString obtainFullSuffix (const QString&, const QString&);
};

}
}
}
}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
