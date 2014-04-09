/**
 * @author Jacky Alciné <me@jalcine.me>
 * @copyright © 2011, 2012, 2013, 2014 Jacky Alciné <me@jalcine.me>
 * @if 0
 * Wintermute is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
 * @endif
 **/
/**
 * @file  Wintermute/Procedure/module.hpp <Wintermute/Procedure/Module>
 * @brief Definition of `Wintermute::Procedure::Module`.
 */

#ifndef WINTERMUTE_PROCEDURE_MODULE_HPP
#define WINTERMUTE_PROCEDURE_MODULE_HPP

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QHash>
#include <QtCore/QCoreApplication>
#include <Wintermute/Globals>
#include <Wintermute/Application>

namespace Wintermute
{
namespace Procedure
{
class Module;
class ModuleCall;
class MethodCall;
class ModulePrivate;

///< @brief Defines a pointer-controlled list.
typedef QList<QPointer<Module>> ModuleList;

/**
 * @brief Represents a collection of related actions.
 * @see   Wintermute::Procedure::ModuleCall
 * @see   Wintermute::Procedure::MethodCall
 *
 * Modules represent a hub of actions that can be invoked by Wintermute over
 * its own internal remote produce system.
 */
class Module : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY ( Module )

  public:

    ///< @brief Defines the identify of a module.
    struct Definition {
      quint64 pid     = 0; ///< @brief The PID at which a Module resides.
      QString package = QString::null; ///< The package where the Module resides.
      QString domain  = QString::null; ///< The domain where the Module resides.

      /**
       * @brief Creates a new Definition
       * @param[in] quint64 The PID to use.
       * @param[in] QString The package of the module.
       * @param[in] QString The domain of the module.
       */
      Definition(const quint64& pid = 0, const QString& package = QString::null,
                 const QString& domain = QString::null);

      /**
       * @brief Determines if this is a valid definition.
       * @retval boolean Whether or not this definition is valid.
       *
       * Checks if there's a value for the package and domain of the definition.
       * @note This will return true even if the PID is zero.
       */
      bool valid() const;

      /**
       * @brief Casts this Definition into a QString.
       * @retval A QString that can be used to represent this Definition.
       */
      operator QString() const;

      /**
       * @brief Equality operator.
       * @param[in] Definition definition.
       * @retval Determines if the provided Definition is equal to this
       *         Definition.
       */
      bool operator==(const Definition& definition) const;

      static const Definition Null; ///< @brief Helper field for null types.

      static const int MetaTypeId; ///< @internal

      /**
       * @brief Converts a JSON string into a Definition.
       * @param[in] QString The JSON data to be used.
       * @retval Definition The definition to be created.
       */
      static Definition fromString(const QString& string);

      /**
       * @brief Creates a new, valid Definition.
       * @param[in] QString The domain of the definition.
       * @param[in] QString The package of the definition.
       * @param[in] quint64 The PID of the definition.
       * @retval Definition The new Definition, when successful.
       * @retval Definition::Null A null, invalid Definition if invalid
       *         parameters are used.
       */
      static Definition compose(const QString& domain, const QString& package,
                                const quint64& pid = 0);
    };

    ///< @brief Represents a list of definitions.
    typedef QList<Definition> DefinitionList;

    ///< @brief Destroys this Module and its calls.
    virtual ~Module();

    /**
     * @brief Obtains the definition of this Module.
     * @retval Module::Definition Its definition.
     */
    Module::Definition definition() const;

    /**
     * @brief Obtains a list of the names of the known Call objects.
     * @retval QStringList The names of the Call objects.
     */
    QStringList calls() const;

    /**
     * @brief Invokes a ModuleObject defined by the provided MethodCall.
     * @param[in] MethodCall The MethodCall to be used for invocation.
     */
    void invokeCall ( const MethodCall& methodCall );

    ///< @brief Stops this module.
    Q_SLOT virtual void stop()  = 0;

    ///< @brief Starts this module.
    Q_SLOT virtual void start() = 0;

    /**
     * @brief Obtains the first module that has this Definition.
     * @param[in] Module::Definition The definition to search with.
     * @retval Module A pointer to the Module.
     * @retval nullptr A null pointer if no Definition matches any Module.
     */
    static Module* findModule ( const Module::Definition& definition );

    /**
     * @brief Obtains a list of the Modules in this process.
     * @retval ModuleList A list of Module object pointers.
     */
    static const ModuleList knownModules();

  protected:
    Q_DECLARE_PRIVATE ( Module )
    QScopedPointer<ModulePrivate> d_ptr; ///< @internal

    /**
     * @brief Mounts a ModuleCall object into this Module.
     * @param[in] The module call to be added.
     */
    void mountCall ( ModuleCall* call );

    /**
     * @brief Creates the Module.
     * @param QObject* The parent QObject in the QObject hierarchy.
     */
    explicit Module ( QObject* parent = wntrApp );

    /**
     * @brief Sets the definition of this Module.
     * @param[in] QString The domain of this module.
     * @param[in] QString The package of this module.
     * @param[in] quint64 The PID of this module..
     */
    void setDefinition(const QString& domain, const QString& package,
                       const quint64& pid = QCoreApplication::applicationPid());

    /**
     * @brief Sets the definition of this Module.
     * @param[in] Module::Definition The definition to use.
     */
    void setDefinition(const Module::Definition& aDef);

};

///< @brief Represents a definition-to-module mapper.
typedef QHash<Module::Definition, QPointer<Module>> ModuleHash;

} /*  Procedure */
} /*  Wintermute */

Q_DECLARE_INTERFACE(Wintermute::Procedure::Module,
                    "in.wintermute.procedure.module/0.1.0")

Q_DECLARE_METATYPE(Wintermute::Procedure::Module::Definition)

///< @internal Output stream handler for Definition.
QDataStream& operator <<(QDataStream& ,
                         const Wintermute::Procedure::Module::Definition& );

///< @internal Input stream handler for Definition.
QDataStream& operator >>(QDataStream& ,
                         Wintermute::Procedure::Module::Definition& );

///< @internal Hashing function for Definition.
// uint qHash(Wintermute::Procedure::Module::Definition& );

#endif /* WINTERMUTE_PROCEDURE_MODULE_HPP */
