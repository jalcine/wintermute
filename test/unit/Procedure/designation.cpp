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

#include <QtCore/QStringList>
#include <WintermuteTestDriver>
#include <Wintermute/Procedure/Designation>

const QStringList domains = QStringList() << "google.com" << "facebook.com" << "jalcine.me";
const QStringList packages = QStringList() << "google.com" << "facebook.com" << "jalcine.me";

using Wintermute::Procedure::Designation;

class TestModuleDefinition : public Designation
{
    friend class ModuleDefinitionUnitTest;
    TestModuleDefinition() : Designation() {
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

    Designation aDef;

    Q_SLOT void init() {
      aDefPtr = new TestModuleDefinition();
    }

    Q_SLOT void cleanup() {
      delete aDefPtr;
      aDefPtr = nullptr;
    }

    Q_SLOT void checkEquality() {
      Designation dsg1 = Designation::compose("me.jalcine", "testing", 300);
      Designation dsg2 = Designation::compose("me.jalcine", "testing", 300);
      QVERIFY(dsg1 == dsg2);
    }

    Q_SLOT void checkInequality() {
      Designation dsg1 = Designation::compose("me.jalcine", "testing", 300);
      Designation dsg2 = Designation::compose("me.jalcine", "testing", 300);
      Designation dsg3 = Designation::compose("me.jalcine", "broken", 300);
      QVERIFY(dsg2 != dsg3);
      QVERIFY(dsg1 != dsg3);
    }

    Q_SLOT void checkInlocality() {
      Designation dsg1 = Designation::compose("me.jalcine", "testing", QCoreApplication::applicationPid() + 4);
      QVERIFY(dsg1.isLocal() == false);
    }

    Q_SLOT void checkLocality() {
      Designation dsg1 = Designation::compose("me.jalcine", "testing", QCoreApplication::applicationPid());
      QVERIFY(dsg1.isLocal() == true);
    }

  public:
    ModuleDefinitionUnitTest() : QObject() {
    }

    virtual ~ModuleDefinitionUnitTest() {
    }
};

QTEST_APPLESS_MAIN ( ModuleDefinitionUnitTest )
#include "designation.moc"
