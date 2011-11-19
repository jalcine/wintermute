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

#include "backend.hpp"

namespace Wintermute {
    namespace Backends {
        AbstractFramework::AbstractFramework(QObject *parent) : QObject(parent),
            m_cmpLst(), m_dfltBcknd() { }

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

            m_dfltBcknd = p_cmpt->id();
        }

        void AbstractFramework::setStartMode(const StartupMode &p_strtMd) {
            m_strtMd = p_strtMd;
        }

        const bool AbstractFramework::isBackendListed(const AbstractBackend *p_cmpt) const {
            return m_cmpLst.contains(p_cmpt->id());
        }

        const AbstractFramework::StartupMode& AbstractFramework::startMode() const {
            return m_strtMd;
        }

        AbstractBackend* AbstractFramework::defaultBackend() const { return m_cmpLst.value(m_dfltBcknd); }

        void AbstractFramework::start() {
            initialize();
            emit started();
        }

        void AbstractFramework::stop() {
            deinitialize();
            emit stopped();
        }

        AbstractFramework::~AbstractFramework() { }

        AbstractBackend::AbstractBackend(QObject *parent) : QObject(parent) { }

        void AbstractBackend::start() {
            initialize();
            emit started();
        }

        void AbstractBackend::stop() {
            deinitialize();
            emit stopped();
        }

        AbstractBackend::~AbstractBackend() { }
    }
}
