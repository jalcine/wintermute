/*
 * @author Jacky Alciné <me@jalcine.me>
 * @copyright © 2014 Jacky Alciné <me@jalcine.me>
 * @if 0
 * Wintermute is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
 * @endif
 **/

#include <WintermuteTestDriver>
#include <Wintermute/Application>
#include <Wintermute/Factory>
#include <Wintermute/PluginProcess>

using Wintermute::Factory;

/*
 * The purpose of this test is to ensure that the Factory serves its purpose at
 * attempting to load and render plugins for Wintermute. In order for this to
 * efficently work, we add an extra path at which the built plug-in libraries
 * are to be found.
 */
class FactoryUnitTest : public QObject
{
  Q_OBJECT;

  Q_SLOT void init() {}

  Q_SLOT void cleanup() {}

  Q_SLOT void loadPlugin()
  {
#ifndef FACTORY_REFACTOR
    const bool loadedPlugin = wntrFactory->loadPlugin("sample");
    QVERIFY(loadedPlugin == true);
    const Plugin::Ptr thePlugin = wntrFactory->plugin("sample");
    QVERIFY(thePlugin.isNull() == false);
#else
    SKIP_NOT_IMPLEMENTED
#endif
  }

  Q_SLOT void spawnPlugin()
  {
#ifndef FACTORY_REFACTOR
    const PluginProcess::Ptr thePlugin = wntrFactory->spawnPlugin("sample");
    QVERIFY(thePlugin.isNull() == false);
#else
    SKIP_NOT_IMPLEMENTED
#endif
  }

  Q_SLOT void killPlugin()
  {
#ifndef FACTORY_REFACTOR
    const bool loadedPlugin = wntrFactory->loadPlugin("sample");
    QVERIFY(loadedPlugin == true);
    const Plugin::Ptr thePlugin = wntrFactory->plugin("sample");
    QVERIFY(thePlugin.isNull() == false);
    const bool unloadedPlugin = wntrFactory->unloadPlugin("sample");
    QVERIFY(unloadedPlugin == true);
#else
    SKIP_NOT_IMPLEMENTED
#endif
  }

  Q_SLOT void despawnPlugin()
  {
#ifndef FACTORY_REFACTOR
    const PluginProcess::Ptr thePlugin = wntrFactory->spawnPlugin("sample");
    QVERIFY(thePlugin.isNull() == false);
    QVERIFY(thePlugin->running() == false);
    thePlugin->start();
    QVERIFY(thePlugin->running() == true);
    thePlugin->stop();
    QVERIFY(thePlugin->running() == false);
#else
    SKIP_NOT_IMPLEMENTED
#endif
  }
};

QTEST_APPLESS_MAIN ( FactoryUnitTest )
#include "factory.moc"
