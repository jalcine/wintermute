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

        AbstractFramework::AbstractFramework(AbstractPlugin* p_plgn, QObject *p_prnt) : QObject(p_prnt),
            m_cmpLst(), m_dfltBcknd(), m_plgn(p_plgn) {
            connect(m_plgn,SIGNAL(initializing()),this,SLOT(start()));
            connect(m_plgn,SIGNAL(deinitializing()),this,SLOT(stop()));

            m_dfltBcknd = m_plgn->attribute("Framework/Defaults").toStringList();
            m_strtMd = (StartupMode) m_plgn->attribute("Framework/StartMode").toInt();
        }

        void AbstractFramework::addBackend(AbstractBackend *p_cmpt) {
            if (!isBackendListed(p_cmpt)){
                m_cmpLst.insert(p_cmpt->id(),p_cmpt);
                emit added();
                emit added(p_cmpt);
            }
        }

        void AbstractFramework::removeBackend(AbstractBackend *p_cmpt) {
            if (isBackendListed(p_cmpt)){
                m_cmpLst.remove(p_cmpt->id());
                emit removed();
                emit removed(p_cmpt);
            }
        }

        void AbstractFramework::setDefaultBackend(AbstractBackend *p_cmpt){
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
            return m_cmpLst.contains(p_cmpt->id());
        }

        const AbstractFramework::StartupMode& AbstractFramework::startMode() const {
            return m_strtMd;
        }

        QList<AbstractBackend*> AbstractFramework::defaultBackend() const {
            QList<AbstractBackend*> l_lst;
            foreach (const QString& l_bcknd, m_dfltBcknd)
                l_lst << m_cmpLst.value(l_bcknd);

            return l_lst;
        }

        void AbstractFramework::start() {
            switch (m_strtMd){
                case Configuration: {
                    if (!m_dfltBcknd.empty()){
                        foreach (const QString& l_bcknd, m_dfltBcknd)
                            m_cmpLst.insert(l_bcknd,AbstractBackend::obtainBackend(l_bcknd));
                    } else {
                        qWarning() << "(core) [AbstractFramework] No back-ends to be automatically loaded from configuration!";
                    }
                };

                default:
                case Manual: {

                };
            }


            initialize();
            emit started();
        }

        void AbstractFramework::stop() {
            deinitialize();
            emit stopped();
        }

        AbstractFramework::~AbstractFramework() { }

        AbstractBackend::AbstractBackend(AbstractPlugin* p_plgn, QObject *p_prnt) : QObject(p_prnt),
            m_plgn(p_plgn) {
            if (AbstractBackend::s_lst.contains(m_plgn->uuid())){
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
        AbstractBackend* AbstractBackend::obtainBackend(const QString& l_bcknd){
            if (!AbstractBackend::s_lst.contains(l_bcknd))
                Factory::loadPlugin(l_bcknd,true);

            return AbstractBackend::s_lst.value(l_bcknd);
        }

        AbstractBackend::~AbstractBackend() {
            AbstractBackend::s_lst.remove(m_plgn->uuid());
        }
    }
}
