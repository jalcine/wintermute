/***
 *  This file is part of the Wintermute project.
 *
 *  Copyright (C) 2012 Jacky Alciné <jacky.alcine@thesii.org>
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
 * @author Jacky Alciné <jacky.alcine@thesii.org>
 * @date 05/02/12 1:23:32 PM
 */

#include <iostream>

#include <QtDebug>
#include <QStringList>
#include <QSettings>
#include <QDateTime>
#include <QVariantMap>
#include <QApplication>
#include <QVariantMap>

#include "ipc.hpp"
#include "global.hpp"
#include "coreprivate.hpp"
#include "core.hpp"
#include "factory.hpp"

using std::cout;
using std::endl;

WINTER_USE_NAMESPACE

WINTER_SINGLETON_DEFINE (Core)

CorePrivate::CorePrivate (Core* p_qPtr) : q_ptr (p_qPtr), app (0), args()
{
    setDefaultArguments();
}

void CorePrivate::configure (int& p_argc, char** p_argv)
{
    Q_Q (Core);
    app = new QApplication (p_argc, p_argv);
    app->setApplicationName ("Wintermute");
    app->setApplicationVersion (WINTER_VERSION_STR);
    app->setOrganizationDomain ("thesii.org");
    app->setOrganizationName ("Synthetic Intellect Institute");
    q->connect (app, SIGNAL (aboutToQuit()), q, SLOT (stop()));
    q->setParent (app);

    parseCommandLineArguments();
}

void CorePrivate::setDefaultArguments()
{
    args.insert (WINTER_COMMAND_LINE_IPC, "root");
    args.insert (WINTER_COMMAND_LINE_FACTORY_DAEMON, false);
}

void CorePrivate::parseCommandLineArguments()
{
    QStringList rawArgs = app->arguments();
    rawArgs.removeFirst();

    QVariantMap resultingArgs;

    bool isParsingArgumentValue = false;
    QString argumentName = QString::null;
    QVariant argumentValue;

    for (int index = 0; index < rawArgs.length(); ++index) {
        const QString argument = rawArgs.at (index);
        const bool hasArgumentValue = argument.contains ("=");

        if (isParsingArgumentValue) {
            resultingArgs.insert (argumentName, argument);
            isParsingArgumentValue = false;
            argumentName = QString::null;
        }
        else {
            argumentName = QString (argument).replace ("-", "");

            if (hasArgumentValue) {
                QStringList comps = argumentName.split ("=", QString::KeepEmptyParts);
                argumentName = comps.first();
                argumentValue = comps.last();
                resultingArgs.insert (argumentName, argumentValue);
                argumentName = QString::null;
                argumentValue = QVariant();
            }

            isParsingArgumentValue = !hasArgumentValue;
        }

        if (index + 1 < rawArgs.length()) {
            const bool isNextStringArgument = rawArgs.at (index + 1).startsWith ("--") || rawArgs.at (index + 1).startsWith ("-");

            if (isNextStringArgument && !argumentName.isNull()) {
                isParsingArgumentValue = false;
                resultingArgs.insert (argumentName, QVariant());
            }
        }
    }

    args.unite (resultingArgs);
}

CorePrivate::~CorePrivate()
{

}

Core::Core() : QObject (0), d_ptr (new CorePrivate (this))
{

}

void Core::boot (int& p_argc, char** p_argv)
{
    instance()->d_func()->configure (p_argc, p_argv);
    start();
}

QVariantMap Core::arguments()
{
    CorePrivate* d = instance()->d_func();
    return d->args;
}

/// @todo Consider testing whether or not Wintermute's running as a daemon and invoke a sub process.
void Core::start()
{
    CorePrivate* d = instance()->d_func();

    if (Core::arguments().value ("ipc").toString() == "master") {
        cout << qPrintable (d->app->applicationName ()) << " "
             << qPrintable (d->app->applicationVersion ())
             << " (pid " << d->app->applicationPid () << ") :: "
             << "Artificial intelligence for Common Man. (Licensed under the GPL3+)" << endl;
    }

    IPC::start();

    if (IPC::module() == "master") {
        QSettings* settings = new QSettings ("Synthetic Intellect Institute", "Wintermute");
        QDateTime lstDate = settings->value ("Statistics/StartupDate", QDateTime::currentDateTime()).toDateTime();
        settings->setValue ("Statistics/StartupDate", QDateTime::currentDateTime());
        qDebug() << "(core) Last startup was at" << lstDate.toLocalTime().toString();
    }

    emit instance()->started();
    qDebug() << "(core) [Core] Started.";
}

void Core::exit (const int p_exitCode, const bool p_closeRootApplication)
{
    qDebug() << "(core) [" << IPC::module () << "] Exitting Wintermute's main event loop...";

    if (p_closeRootApplication) {
        IPC::handleExit();
    }

    qDebug() << "(core) [" << IPC::module () << "] Exited Wintermute's main event loop'.";
    QApplication::exit (p_exitCode);
}

void Core::quit()
{
    Core::exit (WINTER_EXITCODE_NORMAL);
}

void Core::stop ()
{
    qDebug() << "(core) [" << IPC::module() << "] Stopping Wintermute's core...";
    IPC::stop ();
    emit instance()->stopped ();
    qDebug() << "(core) [" << IPC::module() << "] Core of Wintermute stopped.";
}

#include "core.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
