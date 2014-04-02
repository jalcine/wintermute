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

#include <Wintermute/Application>
#include <Wintermute/Procedure/Call>

namespace Wintermute
{
namespace Procedure
{
class Module;
class MethodCall;
class ModuleCall : public Call
{
  Q_OBJECT;
  Q_DISABLE_COPY ( ModuleCall );
public:
  typedef std::function<void (QVariant)> CallbackSignature;
  explicit ModuleCall ( const QString& name, const Module* module );
  virtual ~ModuleCall();
  void setCallback ( CallbackSignature& signature );
  const Module& module() const;
  CallbackSignature callback() const;
  virtual QVariant invoke ( const QVariantList& arguments, const MethodCall& call ) = 0;

private:
  QPointer<const Module> m_module;
  CallbackSignature m_callback;
};
}
}

#endif
