/***
 *  This file is part of Wintermute.
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
 *  You should have received a copy of the GNU Library General Public License
 *  along with Wintermute.  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <QFile>
#include <QSettings>

#include <app/factory.hpp>
#include <app/factory.hxx>
#include <app/plugin.hxx>
#include <app/plugin.hpp>
#include <app/shellplugin.h>

using namespace Wintermute::Plugins;

AbstractPluginPrivate::AbstractPluginPrivate (AbstractPlugin* p_qPtr) :
    m_plgnLdr (0), m_cnfg (0), q_ptr (p_qPtr), m_sttngs (0)
{

}

void AbstractPluginPrivate::doStart()
{
    Q_Q(AbstractPlugin);
    q->start();
    emit q->started();
}

void AbstractPluginPrivate::doStop()
{
    Q_Q(AbstractPlugin);
    q->stop();
    emit q->stopped();
}

bool AbstractPluginPrivate::loadPackages() const
{
    Q_Q (const AbstractPlugin);
    const QStringList deps = q->packages();
    //qDebug () << "(core) [AbstractPlugin] Loading packages for" << q->name () << ";" << deps.length () << "package(s) to be loaded.";
    foreach (const QString dep, deps) {
        const QString depName = dep.split (" ").at (0);
    }

    return true;
}

bool AbstractPluginPrivate::loadPlugins() const
{
    Q_Q (const AbstractPlugin);
    const QStringList plgnLst = q->plugins();
    qDebug () << "(core) [AbstractPluginPrivate] Loading plug-ins for"
              << q->name () << ";"
              << plgnLst.length () << "plugin(s) to be loaded.";

    foreach (const QString plgn, plgnLst) {
        const QString plgnUuid = plgn.split (" ").at (0);

        if (Factory::loadedPlugins().contains (plgnUuid))
            qDebug () << "(core) [AbstractPluginPrivate] Dependency"
                      << Factory::attribute (plgnUuid, "Description/Name").toString()
                      << "already loaded.";
        else {
            ShellPlugin* gnrc = new ShellPlugin (plgnUuid);
            qDebug() << "(core) [AbstractPluginPrivate] Loading dependency"
                     << gnrc->name() << "...";

            if (q->loadLibrary() && QFile::exists (gnrc->d_func()->m_plgnLdr->fileName()))
                qWarning() << "(core) [AbstractPluginPrivate] Loaded symbols of plug-in"
                           << Factory::attribute (plgnUuid, "Description/Name").toString() << ".";
            else {
                qWarning() << "(core) [AbstractPluginPrivate] Unable to load symbols of depedency"
                           << gnrc->name() << ":" << gnrc->d_func()->m_plgnLdr->errorString();
                return false;
            }
        }
    }
    return true;
}

void AbstractPluginPrivate::loadSettings (const QString& p_uuid)
{
    m_sttngs = Factory::getPluginSettings (p_uuid);
    m_cnfg = new QSettings ("Synthetic Intellect Institute", p_uuid);
}

AbstractPluginPrivate::~AbstractPluginPrivate()
{

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
