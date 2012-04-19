/**
 * @file wntrling.hpp
 * This file is part of Wintermute Linguistics
 *
 * Copyright (C) 2011 - Wintermute Development <wntr-devel@thesii.org>
 *
 * Wintermute Linguistics is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute Linguistics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute Linguistics; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef WNTRLING_SYSTEM_HPP
#define WNTRLING_SYSTEM_HPP

// Qt includes
#include <QObject>

namespace Wintermute
{
namespace Linguistics
{
class Engine;
/**
 * @brief Represents a maintence class for Wintermute's linguistics.
 * The System object (as commonly used throughout Wintermute) handles the
 * nitty-gritty work of starting up, cleaning up and shutting down the objects
 * required for optimal natural language intepretative peformance to occur.
 * @class System wntrling.hpp "src/wntrling.hpp"
 */
class System : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (System)

private:
    static System* s_sys; /**< The internal instance. */
    Engine* m_prs; /**< The global parser instance. */

    /**
     * @brief Constructor.
     * @fn System
     */
    System();

    /**
     * @brief Destructor
     */
    virtual ~System();

signals:
    /**
     * @brief Emitted when the System is online.
     * @fn started
     */
    void started();

    /**
     * @brief Emitted when the System is offline.
     * @fn stopped
     */
    void stopped();

    /**
     * @brief Emitted when a response is generated.
     * @fn responseFormed
     * @param p_rpsn The response formed by Wintermute.
     */
    void responseFormed (const QString&);

public:
    static System* instance();

public slots:
    static void start();
    static void stop();
    static void tellSystem (const QString&);
};

}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
