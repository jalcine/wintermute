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

namespace Wintermute
{
namespace Util
{
/**
 * An interface to facilitate the act of serializing and deserializing data.
 *
 * The Serializable interface allows for Wintermute::Util::Serializer to
 * convert it to the format that'd see fit for serialized communication. At
 * the time of writing, textual JSON is the preferred means of serialized
 * data.
 */
class Serializable
{
public:
  /// Represents the type that all Serializable objects are converted into.
  typedef std::string PlainType;

  /// Represents the key-value store that's used to represents Serializable
  /// objects.
  typedef std::map<std::string, std::string> Map;

  /// Converts this serializable data into a JSON string.
  operator PlainType() const;

  /// Converts this serialized data into a map.
  operator Map() const;

  /// Sets this object to the data stored in the stringified hash.
  Serializable& operator =(const PlainType& jsonString);

  /// Sets this object to the data stored in the hash.
  Serializable& operator =(const Map& data);

  /**
   * Takes a map and makes it into to the plain data type.
   * @param data The key-value pairs used to craft said structure.
   * @return The serializable value in the PlainType type.
   *
   * With the provided key-value pairs, it converts each of the values into a
   * single JSON structure.
   */
  static PlainType toString(const Serializable::Map& data);

  /// Takes a map and makes it into a deserializable string.
  static Serializable::Map fromString(const std::string& res);

protected:

  /// Populates this serializable data with the provided map.
  Serializable(const Serializable::Map& dataMap);

  /// Populates this serializable data with the provided JSON strong.
  Serializable(const std::string& jsonString);

  /// An empty serializable object
  explicit Serializable();

  /// Converts this object's representation into a map for serialization.
  virtual Serializable::Map serialize() const = 0;

  /// Takes a map and expands the data stored into this working instance.
  virtual void deserialize(const Serializable::Map& data) = 0;
};

}
}

#define W_SERIALIZABLE(Class) \
  virtual Wintermute::Util::Serializable::Map serialize() const; \
  virtual void deserialize(const Wintermute::Util::Serializable::Map& data);

#endif
