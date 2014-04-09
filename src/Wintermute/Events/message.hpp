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

#ifndef WINTERMUTE_EVENTS_MESSAGE_HPP
#define WINTERMUTE_EVENTS_MESSAGE_HPP

#include <QtCore/QEvent>
#include <Wintermute/Globals>
#include <Wintermute/Procedure/Message>

namespace Wintermute
{
namespace Events
{
/**
 * @brief The basis of events that deal with Message objects.
 * @sa Message
 */
class MessageEvent : public QEvent
{
  public:
    /*
     * @brief Determines the flow direction at which this message is to
     *        be handled.
     */
    enum TransportDirection {
      DirectionReceive  = 0x1, ///<@brief Designates this as an incoming message.
      DirectionDispatch = 0x2  ///<@brief Designates this as an outgoing message.
    };

    /**
     * @brief Creates a new MessageEvent.
     * @param[in] TransportDirection The direction at which this message
     *            was wrought.
     * @param[in] Message The message in question.
     */
    explicit MessageEvent ( const TransportDirection& direction,
                            const Procedure::Message& message );
    virtual ~MessageEvent(); ///< @brief Destructor.

    /**
     * @brief Obtains the direction of the event.
     * @sa Message::TransportDirection.
     * @retval Message::TransportDirection.
     */
    TransportDirection direction() const;

    /**
     * @brief Obtains the message in question.
     * @retval Message The message.
     */
    const Procedure::Message& message() const;

    ///< @brief The event typeid for Qt.
    static const int EventType;

  private:
    TransportDirection m_direction;
    const Procedure::Message m_message;


};
} /* Events */
}

#endif
