/**
 * @file backend.cpp
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

#include <QDebug>
#include "backend.hpp"
#include "plugins.hpp"

using namespace Wintermute::Plugins;

namespace Wintermute {
    namespace Backends {
        BackendList AbstractBackend::s_lst;
        FrameworkList AbstractFramework::s_frmk;

        AbstractFramework::AbstractFramework(AbstractPlugin* p_plgn, QObject *p_prnt) : QObject(p_prnt),
                m_bckndLst(), m_dfltBcknd(), m_plgn(p_plgn) {
            AbstractFramework::s_frmk.insert(m_plgn->uuid(),this);
        }

        void AbstractFramework::addBackend(AbstractBackend *p_cmpt) {
            if (!isBackendListed(p_cmpt)) {
                m_bckndLst.insert(p_cmpt->id(),p_cmpt);
                emit added();
                emit added(p_cmpt);
            }
        }

        void AbstractFramework::removeBackend(AbstractBackend *p_cmpt) {
            if (isBackendListed(p_cmpt)) {
                m_bckndLst.remove(p_cmpt->id());
                emit removed();
                emit removed(p_cmpt);
            }
        }

        void AbstractFramework::setDefaultBackend(AbstractBackend *p_cmpt) {
            if (!isBackendListed(p_cmpt))
                addBackend(p_cmpt);

            m_dfltBcknd << p_cmpt->id();
            m_dfltBcknd.removeDuplicates();
            m_plgn->setAttribute("Framework/Defaults",QVariant::fromValue<QStringList>(m_dfltBcknd));
        }

        void AbstractFramework::setStartMode(const StartupMode &p_strtMd) {
            m_strtMd = p_strtMd;
            m_plgn->setAttribute("Framework/StartMode",QVariant::fromValue<int>((int) p_strtMd));
        }

        const bool AbstractFramework::isBackendListed(const AbstractBackend *p_cmpt) const {
            return m_bckndLst.contains(p_cmpt->id());
        }

        const AbstractFramework::StartupMode& AbstractFramework::startMode() const {
            return m_strtMd;
        }

        QList<AbstractBackend*> AbstractFramework::defaultBackend() const {
            QList<AbstractBackend*> l_lst;
            foreach (const QString& l_bcknd, m_dfltBcknd)
            l_lst << m_bckndLst.value(l_bcknd);

            return l_lst;
        }

        void AbstractFramework::start() {
            qDebug() << "(core) [AbstractFramework] Starting...";
            m_dfltBcknd = m_plgn->attribute("Framework/Defaults").toStringList();
            m_strtMd = (StartupMode) m_plgn->attribute("Framework/StartMode").toInt();
            connect(m_plgn,SIGNAL(started()),this,SLOT(start()));
            connect(m_plgn,SIGNAL(stopped()),this,SLOT(stop()));

            qDebug() << "(core) [AbstractFramework] Startup mode is " << m_strtMd;

            switch (m_strtMd) {
                case Configuration: {
                    if (!m_dfltBcknd.empty()) {
                        foreach (const QString& l_bcknd, m_dfltBcknd) {
                            AbstractBackend* l_bck = AbstractBackend::obtainBackend(l_bcknd);
                            if (!l_bck){
                                qDebug() << "(core) [AbstractFramework] Backend" << Factory::attribute(l_bcknd,"Description/Name").toString() << "couldn't be loaded.";
                                return;
                            }

                            m_bckndLst.insert(l_bcknd,l_bck);
                            connect(this,SIGNAL(started()),l_bck,SLOT(start()));
                        }
                    } else
                        qDebug() << "(core) [AbstractFramework] No back-ends to be automatically loaded from configuration!";
                };

                default:
                case Manual: {
                    // What's there to do? :P
                };
            }


            initialize();
            emit started();
            qDebug() << "(core) [AbstractFramework] Started.";
        }

        void AbstractFramework::stop() {
            deinitialize();
            emit stopped();
        }

        AbstractFramework* AbstractFramework::obtainFramework(const QString &p_uuid) {
            if (!s_frmk.contains(p_uuid))
                return 0;
            return s_frmk.value(p_uuid);
        }

        const int AbstractFramework::frameworks() {
            return s_frmk.values().size();
        }

        AbstractFramework::~AbstractFramework() { }

        AbstractBackend::AbstractBackend(AbstractPlugin* p_plgn, QObject *p_prnt) : QObject(p_prnt),
                m_plgn(p_plgn) {
            if (AbstractBackend::s_lst.contains(m_plgn->uuid())) {
                qWarning() << "(core) [AbstractBackend] Plugin" << m_plgn->uuid() << "already registered.";
                Factory::unloadPlugin(m_plgn->uuid());
            } else
                AbstractBackend::s_lst.insert(m_plgn->uuid(),this);
        }

        void AbstractBackend::start() {
            initialize();
            emit started();
        }

        void AbstractBackend::stop() {
            deinitialize();
            emit stopped();
        }

        /// @note Load the associated plug-in. On load, it should register its backend to the global list.
        /// @note It should determine if the plug-in exists on the system (need addition of a method to Factory).
        AbstractBackend* AbstractBackend::obtainBackend(const QString& l_bcknd) {
            qDebug() << "(core) [AbstractBackend] Obtaining back-end" << l_bcknd << "...";
            if (!AbstractBackend::s_lst.contains(l_bcknd)) {
                if (Factory::loadPlugin(l_bcknd,true)) {
                    qDebug() << "(core) [AbstractBackend] Back-end" << l_bcknd << "obtained.";
                    return AbstractBackend::s_lst.value(l_bcknd);
                }
            }

            return 0;
        }

        AbstractBackend::~AbstractBackend() {
            AbstractBackend::s_lst.remove(m_plgn->uuid());
        }
    }
}

const QString Wintermute::Backends::AbstractBackend::id() const
{
    return m_id;
}
