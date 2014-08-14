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

#include "plugin_library.hh"
#include "plugin.hpp"
#include "logging.hpp"
#include <dlfcn.h>

using Wintermute::Plugin;
using Wintermute::LibraryPrivate;

Plugin::Library::Library(const string& filePath) : d_ptr(new LibraryPrivate)
{
}

bool Plugin::Library::load()
{
	W_PRV(Library);
	assert(!d->handlePtr);

	d->handlePtr.reset(dlopen(d->filePath.c_str(), RTLD_LAZY));

	// TODO Do some error handling.
	return (bool) d->handlePtr;
}

bool Plugin::Library::unload()
{
	W_PRV(const Library);
	return !! d->handlePtr;
}

Plugin::Library::FunctionHandlePtr Plugin::Library::resolveMethod(const string& methodName)
{
	return Library::FunctionHandlePtr(nullptr);
}

string Plugin::Library::lastErrorMessage() const
{
	W_PRV(const Library);
	return string();
}

Plugin::Library::~Library()
{
	// TODO Consider some cleanup work here.
}

LibraryPrivate::LibraryPrivate() : filePath()
{
}

LibraryPrivate::~LibraryPrivate()
{
}
