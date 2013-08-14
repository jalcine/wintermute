/**
 * vim: ft=qt.cpp
 * Copyright (C) 2013 Jacky Alcine <me@jalcine.me>
 *
 * This file is part of Wintermute, the extensible AI platform.
 *
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

#ifndef WINTERMUTE_CORE_PROCEDURE_CALL_HPP
#define WINTERMUTE_CORE_PROCEDURE_CALL_HPP

#include <functional>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QSharedPointer>

namespace Wintermute {
  namespace Procedure {
    class CallPrivate;
    class Call : public QObject {
      Q_OBJECT;
      Q_DISABLE_COPY(Call);

      protected:
      Q_DECLARE_PRIVATE(Call);
      QSharedPointer<CallPrivate> d_ptr;

      public:
      /**
       * @typedef Signature
       *
       * Provides the short-hand signature for Call methods.
       */
      typedef std::function<QVariant (QVariantList)> Signature;

      /**
       * @typedef Callback
       *
       * Provides the callback necessary for async calling.
       */
      typedef std::function<void (QVariant)> Callback;

      /**
       * @enum Type
       *
       * Defines the potential types a Call can take in the procedural system.
       */
      enum Type {
        Method    = 0x001, // This call is for handling and manipulating methods.
        Signal    = 0x002, // This call is for interacting with signals on the platform.
        Existance = 0x003 // This call handles the existence of other things.
      };

      explicit Call (QObject* parent);
      virtual ~Call ();

      /**
       * @fn dispatch
       * @brief Invokes this call out to the network to its intended receiver.
       *
       * Converts this message to a simpler string for RPC digestion and sends
       * it over the network to be recieved by others.
       */
      virtual QVariant dispatch(QVariantList& arguments) final;

      /**
       * @fn doDispatch
       * @brief Invokes the call in the background and invokes callback on complete.
       *
       * Does what dispatch does except that it's done in a separate thread
       * and upon completion, it invokes the callback passed.
       */
      virtual void doDispatch(QVariantList& arguments, Callback callback);

      /**
       * @fn type
       * @brief Obtains the type of call this is.
       *
       * Obtains the type of message that this exposes.
       */
      Type type() const;

      /**
       * @fn toString
       * @brief Generates a string representation of this call.
       */
      QString toString() const;

      /**
       * @fn recipient
       * @brief Obtains the name of the module to recieve this message.
       *
       * Obtains the qualified string name that this Call is sending a message
       * to.
       */
      QString recipient() const;

      /**
       * @fn setRecipient
       * @brief Changes the recipient of this Call.
       */
      void setRecipient(const QString moduleName);

    };
  } /* Procedure */
} /* Wintermute */

#endif /* WINTERMUTE_CORE_PROCEDURE_CALL_HPP */
