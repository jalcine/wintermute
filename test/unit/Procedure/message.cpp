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

#include <Wintermute/Procedure/Message>
#include <Wintermute/Procedure/Module>
#include <WintermuteTestDriver>

using Wintermute::Procedure::Message;
using Wintermute::Procedure::Module;

class TestMessage : public Message
{
    friend class MessageUnitTest;
    TestMessage() : Message() { }
};

class MessageUnitTest : public QObject
{
    Q_OBJECT;
    TestMessage* aMessage;
    Module::Definition senderDef;
    Module::Definition receiverDef;


    Q_SLOT void init() {
      aMessage = new TestMessage();
    }

    void buildWithBoth() {
      aMessage->setSender(senderDef);
      aMessage->setReceiver(receiverDef);
    }

    Q_SLOT void jsonHasSender() {
      buildWithBoth();
      const QString messageString = aMessage->toString().replace("\\\"","\"");
      QVERIFY(aMessage->valid());
      QVERIFY(messageString.contains(static_cast<QString>(senderDef)));
    }

    Q_SLOT void jsonHasReceiver() {
      buildWithBoth();
      const QString messageString = aMessage->toString().replace("\\\"","\"");
      QVERIFY(aMessage->valid());
      QVERIFY(messageString.contains(static_cast<QString>(receiverDef)));
    }

    Q_SLOT void cleanup() {
      delete aMessage;
      aMessage = nullptr;
    }

  public:
    MessageUnitTest() : QObject() {
      senderDef = Module::Definition::compose(WINTERMUTE_DOMAIN, "testing",
                                              QCoreApplication::applicationPid());
      receiverDef = Module::Definition::compose(WINTERMUTE_DOMAIN".remote",
                    "testing");
    }

    virtual ~MessageUnitTest() {
      if (aMessage != nullptr) {
        delete aMessage;
        aMessage = nullptr;
      }
    }
};

QTEST_APPLESS_MAIN ( MessageUnitTest )
#include "message.moc"
