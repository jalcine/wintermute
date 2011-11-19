/**
 * @file backend.hpp
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
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

#include <QMap>
#include <QObject>

namespace Wintermute {
    namespace Backends {
        struct AbstractFramework;
        struct AbstractBackend;

        typedef QMap<QString,AbstractBackend*> BackendList;

        class AbstractFramework : public QObject {
            Q_OBJECT
            Q_DISABLE_COPY(AbstractFramework)
            Q_PROPERTY(const StartupMode Mode READ startMode WRITE setStartMode)

            signals:
                void started();
                void stopped();

                void added();
                void added(AbstractBackend* );

                void removed();
                void removed(AbstractBackend* );

            public:
                enum StartupMode {
                    Default = 0,
                    Configuration,
                    Manual
                };

                AbstractFramework(QObject* = 0);
                virtual ~AbstractFramework();
                void addBackend(AbstractBackend*);
                void removeBackend(AbstractBackend* );
                void setDefaultBackend(AbstractBackend* );
                void setStartMode(const StartupMode& );

                const bool isBackendListed(const AbstractBackend* ) const;
                AbstractBackend* defaultBackend() const;
                const StartupMode& startMode() const;

            public slots:
                void start();
                void stop();

            protected:
                BackendList m_cmpLst;
                QString m_dfltBcknd;
                StartupMode m_strtMd;
                virtual void initialize() = 0;
                virtual void deinitialize() = 0;
        };

        class AbstractBackend : public QObject {
            Q_OBJECT
            Q_PROPERTY(const QString ID READ id)
            Q_PROPERTY(const bool Active READ isActive)
            friend class AbstractFramework;

            signals:
                void started();
                void stopped();

            public:
                AbstractBackend(QObject* = 0);
                virtual ~AbstractBackend();
                virtual const QString id() const = 0;
                virtual const bool isActive() const = 0;

            public slots:
                virtual void start();
                virtual void stop();

            protected:
                QString m_id;
                mutable bool m_actv;
                virtual void initialize() = 0;
                virtual void deinitialize() = 0;
        };
    }
}
