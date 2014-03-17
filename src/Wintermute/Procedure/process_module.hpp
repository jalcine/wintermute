/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2011 - 2013 Jacky Alciné <me@jalcine.me>
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

#ifndef WINTERMUTE_PROCEDURE_PROCESS_MODULE_HPP
#define WINTERMUTE_PROCEDURE_PROCESS_MODULE_HPP

#include <Wintermute/Application>
#include <Wintermute/Procedure/Module>
#include "module.hpp"

namespace Wintermute
{
namespace Procedure
{
/**
 * @class ProcessModule
 *
 * ProcessModule serves as a means of exposing this current process over
 * the network. Typically the first module created by a running process,
 * it connects it to the other processes quickly.
 */
class ProcessModule : public Module
{
  Q_OBJECT;
  Q_DISABLE_COPY ( ProcessModule );
  Q_INTERFACES (Wintermute::Procedure::Module);

  friend class Wintermute::ApplicationPrivate;
  explicit ProcessModule();

public:
  virtual ~ProcessModule();
  Q_SLOT virtual void start();
  Q_SLOT virtual void stop();
  Q_SLOT void greetSystem ( const QString& name );

protected:
  void reboot();
  void quit ( const int exitcode = 0 );
};
}
}

#endif
