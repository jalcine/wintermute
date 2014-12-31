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

#ifndef WINTERMUTE_CORE_CALL_HPP
# define WINTERMUTE_CORE_CALL_HPP

#include <string>
#include <list>
#include <unordered_map>
#include <wintermutecore/util/serializable.hpp>
#include <wintermutecore/util/invokable.hpp>
#include <wintermutecore/globals.hpp>

using std::list;
using std::unordered_map;

namespace Wintermute
{
class CallPrivate;
/**
 * Represents a invokable action in Wintermute.
 * @ingroup Procedure Calling
 */
class WINTERMUTE_EXPORT_PUBLIC Call :
#ifndef DOXYGEN_SKIP
  W_DEF_SHAREABLE(Call),
#endif
  public Wintermute::Util::Invokable<const string, const string& >,
  public Wintermute::Util::Serializable
{
  W_DEF_PRIVATE(Call);

protected:
  /**
   * Default constructor.
   * @param name The name that the call would be identified by.
   */
  explicit Call(const string & name);

public:
  W_DECL_PTR_TYPE(Call)

  // Represents a list of Call objects.
  typedef list<Call::Ptr> List;

  typedef unordered_map<string, Call::Ptr> Map;

  virtual ~Call();

  /**
   * Obtains the name of this Call.
   * @return string
   */
  string name() const;

  /**
   * Serializes this call into Wintermute's hash.
   * @return Serializable::Map
   */
  virtual Serializable::Map serialize() const;

  /**
   *
   */
  virtual void deserialize(const Serializable::Map& data);
};
}

#endif
