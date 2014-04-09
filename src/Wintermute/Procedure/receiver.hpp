/**
 * @author Jacky Alciné <me@jalcine.me>
 * @copyright © 2011, 2012, 2013, 2014 Jacky Alciné <me@jalcine.me>
 * @if 0
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
 * @endif
 **/
/**
 * @file Wintermute/Procedure/receiver.hpp <Wintermute/Procedure/Receiver>
 * @brief Defines the Wintermute::Procedure::Reciever class.
 */

#ifndef WINTERMUTE_PROCEDURE_RECEIVER_HPP
#define WINTERMUTE_PROCEDURE_RECEIVER_HPP

#include <QtCore/QObject>

namespace Wintermute
{
namespace Procedure
{
class Message;
/**
 * @brief Represents a input message queue.
 *
 * Reciever objects are a single-threaded message intake system for
 * Wintermute. This class serves as a basis of what Receiver objects
 * should look like Wintermute and thus should based themselves on this.
 */
class Receiver : public QObject
{
    Q_OBJECT

  protected:
    explicit Receiver(QObject* parent = nullptr); ///< @brief Constructor.
    virtual ~Receiver(); ///< @brief Destructor.

    /**
     * @brief Takes a message and preps it for handling.
     * @param[in] Message& A reference to the message to handle.
     * @note Subclasses should call this method if they want their message
     *       to be handled by Wintermute.
     */
    Q_SLOT virtual void receiveMessage ( const Message& message );
};

///< @brief A list of Receiver pointers.
typedef QList<QPointer<Receiver>> ReceiverList;
}
}

#endif
