#ifndef _DIAGNOSER_HPP_
#define _DIAGNOSER_HPP_

// Qt
#include <QObject>
#include <QString>
#include <QMap>

namespace Wintermute
{
namespace Diagnostics
{

/**
 * @brief Base diagnoser class
 * This class represents one of major WCore's components: diagnostics performer
 * & reporter. Its role is to provide statistics about system's vitality and
 * its used for reporting and self-assesment.
 */
class Diagnoser : public QObject
{
    Q_OBJECT

private:
    static QMap<QString, Diagnoser*> diagnosers; /// Other diagnosers, responsible mainly for plug-ins.

protected:
    QString label;

public:
    /**
     * @brief ...
     *
     * @param  ... Defaults to 0.
     **/
    Diagnoser (QObject* = 0);

    /**
     * @brief Construct a diagnoser using label
     * @param newLabel Diagnoser's label.
     * @param autoRegister If true, automatically registers itself.
     * @param parent QObject parent.
     */
    Diagnoser (QString&, bool = true, QObject* = 0);

    virtual ~Diagnoser();

    /**
     * @brief Get diagnoser's label
     * @returns Label of a diagnoser.
     */
    QString getLabel();

    /**
     * @brief Register new diagnoser
     * @param pDiag Pointer to the diagnoser.
     */
    static void registerDiagnoser (Diagnoser*);

    /**
     * @brief Remove diagnoser
     * @param label Label of a diagnoser you want to remove.
     */
    static void removeDiagnoser (QString&);

    /**
     * @brief Get pointer to a diagnoser with given label
     * @param label Label of the wanted diagnoser.
     * @returns Pointer to the diagnoser with the given label.
     */
    static Diagnoser* getDiagnoser (QString&);
};
} // namespaces
}
#endif // _DIAGNOSER_HPP_
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
