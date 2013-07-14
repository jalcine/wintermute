#ifndef WINTERMUTE_CORE_PRIVATE_PROCEDURE_CALL_HPP 
#define WINTERMUTE_CORE_PRIVATE_PROCEDURE_CALL_HPP

#include <QtCore/QVariantList>
#include <QtCore/QString>
#include "Wintermute/Procedure/call.hpp"

namespace Wintermute {
  namespace Procedure {
    class CallPrivate {
      Call* q_ptr;
      public:
      // Private values.
      QVariantList arguments;
      QString remoteObjectName;
      QString remoteObjectMethod;
      Call::CallingMethod callMethod;

      Q_DECLARE_PUBLIC(Call);

      CallPrivate(Call* q) : q_ptr(q) { }
      virtual ~CallPrivate() { }
    };
  }
}

#endif
