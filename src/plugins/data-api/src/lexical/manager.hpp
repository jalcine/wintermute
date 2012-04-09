/**
 * @file manager.hpp
 * @author Jacky Alcine <jacky.alcine@thesii.org>
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
 * @endlegalese
 */

#ifndef WNTRDATA_LEXMANAGER_HPP
#define WNTRDATA_LEXMANAGER_HPP

// Qt includes
#include <QObject>

namespace Wintermute
{
namespace Data
{
namespace Linguistics
{
namespace Lexical
{

class Data;

class Manager : public QObject
{
    friend class NodeAdaptor;
    friend class NodeInterface;
    Q_OBJECT
    Q_DISABLE_COPY (Manager)

private:
    static Manager* s_inst;
    Manager();

signals:
    void nodeCreated (const QString&);

public slots:
    void generate();
    Data& pseudo (Data&) const;
    Data& read (Data&) const;
    const Data& write (const Data&);
    bool exists (const Data&) const;
    bool isPseudo (const Data&) const;
    static Manager* instance();
};

}
}
}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
