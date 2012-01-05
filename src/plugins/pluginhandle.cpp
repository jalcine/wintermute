// Local
#include "pluginhandle.hpp"
#include "factory.hpp"

// Qt
#include <QApplication>
#include <QDebug>

// Namespaces
using namespace Wintermute::Plugins;

PluginHandle::PluginHandle() : m_uuid(), m_prcss(NULL), m_settings(NULL), QObject(Factory::instance()) {}

PluginHandle::PluginHandle(const PluginHandle &p_plgnInst) : m_uuid(p_plgnInst.m_uuid),
    m_prcss(p_plgnInst.m_prcss), m_settings(p_plgnInst.m_settings), QObject(p_plgnInst.parent()) {}

PluginHandle::PluginHandle(const QString &p_plgnUuid, QSettings *p_settings) : m_uuid(p_plgnUuid),
        m_settings(p_settings), m_prcss(NULL), QObject(Factory::instance())
{
    connect(this, SIGNAL(destroyed(QObject *)), this, SLOT(stop()));
    connect(this, SIGNAL(crashed(QString)), Factory::instance(), SLOT(doPluginCrash(QString)));
    connect(this, SIGNAL(started(QString)), Factory::instance(), SLOT(doPluginLoad(QString)));
    connect(this, SIGNAL(stopped(QString)), Factory::instance(), SLOT(doPluginUnload(QString)));
    start();
}

PluginHandle::~PluginHandle()
{
    this->stop();
}

void PluginHandle::stop()
{
    m_prcss->terminate();
    m_prcss->close();
}

void PluginHandle::start()
{
    if (!m_prcss) {
        m_prcss = new QProcess(Factory::instance());
        connect(m_prcss, SIGNAL(started()), this, SLOT(catchStart()));
        connect(m_prcss, SIGNAL(error(QProcess::ProcessError)), this, SLOT(catchError(QProcess::ProcessError)));
        connect(m_prcss, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(catchExit(int, QProcess::ExitStatus)));

        m_prcss->setProcessChannelMode(QProcess::ForwardedChannels);
        m_prcss->start(QApplication::applicationFilePath(), QStringList() << "--ipc" << "plugin" << "--plugin" << m_uuid);
    } else
        qDebug() << "(core) [PluginPluginHandle] Plug-in" << name() << "has already started in pid" << m_prcss->pid();
}

bool PluginHandle::isActive()
{
    return (m_prcss != NULL && m_prcss->state() == QProcess::Running) && !m_uuid.isEmpty();
}

QSettings* PluginHandle::settings() { return m_settings; }

QString PluginHandle::uuid() { return m_uuid; }

QString PluginHandle::name() { return m_settings->value("Description/Name").toString(); }

void PluginHandle::catchExit(int p_exitCode, const QProcess::ExitStatus& p_exitStatus)
{
    switch (p_exitStatus)
    {
        case QProcess::NormalExit:
            if (p_exitCode == 0) {
                //qDebug() << "(core) [PluginPluginHandle] Plug-in" << name() << "has exitted normally with code" << p_exitCode << ".";
                emit stopped();
            } else
                catchExit(p_exitCode,QProcess::CrashExit);
        break;

        case QProcess::CrashExit:
            if (p_exitCode != 0) {
                qDebug() << "(core) [PluginPluginHandle] Plug-in" << name() << "has exitted abnormally with code" << p_exitCode << ".";
                emit crashed();
            } else catchExit(p_exitCode,QProcess::NormalExit);
        break;

        default:
            qDebug() << p_exitCode << p_exitStatus;
        break;
    }
}

void PluginHandle::catchStart()
{
    //qDebug() << "(core) [PluginPluginHandle] Plug-in" << name() << "running.";
    emit started();
}

/// @todo Implement more efficient error-handling here (i.e.: what signal was sent, how it crashed, etc.).
void PluginHandle::catchError(const QProcess::ProcessError &p_err)
{
    switch (p_err)
    {
        case QProcess::Crashed:
            if (m_prcss->exitCode() != 0)
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
