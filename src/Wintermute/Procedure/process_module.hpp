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

#ifndef WINTERMUTE_CORE_PROCEDURE_PROCESS_MODULE_HPP
#define WINTERMUTE_CORE_PROCEDURE_PROCESS_MODULE_HPP

#include <Wintermute/Application>
#include <Wintermute/Procedure/Module>

namespace Wintermute {
  namespace Procedure {
    /**
     * @class ProcessModule
     *
     * ProcessModule serves as a means of exposing this current process over
     * the network. Typically the first module created by a running process,
     * it connects it to the other processes quickly.
     */
    class ProcessModule : public Module {
      friend class Wintermute::Application;
      explicit ProcessModule();
      public:
        virtual ~ProcessModule();
    };
  }
}

#endif
