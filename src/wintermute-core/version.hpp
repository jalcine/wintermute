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

#ifndef WINTERMUTE_VERSION_HPP
# define WINTERMUTE_VERSION_HPP

#include <wintermutecore/globals.hpp>

namespace Wintermute
{
/**
 * Provides a uniform means of handling versioning.
 * @sa WINTERMUTE_VERSION
 */
class WINTERMUTE_EXPORT_PUBLIC Version
{
public:
  explicit Version(const string& versionStr);
  ~Version() = default;
  Version(const uint& versionMajor, const uint& versionMinor, const uint& versionPatch, const string& versionRc);
  operator string() const;

  uint major;
  uint minor;
  uint patch;
  string rc;

  static inline Version system() {
    return Version(WINTERMUTE_VERSION);
  }
};
}

/**
 * Less than comparison operation.
 * @param lhs Left value.
 * @param rhs Right value.
 * @sa Wintermtue::Version
 */
bool operator< (const Wintermute::Version& lhs, const Wintermute::Version& rhs);

/**
 * Greater than comparison operation.
 * @param lhs Left value.
 * @param rhs Right value.
 * @sa Wintermtue::Version
 */
bool operator> (const Wintermute::Version& lhs, const Wintermute::Version& rhs);

/**
 * Less than or equal to comparison operation.
 * @param lhs Left value.
 * @param rhs Right value.
 * @sa Wintermtue::Version
 */
bool operator<=(const Wintermute::Version& lhs, const Wintermute::Version& rhs);

/**
 * Greater than or equal to comparison operation.
 * @param lhs Left value.
 * @param rhs Right value.
 * @sa Wintermtue::Version
 */
bool operator>=(const Wintermute::Version& lhs, const Wintermute::Version& rhs);

/**
 * Equality comparison operation.
 * @param lhs Left value.
 * @param rhs Right value.
 * @sa Wintermtue::Version
 */
bool operator==(const Wintermute::Version& lhs, const Wintermute::Version& rhs);

/**
 * Not equal to comparison operation.
 * @param lhs Left value.
 * @param rhs Right value.
 * @sa Wintermtue::Version
 */
bool operator!=(const Wintermute::Version& lhs, const Wintermute::Version& rhs);

#endif
