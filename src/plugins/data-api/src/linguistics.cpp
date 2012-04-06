/**
 * @file linguistics.cpp
 * @author Wintermute Development <wntr-devel@thesii.org>
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

// stdc++ includes
#include <algorithm>

// Qt includes
#include <QDir>
#include <QDebug>

// local includes
#include "config.hpp"
#include "linguistics.hpp"
#include "lexical/model.hpp"
#include "syntax/model.hpp"

using namespace Wintermute::Data::Linguistics;
using std::unique;

QString System::s_dir = QString (WNTR_DATA_DIR) + "/" + QString (WNTRDATA_LING_DIR);
QString System::s_lcl = QString (WNTR_LOCALE);

void System::load (const QString p_dir, const QString p_lcl)
{
    qDebug() << "(data) [System] # ling # System loading...";

    System::setDirectory (p_dir);
    System::setLocale (p_lcl);

    Lexical::Cache::addStorage ( (new Lexical::DomStorage));
    Syntax::Cache::addStorage ( (new Syntax::DomStorage));

    Lexical::Cache::generate();

    qDebug() << "(data) [System] # ling # System loaded.";
}

void System::unload()
{
    qDebug() << "(ling) [System] System unloading...";
    Lexical::Cache::clearStorage();
    Syntax::Cache::clearStorage();
    qDebug() << "(data) [System] # ling # System unloaded.";
}

void System::setLocale (const QString p_lcl)
{
    if (p_lcl.isEmpty())
        return;

    System::s_lcl = p_lcl;
    qDebug() << "(data) [System] # ling # Default locale:" << p_lcl;
}

void System::setDirectory (const QString p_dir)
{
    if (p_dir.isEmpty())
        return;

    QDir* d = new QDir (p_dir);

    if (d->exists ()) {
        System::s_dir = d->absolutePath();
        qDebug() << "(data) [System] # ling # Root dir:" << p_dir;
    }
}

const QStringList System::locales()
{
    QDir dir (System::directory ());
    const QString path = dir.absolutePath ();
    dir.setFilter (QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList locale = dir.entryList ();
    unique (locale.begin (), locale.end ());
    return locale;
}

#include "linguistics.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
