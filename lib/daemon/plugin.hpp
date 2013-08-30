/**
 * vim: ft=qt.cpp
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

#ifndef WINTERMUTE_DAEMON_PLUGIN_HPP
#define WINTERMUTE_DAEMON_PLUGIN_HPP

#include <Wintermute/Plugin>

namespace Wintermute {
  namespace Daemon {
    class Module;
    class Plugin : public Wintermute::Plugin {
      Q_OBJECT;
      Q_DISABLE_COPY(Plugin);
      Wintermute::Daemon::Module* module;

      public:
      explicit Plugin();
      virtual QString name() const;
      virtual Q_SLOT void start();
      virtual Q_SLOT void stop();
      virtual Version version() const;
      virtual Version systemVersion() const;
      virtual State state() const;
      virtual Type type() const;
    };
  }
}

#endif /* */
