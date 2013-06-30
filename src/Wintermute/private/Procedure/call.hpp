#ifndef WINTERMUTE_CORE_PRIVATE_PROCEDURE_CALL_HPP 
#define WINTERMUTE_CORE_PRIVATE_PROCEDURE_CALL_HPP

#include <QtCore/QVariantList>
#include <QtCore/QString>
#include "Wintermute/Procedure/call.hpp"

namespace Wintermute {
  namespace Procedure {
    struct CallPrivate {
      QVariantList arguments;
      QString remoteObjectName;
      QString remoteObjectMethod;
      Call::CallingMethod callMethod;
    };
  }
}

#endif
