/**
 * vim: ft=cpp
 */

#ifndef WINTERMUTE_CORE_PROCEDURE_CALL_HPP
#define WINTERMUTE_CORE_PROCEDURE_CALL_HPP

#include <functional>
#include <QtCore/QObject>
#include <QtCore/QVariant>

namespace Wintermute {
  namespace Procedure {
    class Call : public QObject {
      Q_OBJECT;
      Q_DISABLE_COPY(Call);

      public:
        explicit Call (QObject* parent);
        virtual ~Call ();

        typedef std::function<QVariant (QVariantList)> Signature;
    };
  } /* Procedure */
} /* Wintermute */

#endif /* WINTERMUTE_CORE_PROCEDURE_CALL_HPP */
