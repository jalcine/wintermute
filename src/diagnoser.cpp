// Local
#include "diagnoser.hpp"

// Qt
#include <QString>

using namespace Wintermute::Diagnostics;

QMap<QString, Diagnoser*> Diagnoser::diagnosers = QMap<QString, Diagnoser*>();

Diagnoser::Diagnoser(QObject *parent) : QObject(parent)
{
}

Diagnoser::Diagnoser(QString& newLabel, bool autoRegister, QObject *parent)
    : label(newLabel), QObject(parent)
{
    if (autoRegister)
        Diagnoser::registerDiagnoser(this);
}

Diagnoser::~Diagnoser()
{
}

QString Diagnoser::getLabel()
{
    return label;
}

void Diagnoser::registerDiagnoser(Diagnoser *pDiag)
{
    Diagnoser::diagnosers.insert(pDiag->getLabel(), pDiag);
}

void Diagnoser::removeDiagnoser(QString& label)
{
    Diagnoser *pDiag = Diagnoser::diagnosers.value(label);
    Diagnoser::diagnosers.remove(label);
    // By specification no diagnoser can exist beyond this register.
    delete pDiag;
}

Diagnoser* Diagnoser::getDiagnoser(QString& label)
{
    return Diagnoser::diagnosers.value(label);
}
