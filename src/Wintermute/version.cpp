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

#include "version.hpp"

using Wintermute::Version;

Version Version::Null = Version();

QString Version::toString() const
{
  QString versionStr;
  versionStr += QString::number ( this->major );
  versionStr += "." + QString::number ( this->minor );
  versionStr += "." + QString::number ( this->patch );
  return versionStr;
}

Version Version::fromString ( const QString& versionString )
{
  /// TODO Check if components of string can be converted to an integer.
  Version version;
  QStringList components = versionString.split ( "." );
  version.major = components[0].toUShort();
  version.minor = components[1].toUShort();
  version.patch = components[2].toUShort();
  return version;
}

bool Version::operator> ( const Version& version ) const
{
  if ( this->major > version.major ) return true;
  else if ( this->minor > version.minor ) return true;
  else if ( this->patch > version.patch ) return true;
  return false;
}

bool Version::operator< ( const Version& version ) const
{
  if ( this->major < version.major ) return true;
  else if ( this->minor < version.minor ) return true;
  else if ( this->patch < version.patch ) return true;
  return false;
}

bool Version::operator== ( const Version& version ) const
{
  return ( this->major == version.major ) &&
    ( this->minor == version.minor ) &&
    ( this->patch == version.patch );
}

QDataStream& operator<< ( QDataStream& out, const Wintermute::Version& version )
{
  out << version.toString();
  return out;
}

QDataStream& operator>> ( QDataStream& in, Wintermute::Version& version )
{
  QString versionStr;
  in >> versionStr;
  version = Version::fromString ( versionStr );
  return in;
}
