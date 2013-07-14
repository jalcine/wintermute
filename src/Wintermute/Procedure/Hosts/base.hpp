/**
 * vim: ft=qt.cpp
 *
 */
#ifndef WINTERMUTE_CORE_PROCEDURE_HOSTS_BASE_HPP
#define WINTERMUTE_CORE_PROCEDURE_HOSTS_BASE_HPP

#include <QtCore/QString>

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
      };
      typedef Host Base;
    }
  }
}

#endif /* end  */
