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

#include "serializable.hpp"
#include "globals.hpp"
#include "../logging.hpp"
#include <json/json.h>

using std::string;
using Wintermute::Util::Serializable;

Serializable::Serializable()
{

}

Serializable::Serializable(const string& jsonString)
{
  wtrace("We should do something with " + jsonString + ".");
  // TODO How do we attempt deserialization when we can't use the
  // deserialization method?
}

Serializable::~Serializable()
{
}

string Serializable::toString(const Serializable::Map& data)
{
  Json::Value mappedJson;

  for (auto itr : data)
  {
    mappedJson[itr.first] = itr.second;
  }

  Json::FastWriter jsonWriter;
  const string jsonString = jsonWriter.write(mappedJson);
  return jsonString;
}

Serializable::Map Serializable::fromString(const string& jsonString)
{
  //wdebug("Deserializing JSON: " + jsonString);
  Serializable::Map theMap;
  Json::Value incomingJson;

  Json::Reader jsonReader;
  if (!jsonReader.parse(jsonString, incomingJson))
  {
    wtrace("Failed to deserialize JSON: " + jsonString);
    return theMap;
  }

  for (const string name : incomingJson.getMemberNames())
  {
    wtrace("Deserializing JSON key: " + name + " for " + jsonString);
    const Json::Value currentValue = incomingJson[name];
    string currentStringValue;

    switch (currentValue.type())
    {
      case Json::intValue:
        currentStringValue = std::to_string(currentValue.asInt());
        break;

      case Json::realValue:
        currentStringValue = std::to_string(currentValue.asDouble());
        break;

      default:
        currentStringValue = currentValue.asString();
        break;
    }

    theMap.insert(std::make_pair(name, currentStringValue));
  }

  return theMap;
}

Serializable::operator Serializable::PlainType() const
{
  Json::Value serializedJson;
  Serializable::Map serializedData = static_cast<Serializable::Map>(*this);

  for (auto itr : serializedData)
  {
    serializedJson[itr.first] = itr.second;
  }

  Json::FastWriter jsonWriter;
  string jsonString = jsonWriter.write(serializedJson);
  jsonString.resize(jsonString.size() - 1);

  assert(!jsonString.empty());
  wtrace("Serialized into " + jsonString);
  return jsonString;
}

Serializable::operator Serializable::Map() const
{
  return serialize();
}

Serializable& Serializable::operator =(const string& jsonString)
{
  Map map = fromString(jsonString);
  deserialize(map);
  return *this;
}

Serializable& Serializable::operator =(const Map& data)
{
  deserialize(data);
  return *this;
}
