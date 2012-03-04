/**
 * @file backend.hpp
 * @author Wintermute Developement <wntr-devel@thesii.org>
 *
 * @legalese
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * @endlegalese
 */

// Local
#include "plugins.hpp"

// Qt
#include <QMap>
#include <QStringList>
#include <QObject>

namespace Wintermute {
namespace Backends {
class AbstractFramework;
class AbstractBackend;

typedef QMap<QString, AbstractBackend *> BackendList;
typedef QMap<QString, AbstractFramework *> FrameworkList;

using Wintermute::Plugins::AbstractPlugin;

class AbstractFramework : public QObject {
    Q_OBJECT
    Q_CLASSINFO ( "objectName","Abstract Framework" )
    Q_DISABLE_COPY ( AbstractFramework )
    Q_PROPERTY ( const StartupMode Mode READ startMode WRITE setStartMode )

signals:
    void started();
    void stopped();

    void added();
    void added ( AbstractBackend * );

    void removed();
    void removed ( AbstractBackend * );

public:
    enum StartupMode {
        Configuration = 0,
        Manual
    };

    /**
     * @brief Constructor of an AbstractPlugin and QObject
     * @param p Pointer to the AbstractPlugin.
     * @param parent Parent of the object.
     */
    AbstractFramework ( AbstractPlugin* = Plugins::Factory::currentPlugin(), QObject* = 0 );

    /**
     * @brief Destructor
     */
    virtual ~AbstractFramework();

    /**
     * @brief Add new backend to the framework
     * @param p Pointer to the backend.
     */
    void addBackend ( AbstractBackend * );

    /**
     * @brief Remove backend from the framework
     * @param p Pointer to the backend.
     */
    void removeBackend ( AbstractBackend * );

    /**
     * @brief Make the chosen backend default one
     * @param p Pointer to the backend.
     */
    void setDefaultBackend ( AbstractBackend * );

    /**
     * @brief Set start mode for the framework
     * @param mode Mode in which framework should start.
     */
    void setStartMode ( const StartupMode& );

    /**
     * @brief Check if given backend is listed
     * @param p Pointer to the backend.
     */
    bool isBackendListed ( const AbstractBackend * ) const;
    bool isBackendListed ( const QString& ) const;

    /**
     * @brief Return pointer to the default backend
     */
    QList<AbstractBackend *> defaultBackend() const;

    /**
     * @brief Return startup mode of the framework
     * @returns Startup mode of the framework.
     */
    const StartupMode& startMode() const;

    /**
     * @brief Get pointer to the framework identified by the name
     * @param id QString identifier of the framework.
     * @returns Pointer to the framework from FrameworkList.
     */
    static AbstractFramework* obtainFramework ( const QString& );

    /**
     * @brief Get the number of frameworks
     * @returns Number of frameworks.
     */
    static int frameworks();

public slots:
    void start();
    void stop();

protected:
    BackendList m_bckndLst;
    QStringList m_dfltBcknd;
    StartupMode m_strtMd;
    AbstractPlugin* m_plgn;
    virtual void initialize() = 0;
    virtual void deinitialize() = 0;

private:
    static FrameworkList s_frmk;
};

class AbstractBackend : public QObject {
    Q_OBJECT
    Q_CLASSINFO ( "objectName","Abstract Backend" )
    Q_PROPERTY ( const QString ID READ id )
    Q_PROPERTY ( const bool Active READ isActive )
    friend class AbstractFramework;

signals:
    void started();
    void stopped();

public:
    /**
     * @brief Constructor
     */
    AbstractBackend ( AbstractPlugin *, QObject* = 0 );

    /**
     * @brief Destructor
     */
    virtual ~AbstractBackend();

    /**
     * @brief Get id of the backend
     * @returns Id of the backend.
     */
    virtual QString id() const;

    /**
     * @brief Check if backend is active
     */
    virtual bool isActive() const = 0;

    /**
     * @brief Get pointer to the backend with id
     * @param id Id of the backend.
     * @returns Pointer to the backend with given id.
     */
    static AbstractBackend* obtainBackend ( const QString& );

public slots:
    virtual void start();
    virtual void stop();

protected:
    QString m_id;
    AbstractPlugin* m_plgn;
    virtual void initialize() = 0;
    virtual void deinitialize() = 0;

private:
    static BackendList s_lst;
};

} // namespaces
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
