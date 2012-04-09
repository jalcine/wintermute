/**
 * @file plugin.hpp
 * Copyright (C) 2011 - Wintermute Development <wntr-devel@thesii.org>
 *
 * Wintermute Visual is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute Visual is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute Visual; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef WNTRVISUAL_API_HPP
#define WNTRVISUAL_API_HPP

#include <QObject>
#include <backend.hpp>

namespace Wintermute
{
namespace Plugins
{
struct AbstractPlugin;
}
namespace Visual
{

class Framework : public Backends::AbstractFramework
{
    Q_OBJECT
    Q_DISABLE_COPY (Framework)

public:
    enum AlertLevel {
        Unknown = 0,
        Information,
        Warning,
        Alert,
        Error,
        Confirmation,
        Update
    };

    Q_FLAGS (AlertLevel AlertLevels)

signals:
    void alertDisplayed (AlertLevel, QString);

public:
    explicit Framework();
    static Framework* instance();

private:
    virtual void initialize();
    virtual void deinitialize();
    static Framework* s_inst;
};

}
}

#endif // API_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
