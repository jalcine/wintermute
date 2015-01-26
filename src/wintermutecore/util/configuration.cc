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

#include "logging.hpp"
#include "globals.hpp"
#include "util.hh"
#include "util/configuration.hh"

using Wintermute::Util::ConfigurationPrivate;
using libconfig::Setting;
using Wintermute::Util::split_string;
using std::to_string;

list<string> ConfigurationPrivate::directories = list<string>();

void ConfigurationPrivate::buildConfigurationObject()
{
  if (!config)
  {
    config = make_shared<libconfig::Config>();
    config->setIncludeDir(WINTERMUTE_CONFIG_INCLUDE_DIR);
    config->setAutoConvert(false);
  }

  assert(config);
}

bool ConfigurationPrivate::loadConfiguration(const string& configName)
{
  buildConfigurationObject();

  if (!config)
  {
    werror("Can't obtain handle to configuration object.");
    return false;
  }

  string filePath = configName + ".cfg";

  try
  {
    config->readFile(filePath.c_str());
  }
  catch (libconfig::ParseException& e)
  {
    werror("Parsing of " + filePath + " failed: " + e.what());
    config.reset();
    return false;
  }
  catch (libconfig::FileIOException& e)
  {
    werror("Reading file for " + filePath + " failed: " + e.what());
    config.reset();
    return false;
  }
  catch(...)
  {
    werror("Exception raised while handling " + configName + ".");
    config.reset();
    return false;
  }

  return true;
}

Setting* ConfigurationPrivate::addSetting(const string& key, const Setting::Type& type)
{
  Setting* cfg = nullptr;
  wdebug("Attempting to build setting " + key + "...");

  if (!config->exists(key))
  {
    // So in order to set this value; we'd have to split the string on every
    // period and look for a key that matches our current deliminiator. If we
    // run into a problem, create said key and continue.

    cfg = &(config->getRoot());
    std::regex delim("[.]", std::regex::basic);
    auto tokens = split_string(key, delim);
    assert(!tokens.empty());
    wdebug("We got " + to_string(tokens.size()) + " tokens to handle.");

    for ( auto token : tokens )
    {
      if (!cfg->exists(token))
      {
        wdebug("Add key " + token);
        if (token != tokens.back())
        {
          wdebug("Not at end, grouping up...");
          cfg = &(cfg->add(token, Setting::TypeGroup));
        }
        else
        {
          wdebug("At end, making value.");
          cfg = &(cfg->add(token, type));
          wdebug("We made the setting " + string(cfg->getName()));
        }
      }
      else
      {
        wdebug("Look up key " + token);
        cfg = &((*cfg)[token]);
      }

      if (cfg->getPath() == key)
      {
        // No need to explicitly break, we're at the end.
        assert(cfg->getType() != Setting::TypeGroup);
        wdebug("Type of " + cfg->getPath() + " is " + to_string(cfg->getType()));
      }
    }
  }
  else
  {
    wdebug("Direct lookup for " + key + "...");
    cfg = &(config->lookup(key));
    wdebug("Obtained " + cfg->getPath() + ".");
    assert(cfg->isRoot() == false);
    assert(cfg->getPath() == key);
    assert(cfg->isGroup() == false);
  }

  assert(cfg);
  assert(cfg->isRoot() == false);
  wdebug("Obtained key name is " + string(cfg->getPath()));
  wdebug("Original key name is " + key);
  assert(cfg->getType() != Setting::TypeGroup);
  wdebug("The type of the key is " + to_string(cfg->getType()) + " vs " + to_string(type));

  return cfg;
}
