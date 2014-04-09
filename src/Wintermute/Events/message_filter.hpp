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

#ifndef WINTERMUTE_EVENTS_MESSAGE_FILTER_HPP
#define WINTERMUTE_EVENTS_MESSAGE_FILTER_HPP

#include <QtCore/QObject>

namespace Wintermute
{
class ApplicationPrivate;
namespace Procedure
{
class Message;
}
namespace Events
{
class MessageEvent;

/**
 * @brief A QObject that acts as a filter for MessagEvent objects.
 * @sa MessageEvent
 * @sa Message
 */
class MessageEventFilter : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY ( MessageEventFilter )
    friend class Wintermute::ApplicationPrivate;

  public:
    explicit MessageEventFilter(); ///< @brief Constructor.
    virtual ~MessageEventFilter(); ///< @brief Destructor.

    /**
     * @brief Handles the filtering of the event.
     * @sa QObject::eventFilter
     */
    virtual bool eventFilter ( QObject* object, QEvent* event );

  private:
    bool handleDispatchingMessage(const Procedure::Message& message);
    bool handleReceivingMessage(const Procedure::Message& message);
};
}
}

#endif
