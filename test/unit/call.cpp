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

#include <Wintermute/Procedure/Call>
#include <Wintermute/Procedure/Module>
#include <WintermuteTestDriver>

using Wintermute::Procedure::Call;
using Wintermute::Procedure::Module;

class TestCall : public Call
{
  friend class CallUnitTest;
  TestCall(const QString& name) : Call(name) { }
};

/**
 * The purpose of this test to render the ability to handle calls and its
 * properties with ease.
 */
class CallUnitTest : public QObject
{
  Q_OBJECT;
  TestCall* aCall;
  Module::Definition senderDef;
  Module::Definition receiverDef;


  Q_SLOT void init()
  {
    aCall = new TestCall("FOO");
  }

  void buildWithBoth()
  {
    aCall->setSender(senderDef);
    aCall->setReceiver(receiverDef);
  }

  Q_SLOT void isTheNameProvided()
  {
    buildWithBoth();
    QVERIFY(aCall->name() == "FOO");
    QVERIFY(aCall->valid());
  }

  Q_SLOT void isTheSenderProvided()
  {
    buildWithBoth();
    QVERIFY(aCall->sendingModule() == senderDef);
    QVERIFY(aCall->valid());
  }

  Q_SLOT void jsonHasSender()
  {
    buildWithBoth();
    const QString callStr = aCall->toString().replace("\\\"","\"");
    qDebug() << callStr;
    QVERIFY(aCall->valid());
    QVERIFY(callStr.contains(static_cast<QString>(senderDef)));
  }

  Q_SLOT void jsonHasReceiver()
  {
    buildWithBoth();
    const QString callStr = aCall->toString().replace("\\\"","\"");
    qDebug() << callStr;
    QVERIFY(aCall->valid());
    QVERIFY(callStr.contains(static_cast<QString>(receiverDef)));
  }

  Q_SLOT void cleanup()
  {
    delete aCall;
    aCall = nullptr;
  }

public:
  CallUnitTest() : QObject()
  {
    senderDef = Module::Definition::compose(WINTERMUTE_DOMAIN, "testing", QCoreApplication::applicationPid());
    receiverDef = Module::Definition::compose(WINTERMUTE_DOMAIN".remote", "testing");
  }

  virtual ~CallUnitTest()
  {
    if (aCall != nullptr)
    {
      delete aCall;
      aCall = nullptr;
    }
  }
};

QTEST_APPLESS_MAIN ( CallUnitTest )
#include "call.moc"
