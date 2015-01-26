/*
    Wintermute - A foundation for intelligent computing.
    Copyright (c) 2010 - 2015 by Jacky Alcine

    Wintermute is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any later version.

    Wintermute is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with Wintermute; if not, write to the
    Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#ifndef WINTERMUTE_CORE_UTIL_CONFIGURATION_HPP_
# define WINTERMUTE_CORE_UTIL_CONFIGURATION_HPP_

#include <wintermutecore/globals.hpp>
#include <list>

using std::list;

namespace Wintermute
{
namespace Util
{
class ConfigurationPrivate;
class Configuration
#ifndef DOXYGEN_SKIP
  : W_DEF_SHAREABLE(Configuration)
#endif
{
  W_DECL_PTR_TYPE(Configuration)
  W_DEF_PRIVATE(Configuration);

public:

  explicit Configuration(const string & configName);
  virtual ~Configuration();
  static bool addDirectory(const string& directory);
  static list<string> directories();

  static Ptr obtainStore(const string& configName);

  bool has(const string& key) const;

  string get(const string& key, const string& defaultValue) const;
  list<string> get(const string& key, const list<string>& defaultValue) const;
  bool get(const string& key, const bool& defaultValue) const;
  int get(const string& key, const int& defaultValue) const;
  float get(const string& key, const float& defaultValue) const;

  bool set(const string& key, const string& value);
  bool set(const string& key, const bool& value);
  bool set(const string& key, const int& value);
  bool set(const string& key, const float& value);
  bool set(const string& key, const list<string>& value);
};
}
}

#endif
