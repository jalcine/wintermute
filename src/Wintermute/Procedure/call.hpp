/**
 * vim: ft=cpp
 */

#ifndef WINTERMUTE_CORE_PROCEDURE_CALL_HPP
#define WINTERMUTE_CORE_PROCEDURE_CALL_HPP

#include <QtCore/QObject>
#include <Wintermute/Application>

namespace Wintermute {
  namespace Procedure {
    class CallPrivate;
    /**
     * @class Call
     *
     * `Call` embodies the act of sending a method call to another module running 
     * in a separate instance of Wintermute. The way that it's built permits 
     * the actual calling logic to be hidden away from the callee and properly 
     * return the information desired.
     */
    class Call : public QObject {
      Q_OBJECT;
      Q_DISABLE_COPY(Call);
      Q_DECLARE_PRIVATE(Call);
      Q_ENUMS(CallingMethods);

      // TODO: Define getter/setters as properties.

      QSharedPointer<CallPrivate> d_ptr;

      enum CallingMethod {
        Block      = 0x1,
        Background = 0x2,
        Discard    = 0x0
      };

      public:
      /**
       * @ctor
       * @brief Creates a new Call.
       *
       * Creates a new Call object that considers the callee to be its parent 
       * (and callee). The callee object is used to determine the thread at 
       * which to return to once operation has been backgrounded.
       */
      explicit Call(QObject* callee = Wintermute::Application::instance());
      virtual ~Call();

      /**
       * @fn arguments
       * @brief Obtains the arguments defined for this call.
       */
      const QVariantList& arguments() const;

      /**
       * @fn setArguments
       * @brief Defines the arguments to be passed.
       *
       * Sets the arguments to be passed over in conjunction to the method 
       * call.
       */
      void setArguments(const QVariantList& arguments);

      /**
       * @fn callingMethod
       * @brief Defines the method at which this call will be invoked.
       */
      const CallingMethod& callingMethod() const;

      /**
       * @fn setCallingMethod
       * @brief Defines the calling method state of the call.
       *
       * Determines which state at which the call will dispatch itself to the 
       * remote object.
       */
      void setCallingMethod(const CallingMethod& callMethod);

      /**
       * @fn setRemoteObject
       * @brief Defines the remote object to be called.
       * @param name The name of the object to be invoked upon.
       * @param method The method of the object to be invoked.
       *
       * All remote objects and their methods are called according to the 
       * following object-method signature "organization.object.name#method".
       * At the time of writing, methods are NOT namespaced so this should be 
       * done within the object name itself.
       */
      void setRemoteObject(const QString& name, const QString& method);

      /**
       * @fn invoke
       * @brief Invokes the call to the remote object.
       * @param remoteObjectName The name of the object to invoke the method.
       * @param remoteObjectMethod The method of the object to invoke.
       * 
       * Invokes the call request. Depending on the calling method state of 
       * the call, this would return once dispatched with no checking of the 
       * status of action (Discard), prevent execution in the calling thread 
       * until it returned (Block) or execute in a separate thread and raise 
       * the invocationComplete signal when a result has been returned 
       * (Background). One would note that the speed of invocation follows 
       * this expression: Discard < Background < Block where Block > 1 and 
       * Discard >= 0
       */
      Q_SLOT QObject* invoke(const QString& remoteObjectName = QString(), const QString& remoteObjectMethod = QString());

      /**
       * @fn invocationComplete
       * @brief Raised when an background call has elasped.
       */
      Q_SIGNAL void invocationComplete(const QObject* result);
    };
  } /* Procedure */ 
} /* Wintermute */ 

#endif /* end WINTERMUTE_CORE_PROCEDURE_CALL_HPP */
