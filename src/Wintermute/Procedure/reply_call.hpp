/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2014 Jacky Alciné <me@jalcine.me>
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

#ifndef WINTERMUTE_PROCEDURE_REPLY_CALL_HPP
#define WINTERMUTE_PROCEDURE_REPLY_CALL_HPP
#include <Wintermute/Procedure/Call>

namespace Wintermute
{
  namespace Procedure
  {
    class MethodCall;
    class ReplyCall : public Call
    {
        Q_OBJECT
        const MethodCall &m_methodCall;

      public:
        ReplyCall ( const MethodCall &methodCall, const QVariant &response );
        virtual ~ReplyCall();

        QVariant response() const;
        const MethodCall &methodCall() const;
        virtual bool valid() const;
    };
  }
}

#endif
