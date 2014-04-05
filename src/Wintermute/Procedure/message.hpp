/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2014 Jacky Alciné <me@jalcine.me>
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

#ifndef WINTERMUTE_PROCEDURE_MESSAGE_HPP
#define WINTERMUTE_PROCEDURE_MESSAGE_HPP

/**
 * @file  Wintermute/Procedure/message.hpp
 * @brief Definition of `Wintermute::Procedure::Message`.
 */

#include <Wintermute/Globals>
#include <QtCore/QMetaType>
#include <QtCore/QDataStream>
#include <QtCore/QVariantMap>

namespace Wintermute
{
  namespace Procedure
  {
    class MessagePrivate;

    class Message
    {
      public:
        /**
         * @constructor
         * @brief Creates a new, empty Message.
         */
        Message();

        /**
         * @copy-constructor
         * @brief Copies an existing Message.
         */
        Message(const Message &other);

        /**
         * @destructor
         * @brief Deletes this Message.
         */
        virtual ~Message();

        /**
         * @brief Converts this Message into a QVariant.
         * @see operator QString() const
         * @see operator const char *() const
         */
        operator QVariant() const;

        /**
         * @brief Converts this Message into a QString.
         * @see operator QVariant() const
         * @see operator const char *() const
         */
        operator QString() const;

        /**
         * @brief Converts this Message into a C-style string.
         * @see operator QString() const
         * @see operator QVariant() const
         */
        operator const char *() const;

        /**
         * @fn rawData()
         * @brief Obtains a list of maps that represent the data stored by this
         * Message.
         *
         * The entries for this map are as follows:
         *    + sender information
         *    + receiver information
         *    + the data to be sent/received
         */
        QList<QVariantMap> rawData() const;

        /**
         * @fn toString()
         * @brief Converts this Message into a String.
         * @see operator QString() const
         * @see operator const char *() const
         */
        QString toString() const;

        /**
         * @constant
         * @brief Holds the integer that represents this type to `QMetaType`.
         */
        static const int MetaTypeId;

      protected:
        void setSender ( const QVariantMap &newSenderData );
        void setReceiver ( const QVariantMap &newSenderData );
        void setData ( const QVariantMap &newSenderData );
        QSharedDataPointer<MessagePrivate> d;
    };
  }
}

QDataStream &operator<<(QDataStream &out,
                        const Wintermute::Procedure::Message &myObj);
QDataStream &operator>>(QDataStream &in, Wintermute::Procedure::Message &myObj);

Q_DECLARE_METATYPE(Wintermute::Procedure::Message)

#endif
