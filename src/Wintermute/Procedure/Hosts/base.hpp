#ifndef WINTERMUTE_CORE_PROCEDURE_HOSTS_BASE_HPP
#define WINTERMUTE_CORE_PROCEDURE_HOSTS_BASE_HPP

#include <QtCore/QString>

namespace Wintermute {
  namespace Procedure {
    namespace Hosts {
      class Base {
        public:
          explicit Base();
          virtual ~Base();

          /**
           * @fn fullName
           * @brief Obtains name of host.
           */
          virtual QString fullName() const = 0;

      };
    }
  }
}

#endif /* end  */
