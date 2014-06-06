/*
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
 * @brief Declaration of `Wintermute::Procedure::Module`.
 */

#ifndef WINTERMUTE_PROCEDURE_MODULE_HPP
#define WINTERMUTE_PROCEDURE_MODULE_HPP

#include <Wintermute/Globals>
#include <Wintermute/Application>
#include <Wintermute/Procedure/Designation>
#include <QtCore/QCoreApplication>

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

///< @brief Represents a designation-to-module mapper.
typedef QHash<Designation, QPointer<Module>> ModuleHash;

/*
 * @brief Represents a collection of related actions.
 * @see   Wintermute::Procedure::ModuleCall
 * @see   Wintermute::Procedure::MethodCall
 *
 * Modules represent a hub of actions that can be invoked by Wintermute over
 * its own internal remote produce system.
 */
class Module : public QObject
{
    Q_OBJECT;
    Q_DISABLE_COPY ( Module );

  public:
    ///< @brief Destroys this Module and its calls.
    virtual ~Module();

    ///< @brief Obtains the designation of this Module.
    Designation designation() const;

    ///< @brief Obtains a list of the names of the known Call objects.
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

    static Module* findModule ( const Designation& designation );

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
     * @brief Sets the designation of this Module.
     * @param[in] QString The domain of this module.
     * @param[in] QString The package of this module.
     * @param[in] quint64 The PID of this module..
     */
    void setDesignation(const QString& domain, const QString& package,
                        const quint64& pid = QCoreApplication::applicationPid());

    /**
     * @brief Sets the designation of this Module.
     * @param[in] Module::Designation The designation to use.
     */
    void setDesignation(const Designation& aDef);

};


} /*  Procedure */
} /*  Wintermute */

Q_DECLARE_INTERFACE(Wintermute::Procedure::Module,
                    "in.wintermute.procedure.module/0.0.1")

#endif /* WINTERMUTE_PROCEDURE_MODULE_HPP */
