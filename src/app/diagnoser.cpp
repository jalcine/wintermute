// Local
#include "diagnoser.hpp"

// Qt
#include <QString>

using namespace Wintermute::Diagnostics;

QMap<QString, Diagnoser*> Diagnoser::diagnosers = QMap<QString, Diagnoser*>();

Diagnoser::Diagnoser ( QObject *parent ) : QObject ( parent ) {
}

Diagnoser::Diagnoser ( QString& p_newLabel, bool p_autoRegister, QObject *p_parent )
    : QObject ( p_parent ), label ( p_newLabel ) {
    if ( p_autoRegister )
        Diagnoser::registerDiagnoser ( this );
}

Diagnoser::~Diagnoser() {
}

QString Diagnoser::getLabel() {
    return label;
}

void Diagnoser::registerDiagnoser ( Diagnoser *pDiag ) {
    Diagnoser::diagnosers.insert ( pDiag->getLabel(), pDiag );
}

void Diagnoser::removeDiagnoser ( QString& label ) {
    Diagnoser *pDiag = Diagnoser::diagnosers.value ( label );
    Diagnoser::diagnosers.remove ( label );
    // By specification no diagnoser can exist beyond this register.
    delete pDiag;
}

Diagnoser* Diagnoser::getDiagnoser ( QString& label ) {
    return Diagnoser::diagnosers.value ( label );
}

#include "diagnoser.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
