/*
 * Wintermute is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with Wintermute; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "version.hpp"

using std::to_string;
using std::atoi;
using Wintermute::Version;

Version::Version(const string& versionStr)
{
  const int majorDeliminator = versionStr.find(".");
  const int minorDeliminator = versionStr.find(".", majorDeliminator + 1);

  major = atoi(versionStr.substr(0, majorDeliminator).c_str());
  minor = atoi(versionStr.substr(majorDeliminator + 1, minorDeliminator - majorDeliminator).c_str());
  patch = atoi(versionStr.substr(minorDeliminator + 1, versionStr.length() - minorDeliminator).c_str());
}

bool operator< (const Wintermute::Version& lhs, const Wintermute::Version& rhs)
{
  if (lhs.major < rhs.major) return true;
  if (lhs.minor < rhs.minor) return true;
  if (lhs.patch < rhs.patch) return true;

  return false;
}

bool operator> (const Wintermute::Version& lhs, const Wintermute::Version& rhs)
{
  if (lhs.major > rhs.major) return true;
  if (lhs.minor > rhs.minor) return true;
  if (lhs.patch > rhs.patch) return true;

  return false;
}

bool operator<=(const Wintermute::Version& lhs, const Wintermute::Version& rhs)
{
  return (lhs < rhs) || lhs == rhs;
}

bool operator>=(const Wintermute::Version& lhs, const Wintermute::Version& rhs)
{
  return (lhs > rhs) || lhs == rhs;
}

bool operator==(const Wintermute::Version& lhs, const Wintermute::Version& rhs)
{
  return (lhs.major == rhs.major) &&
         (lhs.minor == rhs.minor) &&
         (lhs.patch == rhs.patch);
}

bool operator!=(const Wintermute::Version& lhs, const Wintermute::Version& rhs)
{
  return ! (lhs == rhs);
}

Version::operator string() const
{
  return to_string(major) + "." +
         to_string(minor) + "." +
         to_string(patch);
}
