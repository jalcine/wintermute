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
    class Message;
  }
}

QDataStream &operator>>(QDataStream &, Wintermute::Procedure::Message & );
QDataStream &operator<<(QDataStream &, const Wintermute::Procedure::Message & );

namespace Wintermute
{
  namespace Procedure
  {
    class MessagePrivate;

    /**
     * @brief Basis of data transfer in RPC for Wintermute.
     *
     * This class serves as the basis for data, 'packets' if you will, for
     * Wintermute's RPC. It's meant to be used as a foundation for serializing
     * data like `Call` or `Signal` objects or so that they can be used over any
     * compatible medium.
     *
     * `Message` is registered into Qt's QMetaType system so that you can use it
     * wherever you'd use QVariant; for quick serializing across the board.
     */
    class Message
    {
        friend QDataStream &::operator>>(QDataStream &, Message & );
        friend QDataStream &::operator<<(QDataStream &, const Message & );

      public:
        Message(); /** @brief Creates an empty Message. */
        Message(const Message &other); /** @brief Copies an existing Message. */
        virtual ~Message(); /** @brief Deletes this Message. */

        /**
         * @brief Converts this Message into a QVariant.
         * @sa Message::operator QString() const
         * @sa Message::operator const char *() const
         * @retval QVariant Returns a variant value format of this Message.
         */
        operator QVariant() const;

        /**
         * @brief Converts this Message into a QString.
         * @sa Message::operator QVariant() const
         * @sa Message::operator const char *() const
         * @retval QString Returns a string format of this Message.
         */
        operator QString() const;

        /**
         * @brief Converts this Message into a C-style string.
         * @sa Message::operator QString() const
         * @sa Message::operator QVariant() const
         * @retval char* Returns a C-style string of this message.
         */
        operator const char *() const;

        /**
         * @brief Obtains an QVariant-based version of the internal data.
         *
         * The entries for this map are as follows:
         *    + sender information
         *    + receiver information
         *    + the data to be sent/received
         */
        QVariantList rawData() const;

        /**
         * @brief Converts this Message into a String.
         * @sa operator QString() const
         * @sa operator const char *() const
         */
        QString toString() const;

        /**
         * @brief Determines the validity of this Message.
         * @retval boolean Whether or not this Message is valid.
         */
        virtual bool valid() const;

        /**
         * @brief Obtains the module name that's sending this message.
         * @retval QString The qualified module name that's sending this message.
         */
        const QString sendingModuleName() const;

        /**
         * @brief Obtains the module name that's receiving this message.
         * @retval QString The qualified module name that's receiving this message.
         */
        const QString receivingModuleName() const;

        /**
         * @brief Determines if this message is local to this process.
         * @retval Whether or not this message was made here.
         */
        bool isLocal() const;

        /**
         * @brief Holds the integer that represents this type to `QMetaType`.
         */
        static const int MetaTypeId;

      protected:
        /**
         * @brief Sets the sender information with the new provided value.
         * @param[in] The new value to update the sender with.
         */
        void setSender ( const QVariant &newSenderData );
        /**
         * @brief Sets the receiver information with the new provided value.
         * @param[in] The new value to update the receiver with.
         */
        void setReceiver ( const QVariant &newReceiverData );
        /**
         * @brief Sets the free-form data with the new provided value.
         * @param[in] The new value to update the data with.
         */
        void setData ( const QVariant &newData );

        QSharedDataPointer<MessagePrivate> d; ///< Internal data pointer.
    };
  }
}

Q_DECLARE_METATYPE(Wintermute::Procedure::Message)

#endif
