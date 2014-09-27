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

#ifndef WINTERMUTE_LIBRARY_HPP
#define WINTERMUTE_LIBRARY_HPP

#include <wintermutecore/globals.hpp>

namespace Wintermute
{
  class LibraryPrivate;
  /* A class to load libraries and their function symbols.
   * Represents a portable wrapper over the facilities required to load
   * dynamically loadable libraries.
   */
  class Library : W_DEF_SHAREABLE(Library) {
    public:
      virtual ~Library();
      W_DECL_PTR_TYPE(Library)

      /* Load statuses for the library. */
      enum LoadStatus {
        LoadUndefined = 0x000,
        LoadSuccess   = 0x100,
        LoadFailure   = 0x200
      };

      /* Loads a library into memory.
       * Using the provided 'filename', loads a library into memory.
       */
      static Ptr load(const string& filename);

    protected:
      Library();

    private:
      W_DEF_PRIVATE(Library);
  };
}

#endif
