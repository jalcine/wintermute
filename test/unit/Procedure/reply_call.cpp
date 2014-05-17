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
#include <Wintermute/Procedure/MethodCall>
#include <Wintermute/Procedure/ReplyCall>
#include <Wintermute/Procedure/Designation>
#include <WintermuteTestDriver>

using Wintermute::Procedure::Message;
using Wintermute::Procedure::ReplyCall;
using Wintermute::Procedure::MethodCall;
using Wintermute::Procedure::Designation;

#define LOCAL_METHOD_NAME "localFunTime"
#define REMOTE_METHOD_NAME "remoteFunTime"

class TestReplyCall : public ReplyCall
{
  public:
    TestReplyCall(const MethodCall& m, const QVariant& v = QVariant()) :
      ReplyCall(m, v) {}
};

/**
 * The purpose of this test to render the ability to handle calls and its
 * properties with ease.
 */
class ReplyCallUnitTest : public QObject
{
    Q_OBJECT;

    void setWithLocalMethodCall() {
      cleanup();
      localSenderDef = Designation::compose(WINTERMUTE_DOMAIN, "testing",
                                            QCoreApplication::applicationPid());
      remoteReceiverDef = Designation::compose(WINTERMUTE_DOMAIN, "testing",
                          qrand());
      Q_ASSERT ( localSenderDef.valid() == true );
      Q_ASSERT ( remoteReceiverDef.valid() == true );
      localMethodCall = new MethodCall(LOCAL_METHOD_NAME, remoteReceiverDef,
                                       QVariant());
      localMethodCall->setSender(localSenderDef);
      Q_ASSERT ( localMethodCall->valid() == true );
      replyCall = new TestReplyCall(*localMethodCall, QVariant());
      Q_ASSERT ( replyCall->valid() == true );
    }

    void setWithRemoteMethodCall() {
      cleanup();
      localReceiverDef = Designation::compose(WINTERMUTE_DOMAIN, "testing",
                                              qrand());
      remoteSenderDef = Designation::compose(WINTERMUTE_DOMAIN, "testing",
                                             QCoreApplication::applicationPid());
      Q_ASSERT ( remoteSenderDef.valid() == true );
      Q_ASSERT ( localReceiverDef.valid() == true );
      remoteMethodCall = new MethodCall(REMOTE_METHOD_NAME, localReceiverDef,
                                        QVariant());
      remoteMethodCall->setSender(remoteSenderDef);
      Q_ASSERT ( remoteMethodCall->valid() == true );
      replyCall = new TestReplyCall(*remoteMethodCall, QVariant());
      Q_ASSERT ( replyCall->valid() == true );
    }

    Q_SLOT void cleanup() {
      if (remoteMethodCall) {
        delete remoteMethodCall;
        remoteMethodCall = nullptr;
      }

      if (localMethodCall) {
        delete localMethodCall;
        localMethodCall = nullptr;
      }

      if (replyCall) {
        delete replyCall;
        replyCall = nullptr;
      }
    }

    Q_SLOT void isTheNameProvidedFromMethodCall() {
      setWithLocalMethodCall();
      QVERIFY(replyCall->name() == localMethodCall->name());
    }

    Q_SLOT void canDetermineLocalStatusOfCall() {
      setWithLocalMethodCall();
      QVERIFY(replyCall->isLocal() == true);
    }

    Q_SLOT void canDetermineRemoteStatusOfCall() {
      setWithRemoteMethodCall();
      QVERIFY(replyCall->isLocal() == false);
    }

    TestReplyCall* replyCall;
    MethodCall* localMethodCall;
    MethodCall* remoteMethodCall;
    Designation localSenderDef;
    Designation localReceiverDef;
    Designation remoteSenderDef;
    Designation remoteReceiverDef;

  public:
    ReplyCallUnitTest() : QObject(), replyCall(nullptr), localMethodCall(nullptr),
      remoteMethodCall(nullptr), localSenderDef(Designation::Null),
      localReceiverDef(Designation::Null), remoteSenderDef(Designation::Null),
      remoteReceiverDef(Designation::Null) {
    }

    virtual ~ReplyCallUnitTest() {
      cleanup();
    }
};

QTEST_APPLESS_MAIN ( ReplyCallUnitTest )
#include "reply_call.moc"
