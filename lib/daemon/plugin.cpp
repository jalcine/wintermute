 /**
 * vim: ft=cpp
 *
 * Copyright (c) 2013 Jacky Alcine <me@jalcine.me>
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "plugin.hpp"
#include "globals.hpp"
#include "module.hpp"
#include "plugin.moc"

using Wintermute::Daemon::Plugin;
using Wintermute::Daemon::Module;
using Wintermute::Version;

Plugin::Plugin() {
}

QString
Plugin::name() const
{
  return "wintermute-daemon";
}

void
Plugin::stop()
{
}

void
Plugin::start()
{
  // TODO: Add to our heartbeat that this is a daemon.
  module = new Daemon::Module;
}

Version
Plugin::version() const
{
  return Version::fromString(configuration()->value("Version/Plugin").toString());
}

Version
Plugin::systemVersion() const
{
  return Version::fromString(configuration()->value("Version/System").toString());
}

Plugin::State
Plugin::state() const
{
  return Undefined;
}

Plugin::Type
Plugin::type() const
{
  return Module;
}

Q_EXPORT_PLUGIN2(wintermute-daemon, Wintermute::Daemon::Plugin)
