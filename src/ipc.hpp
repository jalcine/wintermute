/***
 *  This file is part of the Wintermute project.
 *
 *  Copyright (C) 2012 Jacky Alciné <me@jalcine.me>
 *
 *  Wintermute is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  Wintermute is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with Wintermute.
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <me@jalcine.me>
 * @date 04/22/12 5:42:03 AM
 */

#ifndef WINTERMUTE_IPC_HPP
#define WINTERMUTE_IPC_HPP

#define WINTER_COMMAND_LINE_IPC         "ipc"
#define WINTER_COMMAND_LINE_IPC_CORE    "root"
#define WINTER_COMMAND_LINE_IPC_PLUGIN  "plugin"

#include <QDBusMessage>
#include <QVariantList>
#include <QVariantMap>
#include <global.hpp>

class QDBusAbstractAdaptor;
class QDBusConnection;

WINTER_FORWARD_DECLARE_CLASS(Core)
WINTER_FORWARD_DECLARE_CLASS(AbstractAdaptor)
WINTER_FORWARD_DECLARE_STRUCT(IPCPrivate)

WINTER_BEGIN_NAMESPACE

/**
 * @brief Represents the Inter Process Communication (IPC) management of Wintermute.
 *
 * This class manages the inter process communication of Wintermute. It handles the
 * incoming and out-coming requests of method calls, and allows Wintermute to fork off into
 * the appropriate processes that represent it. Wintermute abstracts the local system's means of
 * inter process communication (for now, we rely on D-Bus as we've targted Linux-based systems)
 * and allows for the immediate execution of tasks.
 *
 * @section N01 Modules
 *
 * Wintermute can run as one of the following sub-modules:
 * - <b>Master</b>: Represents the core module, or the master daemon that'll be a means
 * of regulating the sub processes. This module is vital to the execution of Wintermute as it
 * makes sure that everyone is playing fair in their process execution.
 *
 * - <b>Plug-in</b>: Represents the plug-in module. Plug-ins are run in their process,
 * allowing them to take advantage of their own work process, and prevents system failure
 * if the plug-in happens to crash. Having a plug-in run in its own process also allows
 * us to work reliably.
 *
 * @see CoreAdaptor, PluginBase, PluginInstance, Factory
 */
class IPC : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (IPC)
    Q_DECLARE_PRIVATE(IPC)
    WINTER_SINGLETON (IPC)
    friend class Core;

signals:
    /**
     * @brief Does the work of adding user data-types (user PODs) as recognizable, marshallable types for QDBusArgument.
     * @fn registerDataTypes
     */
    void registerDataTypes();

public:
    virtual ~IPC();
    /**
     * Starts up the IPC system by storing the type of module that Wintermute's
     * running under and executing the code required to render that module.
     */
    static void start ();

    /**
     * Stops all of the work of the IPC system and then exits Wintermute.
     * @fn stop
     */
    static void stop();

    /**
     * @brief Obtains the current module.
     * @return The name of the running module.
     */
    static const QString module();

    /**
     * @brief Obtains the adaptor being used (most likely the SystemAdaptor).
     * @fn adaptor
     */
    static AbstractAdaptor* localAdaptor();

    static void setLocalAdaptor(AbstractAdaptor* p_adaptor);

    /**
     * @brief Registers an Adaptor onto the current D-Bus bus.
     * @fn registerObject
     * @param QString The name of the Adaptor.
     * @param Adaptor* The Adaptor to be added.
     */
    static bool registerObject (const QString& p_path, QDBusAbstractAdaptor* p_adaptor);

    /**
     * @brief Calls a method in a different module .
     * @fn callMethod
     * @param p_module
     * @param p_path
     * @param p_objectPath
     * @param p_method
     * @param p_variables
     */
    static QDBusMessage callMethod(const QString& p_module, const QString& p_path, const QString& p_objectPath, const QString& p_method, const QVariantList* p_variables = 0);

private:
    static void handleExit();
    QScopedPointer<IPCPrivate> d_ptr;
};

WINTER_END_NAMESPACE

#endif /* IPC_HPP */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
