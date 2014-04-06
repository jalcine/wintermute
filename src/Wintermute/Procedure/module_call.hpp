/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2011 - 2014 Jacky Alciné <me@jalcine.me>
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

#ifndef WINTERMUTE_PROCEDURE_MODULE_CALL_HPP
#define WINTERMUTE_PROCEDURE_MODULE_CALL_HPP

#include <functional>
#include <Wintermute/Procedure/Call>
#include <Wintermute/Procedure/Module>

namespace Wintermute
{
  namespace Procedure
  {
    /**
     * @brief Allows the registration of a function into a module for invocation.
     * @sa Wintermute::Procedure::Module
     *
     * ModuleCall objects can be used to register calls into a Module object.
     */
    class ModuleCall : public Call
    {
      public:
        virtual ~ModuleCall();
        QPointer<const Module> module() const;
        virtual bool valid() const;
        virtual void invoke ( const QVariantList &arguments,
                              const MethodCall &call ) = 0;

      protected:
        explicit ModuleCall(const QString &name, QPointer<const Module> module);
        QPointer<const Module> m_module;
    };
  }
}

#endif
