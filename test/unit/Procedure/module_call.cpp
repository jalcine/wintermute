// vim: ft=cpp tw=78 fdl=0:
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
 */

#include <Wintermute/Procedure/MethodCall>
#include <Wintermute/Procedure/ModuleCall>
#include <Wintermute/Procedure/Module>
#include <Wintermute/Procedure/Designation>
#include <WintermuteTestDriver>

using Wintermute::Procedure::MethodCall;
using Wintermute::Procedure::ModuleCall;
using Wintermute::Procedure::Module;
using Wintermute::Procedure::Designation;

class TestModule : public Module {
	public:
		TestModule(QObject* p) : Module(p){ }
		virtual void start() { }
		virtual void stop() { }
};

class TestModuleCall : public ModuleCall
{
    friend class ModuleCallUnitTest;
    TestModuleCall ( const QString& name ) : ModuleCall(name, new TestModule()) { }

  public:
    virtual void invoke ( const QVariant& arguments, const MethodCall& call ) {
      // TODO What to?
    }
};

class ModuleCallUnitTest : public QObject
{
    Q_OBJECT;
    TestModuleCall* aModuleCall;
    Designation senderDef;
    Designation receiverDef;

    Q_SLOT void init() {
      aModuleCall = new TestModuleCall("FOO");
    }

    void buildWithBoth() {
      aModuleCall->setSender(senderDef);
      aModuleCall->setReceiver(receiverDef);
    }

    Q_SLOT void cleanup() {
      delete aModuleCall;
      aModuleCall = nullptr;
    }

    Q_SLOT void isTheNameProvided() {
      buildWithBoth();
      QVERIFY(aModuleCall->valid());
      QVERIFY(aModuleCall->name() == "FOO");
    }

  public:
    ModuleCallUnitTest() : QObject() {
      senderDef = Designation::compose(WINTERMUTE_DOMAIN, "testing",
                                       QCoreApplication::applicationPid());
      receiverDef = Designation::compose(WINTERMUTE_DOMAIN".remote",
                                         "testing");
    }

    virtual ~ModuleCallUnitTest() {
      if (aModuleCall != nullptr) {
        delete aModuleCall;
        aModuleCall = nullptr;
      }
    }
};

QTEST_APPLESS_MAIN ( ModuleCallUnitTest )
#include "module_call.moc"
