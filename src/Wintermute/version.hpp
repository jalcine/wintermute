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
 * @brief      Definition of Wintermute::Version struct.
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
 * plug-ins as well. It goes as far as to store Git commit information.
 */

/// @todo Make this usable with `QVariant`.
/// @todo Add comparsion operators.
/// @todo Validate the format of the string.
/// @todo Validate integrity of data.
struct Version {
  /**
   * @brief Represents the possible stages at which this Version can be in.
   */
	enum DevelopmentStage {
	  Nightly   = 0x01,  ///< @brief Automated release.
	  Alpha     = 0x02,  ///< @brief Tested with bugs, but runnable.
	  Beta      = 0x03,  ///< @brief Tested but not-feature complete.
	  Release   = 0x10,  ///< @brief A point release on the timeline.
	  Final     = 0x11   ///< @brief A long-term, ABI-compatible (~80%) release.
	};

	quint8 major;            ///< @brief Represents the major release number.
	ushort minor;            ///< @brief Represents the minor release number.
	ushort patch;            ///< @brief Represents the patch number.
	QString hash;            ///< @brief The git commit hash of this release.
	DevelopmentStage stage;  ///< @brief The development state of this release.

  /**
   * @brief Represents a null version object;
   */
	static Version Null;

  /** 
   * @brief Converts a Version into a QString. 
   * @retval QString A string representing this Version.
   */
	inline QString toString() const
  {
		QString versionStr;
		versionStr += QString::number ( this->major );
		versionStr += "." + QString::number ( this->minor );
		versionStr += "." + QString::number ( this->patch );
		versionStr += "-";

		switch ( stage ) {
			case Alpha:
				versionStr += "a";
				break;
			case Beta:
				versionStr += "b";
				break;
			case Release:
				versionStr += "rc";
				break;
			case Final:
				versionStr += "r";
				break;
			default:
			case Nightly:
				versionStr += "dev";
				break;
		}
		versionStr += "." + this->hash;
		return versionStr;
	}

  /**  
   * @brief Converts a Version into a QString.
   * @retval Version Returns a Version that represents the provided string.
   */
	inline static Version fromString ( const QString& versionStr ) {
		Version version;
		QStringList components = versionStr.split ( "." );
		version.major = components[0].toUShort();
		version.minor = components[1].toUShort();

		QStringList patchInfo = components[2].split ( "-" );
		version.patch = patchInfo[0].toUShort();
		version.stage = ( DevelopmentStage ) patchInfo[1].toUShort();

		version.hash  = components[3];
		return version;
	}
};
}

QDataStream& operator<< ( QDataStream& out, const Wintermute::Version& version );
QDataStream& operator>> ( QDataStream& in,  Wintermute::Version& version );

Q_DECLARE_METATYPE ( Wintermute::Version )

#endif /* WINTERMUTE_VERSION_HPP */
