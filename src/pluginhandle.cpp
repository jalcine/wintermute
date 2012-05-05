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
 * @date 04/22/12 5:43:06 AM
 */

#include <QDebug>
#include <QApplication>

#include "ipc.hpp"
#include "factory.hpp"
#include "pluginhandle.hpp"
#include "pluginhandleprivate.hpp"

WINTER_USE_NAMESPACE

PluginHandlePrivate::PluginHandlePrivate (PluginHandle* p_qPtr) : q_ptr (p_qPtr),
    uuid(), process (0), settings (0)
{

}

PluginHandlePrivate::~PluginHandlePrivate()
{
}

PluginHandle::PluginHandle (const PluginHandle& p_other) : QObject (p_other.parent()),
    d_ptr (p_other.d_ptr.data())
{

}

PluginHandle::PluginHandle (const QString& p_uuid, QSettings* p_settings) : QObject (Factory::instance()),
    d_ptr (new PluginHandlePrivate (this))
{
    Q_D(PluginHandle);
    d->uuid = p_uuid;
    d->settings = p_settings;
    QObject::connect (this, SIGNAL (crashed (QString)), Factory::instance(), SLOT (doPluginCrash (QString)));
    QObject::connect (this, SIGNAL (started (QString)), Factory::instance(), SLOT (doPluginLoad (QString)));
    QObject::connect (this, SIGNAL (stopped (QString)), Factory::instance(), SLOT (doPluginUnload (QString)));
    start();
}

PluginHandle::~PluginHandle()
{
    this->stop();
}

void PluginHandle::stop()
{
    Q_D (PluginHandle);

    if (d->process) {
        d->process->terminate();
        d->process->close();
    }
}

void PluginHandle::start()
{
    Q_D (PluginHandle);

    if (!d->process) {
        QStringList args;
        args << "--" WINTER_COMMAND_LINE_IPC     << WINTER_COMMAND_LINE_IPC_PLUGIN
             << "--" WINTER_COMMAND_LINE_FACTORY << d->uuid;

        d->process = new QProcess (Factory::instance());
        QObject::connect (d->process, SIGNAL (started()), this, SLOT (catchStart()));
        QObject::connect (d->process, SIGNAL (error (QProcess::ProcessError)), this, SLOT (catchError (QProcess::ProcessError)));
        QObject::connect (d->process, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (catchExit (int, QProcess::ExitStatus)));
        QObject::connect (d->process, SIGNAL (readyReadStandardOutput()), this, SLOT (on_process_readyReadStdOut()));
        QObject::connect (d->process, SIGNAL (readyReadStandardError()), this, SLOT (on_process_readyReadStdErr()));

        d->process->setProcessChannelMode (QProcess::SeparateChannels);

        if (d->process->startDetached (QApplication::applicationFilePath(), args)) {
            qDebug() << "(core) [PluginPluginHandle] Forked process for plug-in" << d->uuid;
        }
    }
    else
        qDebug() << "(core) [PluginPluginHandle] Plug-in" << name() << "has already started in pid" << d->process->pid();
}

void PluginHandle::on_process_readyReadStdErr()
{

}

void PluginHandle::on_process_readyReadStdOut()
{

}

bool PluginHandle::isActive() const
{
    Q_D (const PluginHandle);
    return (d->process && d->process->state() == QProcess::Running) && !d->uuid.isEmpty();
}

QSettings* PluginHandle::settings() const
{
    Q_D (const PluginHandle);
    return d->settings;
}

QString PluginHandle::uuid() const
{
    Q_D (const PluginHandle);
    return d->uuid;
}

QString PluginHandle::name() const
{
    if (settings()) {
        return settings()->value ("Description/Name").toString();
    }

    return QString::null;
}

void PluginHandle::catchExit (int p_exitCode, const QProcess::ExitStatus& p_exitStatus)
{
    switch (p_exitStatus) {
    case QProcess::NormalExit:

        if (p_exitCode == 0) {
            qDebug() << "(core) [PluginPluginHandle] Plug-in" << name() << "has exitted normally with code" << p_exitCode << ".";
            emit stopped();
        }
        else
            catchExit (p_exitCode, QProcess::CrashExit);

        break;

    case QProcess::CrashExit:

        if (p_exitCode != 0) {
            qDebug() << "(core) [PluginPluginHandle] Plug-in" << name() << "has exitted abnormally with code" << p_exitCode << ".";
            emit crashed();
        }
        else catchExit (p_exitCode, QProcess::NormalExit);

        break;

    default:
        qDebug() << p_exitCode << p_exitStatus;
        break;
    }
}

void PluginHandle::catchStart()
{
    qDebug() << "(core) [PluginPluginHandle] Plug-in" << name() << "running.";
    emit started();
}

/// @todo Implement more efficient error-handling here (i.e.: what signal was sent, how it crashed, etc.).
void PluginHandle::catchError (const QProcess::ProcessError& p_err) const
{
    Q_D (const PluginHandle);

    switch (p_err) {
    case QProcess::Crashed:

        if (d->process->exitCode() != 0)
            qDebug() << "(core) [PluginPluginHandle] Plug-in" << name() << "has crashed.";

        break;

    case QProcess::FailedToStart:
        qDebug() << "(core) [PluginPluginHandle] Plug-in" << name() << "has failed to start.";
        break;

    default:
        qDebug() << d->process->errorString();
        break;
    }
}


#include "pluginhandle.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
