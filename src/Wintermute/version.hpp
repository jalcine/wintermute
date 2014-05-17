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
 * @headerfile Wintermute/version.hpp <Wintermute/Version>
 * @brief      Declaration of Wintermute::Version struct.
 */

#ifndef WINTERMUTE_VERSION_HPP
#define WINTERMUTE_VERSION_HPP

#include <QtCore/QString>
#include <QtCore/QDataStream>
#include <QtCore/QStringList>
#include <QtCore/QMetaType>

namespace Wintermute
{

  /**
   * @brief Provides a basis of defining versions in Wintermute.
   *
   * This struct provides a simple means of doing version checking on the
   * Wintermute executable and is also used (*and heavily recommended*) in
   * plug-ins as well.
   */

  /// @todo Make this usable with `QVariant`.
  /// @todo Add comparsion operators.
  /// @todo Validate the format of the string.
  /// @todo Validate integrity of data.
  struct Version {
    quint8 major;            ///< @brief Represents the major release number.
    ushort minor;            ///< @brief Represents the minor release number.
    ushort patch;            ///< @brief Represents the patch number.

    /**
     * @brief Represents a null version object;
     */
    static Version Null;

    /**
     * @brief Converts a Version into a QString.
     * @retval QString A string representing this Version.
     */
    QString toString() const;

    /**
     * @brief Converts a Version into a QString.
     * @retval Version Returns a Version that represents the provided string.
     */
    static Version fromString ( const QString& versionString );

    bool operator>  ( const Wintermute::Version& version ) const;
    bool operator<  ( const Wintermute::Version& version ) const;
    bool operator== ( const Wintermute::Version& version ) const;
  };
}

QDataStream& operator<< ( QDataStream& out, const Wintermute::Version& version );
QDataStream& operator>> ( QDataStream& in,  Wintermute::Version& version );

Q_DECLARE_METATYPE ( Wintermute::Version );

#endif /* WINTERMUTE_VERSION_HPP */
