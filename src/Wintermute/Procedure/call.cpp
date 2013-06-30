#include "Wintermute/Procedure/call.hpp"
#include "Wintermute/private/Procedure/call.hpp"

using Wintermute::Procedure::Call;
using Wintermute::Procedure::CallPrivate;

Call::Call(QObject* callee) : QObject(callee),
  d_ptr(){
  // TODO: Consider attaching signal to parent automatically.
}

void Call::setArguments(const QVariantList& arguments) {
  Q_D(Call);
  d->arguments = arguments;
}

void Call::setCallingMethod(const CallingMethod& callMethod) {
 Q_D(Call);
 d->callMethod = callMethod;
}

void Call::setRemoteObject(const QString& name, const QString& method){
  Q_D(Call);
  if (!name.isEmpty())
    d->remoteObjectName = name;

  if (!method.isEmpty())
    d->remoteObjectMethod = method;
}

QObject* Call::invoke(const QString& remoteObjectName, const QString& remoteObjectMethod){
  Q_D(Call);
  QString name = remoteObjectName, method = remoteObjectMethod;
  if (name.isEmpty())   name = d->remoteObjectName;
  if (method.isEmpty()) method = d->remoteObjectMethod;

  // TODO: Invoke the calling logic here.
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
