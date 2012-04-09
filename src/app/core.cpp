/**
 * @author Wintermute Development <wntr-devel@thesii.org>
 *
 * @section lcns Licensing
 * This library is free software; you can redistribute it and/or
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

// Standard
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>

// Qt
#include <QtDebug>
#include <QtDebug>
#include <QProcess>
#include <QSettings>
#include <QDateTime>
#include <QVariantMap>
#include <QLibraryInfo>
#include <QApplication>
#include <QSocketNotifier>

#include "app/ipc.hpp"
#include "app/plugins.hpp"
#include "app/core.hxx"
#include "app/core.hpp"

using namespace std;
using namespace Wintermute;

WINTER_SINGLETON_DEFINE(Core)

Core::Core() : QObject (qApp->instance()), d_ptr (new CorePrivate)
{

}

Core::Core (int& p_argc, char** p_argv) : QObject (qApp->instance()),
    d_ptr (new CorePrivate)
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
    d->app->setApplicationVersion (QString::number (WNTR_VERSION));
    d->app->setOrganizationDomain ("thesii.org");
    d->app->setOrganizationName ("Synthetic Intellect Institute");
    connect (d->app, SIGNAL (aboutToQuit()), instance(), SLOT (stop()));
    instance()->setParent (d->app);

    d->configureCommandLine();

    if (d->args.count ("ipc") != 0)
        ipcMod = d->args.value ("ipc").toString();
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
