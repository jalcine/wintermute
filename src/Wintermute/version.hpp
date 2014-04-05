/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2011 - 2013 Jacky Alciné <me@jalcine.me>
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

#ifndef WINTERMUTE_CORE_VERSION_HPP
#define WINTERMUTE_CORE_VERSION_HPP

#include <QtCore/QString>
#include <QtCore/QDataStream>
#include <QtCore/QStringList>
#include <QtCore/QMetaType>

namespace Wintermute
{
/***
 * @class Wintermute::Version
 * @brief Provides a basis of defining version numbers.
 * @todo  Consider making the number into a bit flag.
 */
struct Version {
	enum DevelopmentStage {
	  Nightly = 0,
	  Alpha,
	  Beta,
	  ReleaseCandidate,
	  FinalRelease
	};

	ushort major;
	ushort minor;
	ushort patch;
	QString stage;
	DevelopmentStage state;

	static Version Any;

	inline QString toString() const {
		QString versionStr;
		versionStr += QString::number ( this->major );
		versionStr += "." + QString::number ( this->minor );
		versionStr += "." + QString::number ( this->patch );
		versionStr += "-";
		switch ( state ) {
			case Alpha:
				versionStr += "a";
				break;
			case Beta:
				versionStr += "b";
				break;
			case ReleaseCandidate:
				versionStr += "rc";
				break;
			case FinalRelease:
				versionStr += "r";
				break;
			default:
			case Nightly:
				versionStr += "dev";
				break;
		}
		versionStr += "." + this->stage;
		return versionStr;
	}

	inline static Version fromString ( const QString& versionStr ) {
		Version version;
		QStringList components = versionStr.split ( "." );
		QStringList patchInfo = components[2].split ( "-" );
		version.major = components[0].toUShort();
		version.minor = components[1].toUShort();
		version.patch = patchInfo[0].toUShort();
		version.stage = components[3];
		version.state = ( DevelopmentStage ) patchInfo[1].toUShort();
		return version;
	}
};
}

QDataStream& operator<< ( QDataStream& out, const Wintermute::Version& version );
QDataStream& operator>> ( QDataStream& in,  Wintermute::Version& version );

Q_DECLARE_METATYPE ( Wintermute::Version )

#endif /* WINTERMUTE_CORE_VERSION_HPP */
