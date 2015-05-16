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

#include <cstring>
#include "util.hh"
#include "logging.hpp"
#include "configuration.hh"
#include "configuration.hpp"

using Wintermute::Util::Configuration;
using Wintermute::Util::Serializable;
using Wintermute::Util::ConfigurationPrivate;
using libconfig::Setting;
using std::to_string;
using Wintermute::Util::join_string;
using Wintermute::Util::split_string;

Configuration::Configuration(const string& name) :
  d_ptr(new ConfigurationPrivate)
{
  W_PRV(Configuration);
  assert(d->loadConfiguration(name));
}

Configuration::~Configuration()
{
  // TODO: Unload the configuration, I guess?
}

bool Configuration::addDirectory(const string& directory)
{
  wdebug(directory);
  return true;
}

list<string> Configuration::directories()
{
  return ConfigurationPrivate::directories;
}

Configuration::Ptr Configuration::obtainStore(const string& storeName)
{
  wdebug("Fetching configuration of " + storeName + "...");
  Ptr configPtr = nullptr;
  configPtr = make_shared<Configuration>(storeName);
  assert(configPtr);
  return configPtr;
}

string Configuration::get(const string& key, const string& defaultValue) const
{
  W_PRV(const Configuration);
  if (has(key))
  {
    return d->config->lookup(key);
  }

  return defaultValue;
}

bool Configuration::get(const string& key, const bool& defaultValue) const
{
  W_PRV(const Configuration);
  if (has(key))
  {
    return d->config->lookup(key);
  }

  return defaultValue;

}

float Configuration::get(const string& key, const float& defaultValue) const
{
  W_PRV(const Configuration);
  if (has(key))
  {
    return d->config->lookup(key);
  }

  return defaultValue;
}

int Configuration::get(const string& key, const int& defaultValue) const
{
  W_PRV(const Configuration);
  if (has(key))
  {
    return d->config->lookup(key);
  }

  return defaultValue;
}

bool Configuration::has(const string& key) const
{
  W_PRV(const Configuration);
  return d->config->exists(key);
}

list<string> Configuration::get(const string& key,
  const list<string>& defaultValue) const
{
  const string theOriginalValue = get(key, string());
  std::regex delim("[;]", std::regex::basic);
  list<string> value = split_string(theOriginalValue, delim);
  return defaultValue;
}

#define _W_CFG_SET_VALUE(Value, Key, Type) \
  W_PRV(Configuration); \
  Setting* cfg = d->addSetting(Key, Type); \
  assert(cfg); \
  *cfg = Value; \
  return static_cast<decltype(Value)>(*cfg) == Value; \

bool Configuration::set(const string& key, const string& value)
{
  _W_CFG_SET_VALUE(value.c_str(), key, Setting::TypeString);
}

bool Configuration::set(const string& key, const int& value)
{
  _W_CFG_SET_VALUE(value, key, Setting::TypeInt);
}

bool Configuration::set(const string& key, const bool& value)
{
  _W_CFG_SET_VALUE(value, key, Setting::TypeBoolean);
}

bool Configuration::set(const string& key, const float& value)
{
  _W_CFG_SET_VALUE(value, key, Setting::TypeFloat);
}

bool Configuration::set(const string& key, const list<string>& value)
{
  const string finalString = join_string(value, ";");
  return set(key, finalString);
}
