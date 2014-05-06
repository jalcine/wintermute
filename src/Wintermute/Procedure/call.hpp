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

#ifndef WINTERMUTE_PROCEDURE_CALL_HPP
#define WINTERMUTE_PROCEDURE_CALL_HPP

#include <Wintermute/Procedure/Message>

namespace Wintermute
{
namespace Procedure
{
class Call : public Message
{
  public:
    virtual ~Call(); ///< @brief Destructor.
    Call(const Call& other);
    Call(const Message& other);
    static Call Null;

    /**
     * @brief Determines if this call is valid.
     * @retval boolean Whether or not this call is valid.
     */
    virtual bool valid() const;

    /**
     * @brief Obtains the name of this call.
     * @retval QString The name of this call.
     */
    QString name() const;

  protected:
    /**
     * @brief Creates a Call named 'name'.
     * @param[in] QString The new name of this call.
     */
    explicit Call(const QString& name);

    /**
     * @brief Obtains the variant map of this call.
     * @retval QVariantMap The variant map of this call.
     */
    QVariantMap callData() const;

    /**
     * @brief Updates the name of this call.
     * @param[in] QString The new name of this call.
     */
    void setName(const QString& newName);

    /**
     * @brief Updates the data of this call.
     * @param[in] QVariantMap The new data for this call.
     */
    void setCallData(const QVariantMap& data);

};
}
}

#endif /* WINTERMUTE_PROCEDURE_CALL_HPP */
