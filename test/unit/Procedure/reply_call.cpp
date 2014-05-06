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

#include <Wintermute/Procedure/ReplyCall>
#include <Wintermute/Procedure/Module>
#include <WintermuteTestDriver>

using Wintermute::Procedure::ReplyCall;
using Wintermute::Procedure::Module;

class TestReplyCall : public ReplyCall
{
    friend class ReplyCallUnitTest;
    TestReplyCall(const QString& name) : ReplyCall(Call::Null, QVariant()) { }
};

/**
 * The purpose of this test to render the ability to handle calls and its
 * properties with ease.
 */
class ReplyCallUnitTest : public QObject
{
    Q_OBJECT;
    TestReplyCall* aReplyCall;
    Module::Definition senderDef;
    Module::Definition receiverDef;

    Q_SLOT void init() {
      aReplyCall = new TestReplyCall("FOO");
    }

    void buildWithBoth() {
      aReplyCall->setSender(senderDef);
      aReplyCall->setReceiver(receiverDef);
    }

    Q_SLOT void cleanup() {
      delete aReplyCall;
      aReplyCall = nullptr;
    }

    Q_SLOT void isTheNameProvided() {
      buildWithBoth();
      QVERIFY(aReplyCall->valid());
      QVERIFY(aReplyCall->name() == "FOO");
    }


  public:
    ReplyCallUnitTest() : QObject() {
      senderDef = Module::Definition::compose(WINTERMUTE_DOMAIN, "testing",
                                              QCoreApplication::applicationPid());
      receiverDef = Module::Definition::compose(WINTERMUTE_DOMAIN".remote",
                    "testing");
    }

    virtual ~ReplyCallUnitTest() {
      if (aReplyCall != nullptr) {
        delete aReplyCall;
        aReplyCall = nullptr;
      }
    }
};

QTEST_APPLESS_MAIN ( ReplyCallUnitTest )
#include "reply_call.moc"
