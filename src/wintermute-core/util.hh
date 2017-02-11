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

#ifndef WINTERMUTE_CORE_UTIL_HH_
# define WINTERMUTE_CORE_UTIL_HH_

#include <cstdlib>
#include <list>
#include <regex>
#include <string>
#include <algorithm>
#include <iterator>

using std::list;
using std::regex;
using std::string;
using std::copy;
using std::sregex_token_iterator;
using std::back_inserter;
using std::begin;
using std::end;

namespace Wintermute
{
namespace Util
{
  // TODO: Document.
  inline list<string> split_string(const string& str, const regex& delim)
  {
    list<string> tokens;
    copy ( sregex_token_iterator(str.begin(), str.end(), delim, -1),
           sregex_token_iterator(),
           back_inserter(tokens) );

    return tokens;
  }

  // TODO: Document.
  inline string join_string(const list<string>& tokens, const string& delim)
  {
    string resultingString;

    if (!tokens.empty())
    {
      resultingString = tokens.front();

      for_each(++begin(tokens), end(tokens), [&](const string& token)
      {
        resultingString += delim + token;
      });
    }

    return resultingString;
  }

  /**
   * @fn Wintermute::Util::generate_uuid
   * @return A UUID as a string.
   * Generates a UUID.
   * @TODO: Make this generate pure UUIDs, as opposed to a random number.
   */
  inline string generate_uuid()
  {
    const int randomNumber = rand();
    const string uuidStr = std::to_string(randomNumber);
    return uuidStr;
  }

  /**
   * @fn
   */
  inline string get_tmpdir()
  {
    const string envName = "TMPDIR";
#ifdef CFE_secure_getenv
    const string envValue = secure_getenv(envName.c_str());
#else
    const string envValue = getenv(envName.c_str());
#endif

    return envValue;
  }
}
}

#endif
