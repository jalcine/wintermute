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

#include <QtCore/QFile>
#include <WintermuteTestDriver>
#include <Wintermute/Application>
#include <Wintermute/Plugin>

class TestPlugin : public Wintermute::Plugin
{
  Q_OBJECT;
  Q_CLASSINFO("Name", "TestPlugin");

  public:
  explicit TestPlugin() : Plugin() {
    configuration()->setValue("Magic/Option",3);
    configuration()->setValue("Gray/Skull","ThePowerOfGraySkull");
  }

  virtual void start() {}
  virtual void stop() {}

  virtual ~TestPlugin()
  {
    QFile theSettings(configuration()->fileName());
    if (theSettings.exists())
    {
      QVERIFY(theSettings.remove());
      QVERIFY(!theSettings.exists());
    }
  }


  using Plugin::configuration;
};

class TestVersionPlugin : public TestPlugin
{
  Q_OBJECT;
  Q_CLASSINFO("Name", "TestPlugin");
  Q_CLASSINFO("Version", "0.0.1");
  Q_CLASSINFO("System Version", "0.0.1");

  public:
  TestVersionPlugin() : TestPlugin() {}
};

class PluginUnitTest : public QObject
{
  Q_OBJECT;
  TestPlugin* testPlugin;

  Q_SLOT void cleanup()
  {
    if (testPlugin) {
      delete testPlugin;
      testPlugin = nullptr;
    }
  }

  Q_SLOT void manipulateSettings()
  {
    testPlugin = new TestPlugin;
    QSettings* settings = testPlugin->configuration();
    QVERIFY(settings->value("Magic/Option").toInt() == 3);
    QVERIFY(settings->value("Gray/Skull").toString() == "ThePowerOfGraySkull");
  }

  Q_SLOT void versionChecking()
  {
    testPlugin = new TestVersionPlugin;
    QVERIFY(testPlugin->version().toString() == WINTERMUTE_VERSION); 
    QVERIFY(testPlugin->systemVersion().toString() == WINTERMUTE_VERSION); 
  }

};

QTEST_APPLESS_MAIN ( PluginUnitTest )
#include "plugin.moc"
