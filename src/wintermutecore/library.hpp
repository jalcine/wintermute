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
class Library : W_DEF_SHAREABLE(Library)
{
public:
  W_DECL_PTR_TYPE(Library);
  typedef void* FunctionPtr;

  /* Default destructor. */
  virtual ~Library();

  /* Load a plugin with the provided filename. */
  explicit Library(const string & filename);

  enum LoadState
  {
    LoadUndefined    = 0x000,
    LoadNotLoaded    = 0x100,
    LoadIsLoaded     = 0x200,
    LoadStateSuccess = LoadIsLoaded,
    LoadStateFailure = LoadNotLoaded,
  };

  /* Loads a library into memory.
   * Using the provided 'filename', loads a library into memory. If the
   * load is successful ( a return value of LoadSuccess ), then filename()
   * will match the one provided. Otherwise, filename() != filename and the
   * return value will LoadSuccess != return_value.
   */
  LoadState load(const string & filename);

  /* Unloads a library from memory. */
  LoadState unload();

  /* Obtains the filename for the library. */
  string filename() const;

  /* Determine the current load status of the plugin. */
  LoadState loadedStatus() const;

  /* Resolves a function from the library. */
  FunctionPtr resolveFunction(const string & functionName) const;

  /* Finds a plugin through a varity of means.
   *  - '<name>'
   *  - 'lib<name>'
   *  - 'lib<name>.so'
   *  - '$WINTERMUTE_PLUGIN_PATH[i]/<name>'
   *  - '$WINTERMUTE_PLUGIN_PATH[i]/<name>.so'
   *  - '$WINTERMUTE_PLUGIN_PATH[i]/lib<name>.so'
   * */
  static Ptr find(const string & libraryQuery);

protected:
  /* Default constructor. */
  explicit Library();

private:
  W_DEF_PRIVATE(Library);
};
}

#define W_RESOLVE_FUNCTION(FunctionVariable, FunctionHandle) *(void **)(&FunctionVariable) = FunctionHandle;

#endif
