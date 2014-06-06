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

#include <Wintermute/Procedure/LambdaCall>
#include <Wintermute/Procedure/Module>
#include <Wintermute/Procedure/Designation>
#include <WintermuteTestDriver>

using Wintermute::Procedure::LambdaCall;
using Wintermute::Procedure::Module;
using Wintermute::Procedure::Designation;

class TestLambdaCall : public LambdaCall
{
    friend class LambdaCallUnitTest;
    TestLambdaCall(const QString& name) : LambdaCall(name) { }
};

class LambdaCallUnitTest : public QObject
{
    Q_OBJECT;
    TestLambdaCall* aLambdaCall;
    Designation senderDef;
    Designation receiverDef;

    Q_SLOT void init() {
      aLambdaCall = new TestLambdaCall("FOO");
    }

    void buildWithBoth() {
      aLambdaCall->setSender(senderDef);
      aLambdaCall->setReceiver(receiverDef);
    }

    Q_SLOT void cleanup() {
      delete aLambdaCall;
      aLambdaCall = nullptr;
    }

    Q_SLOT void isTheNameProvided() {
      buildWithBoth();
      QVERIFY(aLambdaCall->valid());
      QVERIFY(aLambdaCall->name() == "FOO");
    }

  public:
    LambdaCallUnitTest() : QObject() {
      senderDef = Designation::compose(WINTERMUTE_DOMAIN, "testing",
                                       QCoreApplication::applicationPid());
      receiverDef = Designation::compose(WINTERMUTE_DOMAIN".remote",
                                         "testing");
    }

    virtual ~LambdaCallUnitTest() {
      if (aLambdaCall != nullptr) {
        delete aLambdaCall;
        aLambdaCall = nullptr;
      }
    }
};

QTEST_APPLESS_MAIN ( LambdaCallUnitTest )
#include "call.moc"
