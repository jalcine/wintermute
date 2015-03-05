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

#include <wintermutecore/plugin.hpp>

namespace Wintermute
{
class SamplePlugin : public Plugin
{
public:
  explicit SamplePlugin();
  virtual ~SamplePlugin();
  virtual bool startup();
  virtual bool shutdown();
  virtual Plugin::PluginType type() const;
};
}

W_DECL_PLUGIN(Wintermute::SamplePlugin, WINTERMUTE_VERSION)

extern "C" int w_sample_test();
