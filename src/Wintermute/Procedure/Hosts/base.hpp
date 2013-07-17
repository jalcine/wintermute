/**
 * vim: ft=qt.cpp
 *
 */
#ifndef WINTERMUTE_CORE_PROCEDURE_HOSTS_BASE_HPP
#define WINTERMUTE_CORE_PROCEDURE_HOSTS_BASE_HPP

#include <QtCore/QString>
#include <Wintermute/Procedure/Call>

namespace Wintermute {
  namespace Procedure {
    namespace Hosts {
      class Base {
        public:
          /**
           * @ctor
           * Handles the construction of this host.
           */
          explicit Base();

          /**
           * @dtor
           * Handles the destruction of this host.
           */
          virtual ~Base();

          /**
           * @fn fullName
           * @brief Obtains name of host.
           */
          virtual QString fullName() const = 0;

          /**
           * @fn discoverHost
           *
           * Searches for a host with the specified name.
           */
          static Base* discoverHost(QString& qualifiedName);

        protected:
          /**
           * @slot callRecieved
           *
           * Handles the act of manipulating incoming calls.
           */
          Q_SLOT void callRecieved(Call* const call) = 0;

          /**
           * @fn dispatchCall
           * Handles the work of sending a call.
           */
          bool dispatchCall(Call* const call);
      };
    }
  }
}

#endif /* end  */
