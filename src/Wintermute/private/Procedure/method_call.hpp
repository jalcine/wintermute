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

#include <QtCore/QString>
#include <QtCore/QVariantList>
#include "Wintermute/Procedure/method_call.hpp"
#include "Wintermute/private/Procedure/call.hpp"

namespace Wintermute
{
namespace Procedure
{
class MethodCallPrivate : public CallPrivate
{
public:
  explicit MethodCallPrivate ( MethodCall* q ) : CallPrivate ( q ) {
    type = Call::TypeInvocation;
  }

  virtual ~MethodCallPrivate() {
  }

  MethodCall* q;
};
}
}
