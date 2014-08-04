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

#ifndef WINTERMUTE_CORE_UTIL_SERIALIZER_HPP
#define WINTERMUTE_CORE_UTIL_SERIALIZER_HPP

#include <map>
#include <string>
#include <boost/variant.hpp>

namespace Wintermute
{
namespace Util
{
/* An interface to facilitate the act of serializing and deserializing data.
 * The Serializable interface allows for Wintermute::Util::Serializer to
 * convert it to the format that'd see fit for serialized communication. At
 * the time of writing, textual JSON is the preferred means of serialized
 * data.
 */
class Serializable
{
public:
  // Converts this serializable data into a JSON string.
  operator std::string() const;

  // Represents the internal string-to-string mapping of data.
  typedef std::map<std::string, std::string> Map;

protected:

  // Populates this serializable data with the provided map.
  Serializable(const Serializable::Map& dataMap);

  // Populates this serializable data with the provided JSON strong.
  Serializable(const std::string& jsonString);

  // An empty serializable object
  explicit Serializable();

  // Converts this object's representation into a map for serialization.
  virtual Serializable::Map serialize() const = 0;

  // Takes a map and expands the data stored into this working instance.
  virtual void deserialize(const Serializable::Map& data) = 0;

  // Takes a map and makes it into a deserializable string.
  static std::string toString(const Serializable::Map& data);

  // Takes a map and makes it into a deserializable string.
  static Serializable::Map fromString(const std::string& res);
};

}
}

#endif
