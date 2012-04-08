/**
 * @file pluginhandle.hpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 *
 * @section lcns Licensing
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
 *
 */

#ifndef _PLUGINHANDLE_HPP_
#define _PLUGINHANDLE_HPP_

// Qt
#include <QString>
#include <QSettings>
#include <QProcess>

namespace Wintermute
{
namespace Plugins
{

// Forward declarations
class Factory;

/**
 * @brief Provides the Factory with a controllable instnace of a plugin.
 *
 * @see Factory, AbstractPlugin
 */
class PluginHandle : public QObject
{
    friend class Factory;

    Q_OBJECT
    Q_DISABLE_COPY (PluginHandle)
    Q_PROPERTY (const bool Active READ isActive)
    Q_PROPERTY (const QString Name READ name)

private:
    const QString m_uuid;
    QProcess* m_prcss;
    QSettings* m_settings;
    void doCrashed (const QString&);
    void doLoaded (const QString&);
    void doUnloaded (const QString&);

public:
    /**
     * @brief Null constructor.
     */
    PluginHandle();

    virtual ~PluginHandle();

    /**
     * @brief Default constructor.
     * @param p_uuid The UUID of the plug-in.
     * @param p_stgs The QSettings of the plug-in.
     */
    explicit PluginHandle (const QString&, QSettings*);

    /**
     * @brief Determines if the plug-in's currently active.
     */
    bool isActive();

    /**
     * @brief Obtains the UUID of the captured plug-in.
     */
    QString uuid();

    /**
     * @brief Obtains the friendly name of the captured plug-in.
     */
    QString name();

    /**
     * @brief Obtains the representative QSettings of the plug-in.
     */
    QSettings* settings();

signals:
    /**
     * @brief Emitted when the process of the plug-in experiences a crash.
     * @fn crashed
     */
    void crashed();
    void crashed (const QString&);

    /**
     * @brief Emitted when the process of the plug-in starts.
     * @fn started
     */
    void started();
    void started (const QString&);

    /**
     * @brief Emitted when the process of the plug-in stops.
     * @fn stopped
     */
    void stopped();
    void stopped (const QString&);

public slots:

    /**
     * @brief Halts the plug-in's process.
     */
    void stop();

    /**
     * @brief Starts the plug-in's process.
     */
    void start();

private slots:
    void catchStart();
    void catchError (const QProcess::ProcessError&);
    void catchExit (int, const QProcess::ExitStatus&);
};

} // namespaces
}

#endif // _PLUGINHANDLE_HPP_
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
