/**
 * @file pluginhandle.cpp
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
#include "pluginhandle.hpp"
#include "factory.hpp"

// Qt
#include <QApplication>
#include <QDebug>

// Namespaces
using namespace Wintermute::Plugins;

PluginHandle::PluginHandle() : QObject ( Factory::instance() ),
    m_uuid ( QString::null ),
    m_prcss ( NULL ),
    m_settings ( NULL ) {}

PluginHandle::PluginHandle ( const PluginHandle &p_plgnInst ) : QObject ( p_plgnInst.parent() ),
    m_uuid ( p_plgnInst.m_uuid ),
    m_prcss ( p_plgnInst.m_prcss ),
    m_settings ( p_plgnInst.m_settings ) {}

PluginHandle::PluginHandle ( const QString &p_plgnUuid, QSettings *p_settings ) : QObject ( Factory::instance() ),
    m_uuid ( p_plgnUuid ),
    m_prcss ( NULL ),
    m_settings ( p_settings ) {
    connect ( this, SIGNAL ( destroyed ( QObject * ) ), this, SLOT ( stop() ) );
    connect ( this, SIGNAL ( crashed ( QString ) ), Factory::instance(), SLOT ( doPluginCrash ( QString ) ) );
    connect ( this, SIGNAL ( started ( QString ) ), Factory::instance(), SLOT ( doPluginLoad ( QString ) ) );
    connect ( this, SIGNAL ( stopped ( QString ) ), Factory::instance(), SLOT ( doPluginUnload ( QString ) ) );
    start();
}

PluginHandle::~PluginHandle() {
    this->stop();
}

void PluginHandle::stop() {
    m_prcss->terminate();
    m_prcss->close();
}

void PluginHandle::start() {
    if ( !m_prcss ) {
        m_prcss = new QProcess ( Factory::instance() );
        connect ( m_prcss, SIGNAL ( started() ), this, SLOT ( catchStart() ) );
        connect ( m_prcss, SIGNAL ( error ( QProcess::ProcessError ) ), this, SLOT ( catchError ( QProcess::ProcessError ) ) );
        connect ( m_prcss, SIGNAL ( finished ( int, QProcess::ExitStatus ) ), this, SLOT ( catchExit ( int, QProcess::ExitStatus ) ) );

        m_prcss->setProcessChannelMode ( QProcess::ForwardedChannels );
        m_prcss->start ( QApplication::applicationFilePath(), QStringList() << "--ipc" << "plugin" << "--plugin" << m_uuid );
    } else
        qDebug() << "(core) [PluginPluginHandle] Plug-in" << name() << "has already started in pid" << m_prcss->pid();
}

bool PluginHandle::isActive() {
    return ( m_prcss != NULL && m_prcss->state() == QProcess::Running ) && !m_uuid.isEmpty();
}

QSettings* PluginHandle::settings() {
    return m_settings;
}

QString PluginHandle::uuid() {
    return m_uuid;
}

QString PluginHandle::name() {
    return m_settings->value ( "Description/Name" ).toString();
}

void PluginHandle::catchExit ( int p_exitCode, const QProcess::ExitStatus& p_exitStatus ) {
    switch ( p_exitStatus ) {
    case QProcess::NormalExit:
        if ( p_exitCode == 0 ) {
            qDebug() << "(core) [PluginPluginHandle] Plug-in" << name() << "has exitted normally with code" << p_exitCode << ".";
            emit stopped();
        } else
            catchExit ( p_exitCode,QProcess::CrashExit );
        break;

    case QProcess::CrashExit:
        if ( p_exitCode != 0 ) {
            qDebug() << "(core) [PluginPluginHandle] Plug-in" << name() << "has exitted abnormally with code" << p_exitCode << ".";
            emit crashed();
        } else catchExit ( p_exitCode,QProcess::NormalExit );
        break;

    default:
        qDebug() << p_exitCode << p_exitStatus;
        break;
    }
}

void PluginHandle::catchStart() {
    qDebug() << "(core) [PluginPluginHandle] Plug-in" << name() << "running.";
    emit started();
}

/// @todo Implement more efficient error-handling here (i.e.: what signal was sent, how it crashed, etc.).
void PluginHandle::catchError ( const QProcess::ProcessError &p_err ) {
    switch ( p_err ) {
    case QProcess::Crashed:
        if ( m_prcss->exitCode() != 0 )
            qDebug() << "(core) [PluginPluginHandle] Plug-in" << name() << "has crashed.";
        break;

    case QProcess::FailedToStart:
        qDebug() << "(core) [PluginPluginHandle] Plug-in" << name() << "has failed to start.";
        break;

    default:
        qDebug() << m_prcss->errorString();
        break;
    }
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
