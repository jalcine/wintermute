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

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include "Wintermute/private/Procedure/module.hpp"

namespace Wintermute
{
namespace Heartbeat
{
  class Process;
  class MonitorModulePrivate
  {
    public:
      QMap<qint64, Process*> processes;
      MonitorModule* q_ptr;
      Q_DECLARE_PUBLIC ( MonitorModule );

      MonitorModulePrivate ( MonitorModule* q ) :
        processes (), q_ptr ( q )
      {
        processes.clear();
        winfo(q, "Prepped to monitor this system.");
      }

      QVariant greet ( const QVariantList& arguments )
      {
        for (const QVariant i: arguments)
        {
          winfo(wntrApp, i.toString());
        }
        return 10;
      }

      /**
       * @fn record
       * @param QVariantList arguments
       *        - count: A count of the number of total pings sent.
       *        - type:  The type of ping this is.
       *        - pid:   PID of process that's pinging.
       */
      QVariant record ( const QVariantList& arguments )
      {
        // TODO: Check if said process exists.
        // TODO: Update process information.
        quint64 pid = arguments[2].toUInt();
        return true;
      }
  };
}
}
