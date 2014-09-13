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

#ifndef WINTERMUTE_PLUGIN_HPP
#define WINTERMUTE_PLUGIN_HPP

#include "globals.hpp"
#include "logging.hpp"
#include <list>
#include <map>
#include <string>
#include <functional>

using std::map;
using std::list;
using std::string;
using std::function;

namespace Wintermute
{
class PluginPrivate;
class LibraryPrivate;
class WINTERMUTE_EXPORT Plugin :
  W_DECLARE_SHAREABLE(Plugin)
{
  W_DEFINE_PRIVATE(Plugin)

public:
  virtual ~Plugin();
  explicit Plugin(Plugin& other);
  W_DECLARE_PTR_TYPE(Plugin)

  // Provides the known load states for a particular plugin.
  enum WINTERMUTE_EXPORT LoadState
  {
    // Represents an undefined state.
    LoadUndefined = 0x000,

    // Represents the state of an unloaded plugin.
    Unloaded      = 0x100,

    // Represents the state of a failed plugin load.
    LoadingFailed = 0x200,

    // Represents the state of a successfully loaded plugin.
    Loaded        = 0x400
  };

  // Represents all of the possible failure states for a plugin.
  enum WINTERMUTE_EXPORT LoadFailure
  {
    // Represents an undefined failure state.
    FailureUndefined      = 0x0000,

    // Represents a state of no failure.
    FailureNone           = FailureUndefined,

    // Defines a failing load state that relates to the library.
    FailureLibrary        = 0x0001,
    // Defines a failing load state that relates to the failure to resolve a
    // symbol.
    FailureSymbol         = 0x0002,

    // Defines a failing load state that relates an incompatible binary
    // version.
    FailureABI            = 0x1000,

    // Defines a failing load state that relates to a failure in interacting
    // with the library due to a ABI issue.
    FailureABIException   = 0x1100,

    // Defines a failing load state that relates to the loss of a particular
    // symbol or object.
    FailureMissing        = 0x2000,

    // Defines a failing load state to the inability to find the library in
    // question.
    FailureMissingLibrary = FailureMissing | FailureLibrary,

    // Defines a failing load state to the inability to resolve a particular
    // symbol from the library in question.
    FailureMissingSymbol  = FailureMissing | FailureSymbol,

    // Anything defined over this is considered to be a user-defined error for
    // plugins.
    FailureUser           = 0x9000
  };

  // Represents a list of Plugin pointers.
  typedef list<Plugin::Ptr> List;

  // Represents a sorted map of Plugin pointers.
  typedef map<string, Plugin::Ptr> Map;

  // Obtains the identifing map of this plugin.
  string name() const;

  // Attempts to start this plugin.
  LoadState start();

  // Attempts to stop this plugin.
  LoadState stop();

  // Obtains the current state of this plugin.
  LoadState state() const;

  // Obtains the most recent loading error.
  LoadFailure loadFailure() const;

  // Loads a plugin by a provided file path.
  static Plugin::Ptr __hot loadByName(const string& name);

  // Loads a plugin by a provided file path.
  static Plugin::Ptr __hot loadFromFilepath(const string& filepath);

  // Unloads a plugin by its name.
  static bool __cold unload(const string& name);

  // Gets a list of all of the loaded plugins.
  static list<string> __hot loadedPlugins();

  // Determines if there's a loaded plugin with the provided name.
  static bool __hot isCurrentlyLoaded(const string& name);

  class WINTERMUTE_EXPORT Library : W_DECLARE_SHAREABLE(Library)
  {
  private:
    W_DEFINE_PRIVATE(Library)

  public:
    W_DECLARE_PTR_TYPE(Library)
    // Creates a new Library with the provided file path.
    explicit Library(const string& libraryPath);

    // Destroys this instance of a library.
    virtual ~Library();

    // Represents a raw function pointer.
    typedef void* FunctionHandlePtr;

    // Obtains the path to the library in question.
    string __hot filePath() const;

    // Whether or not this library has been lazily loaded into the process.
    bool __hot isLoaded() const;

    // Checks if the library exists (for even loading).
    bool __hot exists() const;

    // Obtains and clears the last error message.
    string __hot errorMessage() const;

    // Loads the library from disk.
    bool __cold load();

    // Unloads the library from this application's memory.
    bool __cold unload();

    // Attempts to obtain a function from the library.
    Library::FunctionHandlePtr __hot resolveMethod(const string& methodName) const;

    // Find a library with the provided file path.
    static Library::Ptr __cold find(const string& filepath);
  };

  // Loads a library.
  static Plugin::Ptr __hot loadFromLibrary(Library::Ptr& library);

protected:
  // Default constructor.
  explicit __cold Plugin(const string& name);

  // Handles the work to be done when this plugin is stopped.
  virtual __cold bool shutdown() = 0;

  // Handles the work to be done when this plugin is started.
  virtual __cold bool startup() = 0;
};
}

/*
 * Provides the ctor and dtor methods for this plugin library.
 */
#define W_DEFINE_PLUGIN(Class) \
  extern "C" WINTERMUTE_EXPORT Wintermute::Plugin::Ptr w_plugin_create () { \
    Wintermute::Plugin::Ptr pluginPtr = std::make_shared<Class>(); \
    return pluginPtr; \
  } \
  extern "C" WINTERMUTE_EXPORT bool w_plugin_destroy (Wintermute::Plugin::Ptr& pluginPtr) { \
    wdebug( #Class " count: " + std::to_string(pluginPtr.use_count())); \
    delete pluginPtr->getptr().get(); \
    return (bool) !pluginPtr; \
  }

#endif
