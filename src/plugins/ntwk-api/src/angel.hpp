/**
 * @file angel.hpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 * @date October 29, 2011 3:44 PM
 *
 * This library is free software; you can redistribute it and/or
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
 *
 */

#ifndef ANGEL_HPP
#define	ANGEL_HPP

#include <QObject>

namespace Wintermute {
namespace Network {
struct Instance;

/**
 * @brief
 */
class Instance : public QObject {
    Q_OBJECT
    Q_ENUMS ( Type Class )
    Q_DISABLE_COPY ( Instance )

signals:
    void started();
    void stopped();

public:
    enum Type {
        MicroMind = 0,
        SubMind,
        Mind
    };

    enum Class {
        Seraph = 0,
        Ophan,
        Cherub,
        Angel,
        ArchAngel
    };

    Instance ( QObject* = 0 );
    virtual ~Instance();
};
}
}

#endif	/* ANGEL_HPP */

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
