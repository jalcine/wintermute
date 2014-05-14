/**
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

#include <Wintermute/Procedure/Designation>
#include <WintermuteTestDriver>

using Wintermute::Procedure::Module;

const QString[] domains = { "google.com", "facebook.com", "jalcine.me" };
const QString[] packages = { "google.com", "facebook.com", "jalcine.me" };

class TestModuleDefinition : public Module::Definition
{
    friend class ModuleDefinitionUnitTest;
    TestModuleDefinition() : Module::Definition() {
      this->domain = domain;
    }
};

/**
 * The purpose of this test to render the ability to handle calls and its
 * properties with ease.
 */
class ModuleDefinitionUnitTest : public QObject
{
    Q_OBJECT;
    TestModuleDefinition* aDefPtr;

    Module::Definition aDef;

    Q_SLOT void init() {
      aDefPtr = new TestModuleDefinition();
    }

    Q_SLOT void cleanup() {
      delete aDefPtr;
      aDefPtr = nullptr;
    }

    Q_SLOT void checkEquality() {
      // FIXME: Test equality.
    }

  public:
    ModuleDefinitionUnitTest() : QObject() {
    }

    virtual ~ModuleDefinitionUnitTest() {
    }
};

QTEST_APPLESS_MAIN ( ModuleDefinitionUnitTest )
#include "module_def.moc"
