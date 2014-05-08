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

#include <Wintermute/Globals>
#include <Wintermute/Logging>
#include <Wintermute/Procedure/Module>
#include <QtCore/QVariantMap>

namespace Wintermute
{
namespace Procedure
{
/**
 * @brief Data class for `Wintermute::Procedure::Message`.
 *
 * Holds the underlying aspect of the data class for the `Message` class.
 * This allows for the class to be implicitly shared.
 */
struct MessagePrivate : public QSharedData
{
  MessagePrivate() : QSharedData(), sender(Module::Definition::Null),
    receiver(Module::Definition::Null), dataMap()
  {
  }

  MessagePrivate( const MessagePrivate& other ) : QSharedData ( other ),
    sender ( other.sender ), receiver ( other.receiver ),
    dataMap ( other.dataMap )
  {
  }

  virtual ~MessagePrivate()
  {
  }

  bool valid() const
  {
    Q_ASSERT ( isValidSenderData() );
    Q_ASSERT ( isValidReceiverData() );
    return isValidSenderData() && isValidReceiverData();
  }

  bool isValidSenderData () const
  {
    return sender.valid() && sender.pid != 0;
  }

  bool isValidReceiverData () const
  {
    return receiver.valid();
  }

  Module::Definition sender;
  Module::Definition receiver;
  QVariantMap dataMap;
};
}
}
