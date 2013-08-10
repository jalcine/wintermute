#ifndef WINTERMUTE_CORE_PROCEDURE_MODULE_HPP
#define WINTERMUTE_CORE_PROCEDURE_MODULE_HPP

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <Wintermute/Procedure/Call>

namespace Wintermute {
  namespace Procedure {
    class ModulePrivate;
    class Module : public QObject {
      Q_OBJECT;
      Q_DISABLE_COPY(Module);
      QScopedPointer<ModulePrivate> d_ptr;

      public:
      explicit Module(QObject* parent);
      virtual ~Module();

      /**
       * @fn qualifiedName
       * Obtains a fully qualified name for this Module.
       */
      virtual QString qualifiedName() const;

      /**
       * @fn mountCall
       * Takes a function reference and a name and registers said reference
       * as a valid call.
       */
      //bool mountCall(Call::Signature& callSig, QString& name);
    };
  } /*  Procedure */
} /*  Wintermute */

#endif /* WINTERMUTE_CORE_PROCEDURE_MODULE_HPP */
