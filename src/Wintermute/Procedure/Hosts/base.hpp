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
      class BasePrivate;
      class Base {
        Q_OBJECT;
        Q_DISABLE_COPY(Base);
        Q_DECLARE_PRIVATE(Base);
        QSharedPointer<BasePrivate> d_ptr;


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
         *
         * When discovering or querying for hosts, this method intends to
         * return the fully qualified name.
         */
        virtual QString fullName() const = 0;

        /**
         * @fn discoverHost
         *
         * Searches for a host with the specified name and returns the
         * host's information for invocation.
         */
        static Base* discoverHost(QString& qualifiedName);

        /**
         * @fn invokeCall
         */
        bool invokeCall(const QString& callMethod, const QVariantList& arguments);

        protected:
        /**
         * @slot callRecieved
         *
         * Handles the act of manipulating incoming calls.
         */
        Q_SLOT virtual void callRecieved(Call* const call) = 0;

        /**
         * @fn dispatchCall
         * Handles the work of sending a call.
         */
        bool dispatchCall(Call* const call);

        /**
         * @fn registerCall
         * Adds a call as a valid call for this object.
         */
        bool registerCall(Call* const call);
      };
    }
  }
}


#endif /* end  */
