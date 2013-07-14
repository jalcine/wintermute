#include "Wintermute/Procedure/call.hpp"
#include "Wintermute/private/Procedure/call.hpp"

using Wintermute::Procedure::Call;
using Wintermute::Procedure::CallPrivate;

Call::Call(QObject* callee) : QObject(callee),
  d_ptr(new CallPrivate(this)) {
  // TODO: Consider attaching signal to parent automatically.
}

const QVariantList& Call::arguments() const {
  Q_D(const Call);
  return d->arguments;
}

void Call::setArguments(const QVariantList& arguments) {
  Q_D(Call);
  d->arguments = arguments;
}

const Call::CallingMethod& Call::callingMethod() const {
  Q_D(const Call);
  return d->callMethod;
}

void Call::setCallingMethod(const CallingMethod& callMethod) {
 Q_D(Call);
 d->callMethod = callMethod;
}

void Call::setRemoteObject(const QString& name){
  Q_D(Call);
  if (!name.isEmpty())
    d->remoteObjectName = name;
}

void Call::setCallName(const QString& callName){
  Q_D(Call);
  if (!callName.isEmpty())
    d->remoteObjectMethod = callName;
}

QObject* Call::invoke(const QString& remoteObjectName, const QString& remoteObjectMethod){
  Q_D(Call);
  QString name = remoteObjectName, method = remoteObjectMethod;
  if (name.isEmpty())   name = d->remoteObjectName;
  if (method.isEmpty()) method = d->remoteObjectMethod;

  switch (this->callingMethod()){
    case CallingMethod::Background:
      // TODO: Require QBackgroundTask to be implemented.
      break;

    case CallingMethod::Discard:
      break;

    case CallingMethod::Block:
      break;
  }

  return 0;
}

Call::~Call() { }

#include "Wintermute/Procedure/call.moc"
