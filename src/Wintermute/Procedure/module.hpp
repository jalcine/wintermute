/**
 * vim: ft=qt.cpp
 * Copyright (C) 2013 Jacky Alcine <me@jalcine.me>
 *
 * This file is part of Wintermute, the extensible AI platform.
 *
 *
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
 **/

#ifndef WINTERMUTE_PROCEDURE_MODULE_HPP
#define WINTERMUTE_PROCEDURE_MODULE_HPP

#include <QtCore/QString>
#include <Wintermute/Globals>
#include <Wintermute/Application>
#include <Wintermute/Procedure/LambdaCall>

namespace Wintermute
{
  namespace Procedure
  {
    class ModuleCall;
    class MethodCall;
    class ModulePrivate;

    /**
     * @class Wintermute::Procedure::Module
     * @see   Wintermute::Procedure::ModuleCall
     * @see   Wintermute::Procedure::MethodCall
     * @brief Represents a collection of related actions.
     *
     * Modules represent a hub of actions that can be invoked by Wintermute over
     * its own internal remote produce system.
     */
    class Module : public QObject
    {
        Q_OBJECT
        Q_DISABLE_COPY ( Module )

      protected:
        Q_DECLARE_PRIVATE ( Module )
        QScopedPointer<ModulePrivate> d_ptr;
        /**
         * Creates a Module
         */
        explicit Module ( QObject *parent = wntrApp );

      public:
        /**
         * @dtor
         * Destroys this Module and its calls.
         */
        virtual ~Module();

        /**
         * @function domain()
         * @brief    Obtains the domain name that this Module claims.
         */
        QString domain() const;

        /**
         * @function package()
         * @brief    Obtains the package that this Module claims.
         */
        QString package() const;

        /**
         * @function qualifiedName()
         * @see      module()
         * @see      package()
         * @brief    Obtains the qualified name this this Module claims.
         *
         * The qualified name of a module is created by concatenating the domain
         * name and the package name together. Thus, this method will return nothing
         * until both values are set by the sub-classing Module.
         */
        QString qualifiedName() const;

        /**
         * @function calls()
         */
        QStringList calls() const;

        /**
         * @function invokeCall()
         */
        void invokeCall ( const MethodCall &methodCall );

        /**
         * @function start()
         */
        Q_SLOT virtual void start() = 0;

        /**
         * @function stop()
         */
        Q_SLOT virtual void stop()  = 0;

        /**
         * @function findModule()
         */
        static QPointer<Module> findModule ( const QString &name );

        /**
         * @function  knownModules()
         */
        static QList<QPointer<Module>> knownModules();

      protected:
        void mountCall ( ModuleCall *call );
        void setDomain ( const QString &value );
        void setPackage ( const QString &value );
    };
  } /*  Procedure */
} /*  Wintermute */

Q_DECLARE_INTERFACE(Wintermute::Procedure::Module,
                    "in.wintermute.procedure.module/0.1.0")

#endif /* WINTERMUTE_PROCEDURE_MODULE_HPP */
