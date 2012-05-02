/***
 *  This file is part of the Wintermute project.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
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
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/02/12 1:23:32 PM
 */
#include <iostream>

#include <QtDebug>
#include <QSettings>
#include <QDateTime>
#include <QVariantMap>
#include <QApplication>
#include <QVariantMap>

#include "ipc.hpp"
#include "config.hpp"
#include "coreprivate.hpp"
#include "core.hpp"

using std::cout;
using std::endl;
using namespace Wintermute;

WINTER_SINGLETON_DEFINE (Core)

CorePrivate::CorePrivate (Core* p_qPtr) : q_ptr (p_qPtr), app (0), args()
{
    setDefaultArguments();
}

void CorePrivate::setDefaultArguments() {
    args.insert(WINTER_COMMAND_LINE_IPC,"root");
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
                QStringList kargs = argumentName.split ("=", QString::KeepEmptyParts);
                argumentName = kargs.first();
                argumentValue = kargs.last();
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

    args.unite(resultingArgs);
}

CorePrivate::~CorePrivate()
{

}

Core::Core() : QObject (qApp->instance()), d_ptr (new CorePrivate (this))
{

}

Core::Core (int& p_argc, char** p_argv) : QObject (qApp->instance()),
    d_ptr (new CorePrivate (this))
{
    configure (p_argc, p_argv);
    start();
}

void Core::configure (int& p_argc, char** p_argv)
{
    CorePrivate* d = instance()->d_func();
    QString ipcMod = "master";
    d->app = new QApplication (p_argc, p_argv);
    d->app->setApplicationName ("Wintermute");
    d->app->setApplicationVersion (QString::number (WINTER_VERSION));
    d->app->setOrganizationDomain ("thesii.org");
    d->app->setOrganizationName ("Synthetic Intellect Institute");
    connect (d->app, SIGNAL (aboutToQuit()), instance(), SLOT (stop()));
    instance()->setParent (d->app);

    d->parseCommandLineArguments();

    if (d->args.count (WINTER_COMMAND_LINE_IPC) != 0)
        ipcMod = d->args.value (WINTER_COMMAND_LINE_IPC).toString();
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

    IPC::System::start();

    if (IPC::System::module() == "master") {
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
    qDebug() << "(core) [" << IPC::System::module () << "] Exitting...";

    if ( (IPC::System::module () != "master" && arguments ().value ("help") == "ignore") && p_closeRootApplication) {
        qDebug() << "(core) [" << IPC::System::module () << "] Closing root appplication...";
        QDBusMessage msg = QDBusMessage::createMethodCall ("org.thesii.Wintermute", "/Master", "org.thesii.Wintermute.Master", "quit");
        QDBusMessage reply = IPC::System::bus ()->call (msg, QDBus::Block);

        if (reply.type () == QDBusMessage::ErrorMessage)
            qDebug() << "(core) [" << IPC::System::module () << "] Can't terminate master module of Wintermute:" << reply.errorName();
    }

    qDebug() << "(core) [" << IPC::System::module () << "] Exitted.";
    QApplication::exit (p_exitCode);
}

void Core::quit()
{
    Core::exit (0);
}

void Core::stop ()
{
    qDebug() << "(core) [" << IPC::System::module() << "] Stopping...";
    IPC::System::stop ();
    emit instance()->stopped ();
    qDebug() << "(core) [" << IPC::System::module() << "] Process stopped.";
}

#include "core.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
