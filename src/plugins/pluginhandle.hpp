#ifndef _PLUGINHANDLE_HPP_
#define _PLUGINHANDLE_HPP_

// Qt
#include <QString>
#include <QSettings>
#include <QProcess>

namespace Wintermute {
namespace Plugins {

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
    Q_DISABLE_COPY(PluginHandle)
    Q_PROPERTY(const bool Active READ isActive)
    Q_PROPERTY(const QString Name READ name)

    private:
        QProcess* m_prcss;
        const QString m_uuid;
        QSettings* m_settings;
        void doCrashed(const QString&);
        void doLoaded(const QString&);
        void doUnloaded(const QString&);

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
        explicit PluginHandle(const QString &, QSettings *);

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
        void crashed(const QString&);

        /**
         * @brief Emitted when the process of the plug-in starts.
         * @fn started
         */
        void started();
        void started(const QString&);

        /**
         * @brief Emitted when the process of the plug-in stops.
         * @fn stopped
         */
        void stopped();
        void stopped(const QString&);

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
        void catchError(const QProcess::ProcessError& );
        void catchExit(int, const QProcess::ExitStatus& );
};

} // namespaces
}

#endif // _PLUGINHANDLE_HPP_
