/**
 * @file rules.hpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 * @date Sun, 30 Oct 2011 21:54:16
 *
 * @section lcns Licensing
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

#ifndef WNTRDATA_SYNTAX_MANAGER_HPP
#define WNTRDATA_SYNTAX_MANAGER_HPP

// Qt includes
#include <QObject>

namespace Wintermute {
namespace Data {
namespace Linguistics {
namespace Syntax {

class Chain;

class Manager : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( Manager )

private:
    static Manager* s_inst;
    Manager();

public slots:
    static Manager* instance();
    void read ( Chain& );
    void write ( Chain& );
    const bool exists ( const QString&, const QString& ) const;
};

}
}
}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
