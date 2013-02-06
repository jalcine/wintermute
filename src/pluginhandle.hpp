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
 * @date 04/22/12 5:43:18 AM
 */

#ifndef WINTERMUTE_PLUGINHANDLE_HPP
#define WINTERMUTE_PLUGINHANDLE_HPP

#include <QObject>
#include <QProcess>
#include <QSettings>

#include <global.hpp>

WINTER_FORWARD_DECLARE_CLASS(Factory)
WINTER_FORWARD_DECLARE_STRUCT(PluginHandlePrivate)

WINTER_BEGIN_NAMESPACE

/**
 * @brief Provides the Factory with a controllable instance of a plugin.
 *
 * @see Factory, AbstractPlugin
 */
class PluginHandle : public QObject
{
    friend class Factory;

    Q_OBJECT
    WINTER_DECLARE_PRIVATE_STRUCT(PluginHandle)
    Q_DISABLE_COPY (PluginHandle)
    Q_PROPERTY (bool Active READ isActive)
    Q_PROPERTY (QString Name READ name)
    QScopedPointer<PluginHandlePrivate> d_ptr;

public:
    explicit PluginHandle (const QString& p_uuid, QSettings* p_settings);
    virtual ~PluginHandle();
    bool isActive() const;
    QString uuid() const;
    QString name() const;
    QSettings* settings() const;

signals:
    void crashed (const QString& p_uuid = QString::null);
    void started (const QString& p_uuid = QString::null);
    void stopped (const QString& p_uuid = QString::null);

public slots:
    void stop();
    void start();
    void on_watcher_fileChanged (const QString& p_path);

private slots:
    void on_process_readyReadStdOut();
    void on_process_readyReadStdErr();
    void catchStart();
    void catchError (const QProcess::ProcessError& p_err) const;
    void catchExit (int, const QProcess::ExitStatus&);
};

WINTER_END_NAMESPACE

#endif // WINTER_PLUGINHANDLE_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
