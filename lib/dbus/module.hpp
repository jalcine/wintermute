/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2013 Jacky Alciné <me@jalcine.me>
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

#ifndef WINTERMUTE_ZEROMQ_PLUGIN_MODULE_HPP
#define WINTERMUTE_ZEROMQ_PLUGIN_MODULE_HPP

#include <Wintermute/Procedure/Module>

namespace Wintermute
{
namespace DBus
{
class Plugin;
class Dispatcher;
class Receiver;
class Adapter;
class Module : public Wintermute::Procedure::Module
{
  Q_OBJECT;
  Dispatcher* m_dispatcher;
  Receiver* m_receiver;
public:
  explicit Module ( DBus::Plugin* plugin );
  Q_SLOT virtual void start();
  virtual ~Module();
};
}
}

#endif /* */
