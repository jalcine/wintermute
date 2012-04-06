/**
 * @file abstractplugin.cpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 *
 * @section lcns Licensing
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
 *
 */

// Local
#include "config.hpp"
#include "abstractplugin.hpp"
#include "factory.hpp"
#include "core.hpp"

// Qt
#include <QApplication>
#include <QDebug>
#include <QFile>

// Namespaces
using namespace Wintermute::Plugins;

// Constructors
AbstractPlugin::AbstractPlugin (QPluginLoader* p_pl) : QObject (p_pl), m_plgnLdr (p_pl), m_sttngs (NULL) {}

AbstractPlugin::AbstractPlugin (const AbstractPlugin& p_pb) : QObject (p_pb.m_plgnLdr),
    m_plgnLdr (p_pb.m_plgnLdr), m_sttngs (p_pb.m_sttngs), m_cnfg (p_pb.m_cnfg) {}

// Access methods
QString AbstractPlugin::author() const
{
    return m_sttngs->value ("Description/Author").toString();
}

QString AbstractPlugin::name() const
{
    return m_sttngs->value ("Description/Name").toString();
}

QString AbstractPlugin::vendorName() const
{
    return m_sttngs->value ("Description/Vendor").toString();
}

QString AbstractPlugin::uuid() const
{
    return m_sttngs->value ("Version/UUID").toString();
}

QString AbstractPlugin::description() const
{
    return m_sttngs->value ("Description/Blurb").toString();
}

QString AbstractPlugin::webPage() const
{
    return m_sttngs->value ("Description/WebPage").toString();
}

double AbstractPlugin::version() const
{
    return m_sttngs->value ("Version/Plugin").toDouble();
}

double AbstractPlugin::compatVersion() const
{
    return m_sttngs->value ("Version/Compat", WNTR_VERSION).toDouble();
}

bool AbstractPlugin::isSupported() const
{
    return WNTR_VERSION >= compatVersion();
}

QStringList AbstractPlugin::plugins() const
{
    QStringList dep = m_sttngs->value ("Depends/Plugins").toStringList();
    dep.removeDuplicates();
    dep.removeAll ("None");
    return dep;
}

bool AbstractPlugin::loadPlugins() const
{
    const QStringList plgnLst = this->plugins();
    qDebug () << "(core) [AbstractPlugin] Loading plug-ins for"
              << name () << ";"
              << plgnLst.length () << "plugin(s) to be loaded.";

    foreach (const QString plgn, plgnLst) {
        const QString plgnUuid = plgn.split (" ").at (0);

        if (Factory::loadedPlugins().contains (plgnUuid))
            qDebug () << "(core) [AbstractPlugin] Dependency"
                      << Factory::attribute (plgnUuid, "Description/Name").toString()
                      << "already loaded.";
        else {
            Factory::ShellPlugin* gnrc = new Factory::ShellPlugin (plgnUuid);
            qDebug() << "(core) [AbstractPlugin] Loading dependency"
                     << gnrc->name() << "...";

            if (gnrc->loadLibrary() && QFile::exists (gnrc->m_plgnLdr->fileName()))
                qWarning() << "(core) [AbstractPlugin] Loaded symbols of plug-in"
                           << Factory::attribute (plgnUuid, "Description/Name").toString() << ".";
            else {
                qWarning() << "(core) [AbstractPlugin] Unable to load symbols of depedency"
                           << gnrc->name() << ":" << gnrc->m_plgnLdr->errorString();
                return false;
            }
        }
    }
    return true;
}

bool AbstractPlugin::hasPlugins() const
{
    const QStringList deps = this->plugins();
    foreach (const QString dep, deps) {
        const QString depName = dep.split (" ").at (0);
        const QString depComparison = dep.split (" ").at (1);
        const QString depVersion = dep.split (" ").at (2);

        if (Factory::allPlugins().contains (depName)) {
            if (!Factory::loadedPlugins().contains (depName))
                qDebug() << "(core) [AbstractPlugin] Dependency" << depName
                         << "of" << this->name () << "isn't loaded.";

            const AbstractPlugin* plgn = Factory::s_plgnLst.value (depName);

            if (depComparison == "==") {
                if (! (depVersion.toDouble() == plgn->version())) {
                    qDebug() << "(core) [AbstractPlugin] " << this->name()
                             << "requires" << depName
                             << "to have a version of" << depVersion;
                    return false;
                }
            }
            else if (depComparison == ">") {
                if (! (depVersion.toDouble () > plgn->version())) {
                    qDebug() << "(core) [AbstractPlugin] " << this->name ()
                             << "requires" << depName
                             << "to have a version greater than" << depVersion;
                    return false;
                }
            }
            else if (depComparison == "<") {
                if (! (depVersion.toDouble () < plgn->version ())) {
                    qDebug() << "(core) [AbstractPlugin] " << this->name()
                             << "requires" << depName << "to have a version less than" << depVersion;
                    return false;
                }
            }
            else if (depComparison == ">=") {
                if (! (depVersion.toDouble () >= plgn->version ())) {
                    qDebug() << "(core) [AbstractPlugin] " << this->name()
                             << "requires" << depName
                             << "to have a version of at least" << depVersion;
                    return false;
                }
            }
            else if (depComparison == "==") {
                if (! (depVersion.toDouble () > plgn->version ())) {
                    qDebug() << "(core) [AbstractPlugin] " << this->name()
                             << "requires" << depName << "to have a version of at most" << depVersion;
                    return false;
                }
            }
            else {
                qDebug() << "(core) [AbstractPlugin] <" << this->name()
                         << "> : Invalid version string ("
                         << depComparison << ").";
            }
        }
    }

    return true;
}

QStringList AbstractPlugin::packages() const
{
    QStringList dep = m_sttngs->value ("Depends/Packages").toStringList();
    dep.removeDuplicates();
    dep.removeAll ("None");
    return dep;
}

/// @note This method requires code from QPackageKit.
/// @note issue #0000029
bool AbstractPlugin::loadPackages() const
{
    const QStringList deps = this->packages();
    //qDebug () << "(core) [AbstractPlugin] Loading packages for" << name () << ";" << deps.length () << "package(s) to be loaded.";
    foreach (const QString dep, deps) {
        const QString depName = dep.split (" ").at (0);
    }

    return true;
}

/// @note This method requires code from QPackageKit.
/// @note issue #0000029
bool AbstractPlugin::hasPackages() const
{
    const QStringList deps = this->packages();

    foreach (const QString dep, deps) {
        const QString depName = dep.split (" ").at (0);
        const QString depComparison = dep.split (" ").at (1);
        const QString depVersion = dep.split (" ").at (2);
    }

    return true;
}

QVariant AbstractPlugin::attribute (const QString& p_attrPath) const
{
    QVariant val = m_cnfg->value (p_attrPath);

    if (val.isNull() || !val.isValid())
        val = m_cnfg->value ("Configuration/" + const_cast<QString*> (&p_attrPath)->replace ("/", ":"));

    return val;
}

void AbstractPlugin::setAttribute (const QString& p_attrPath, const QVariant& p_attrVal)
{
    m_cnfg->setValue (p_attrPath, p_attrVal);
}

/// @note issue #0000030
void AbstractPlugin::resetAttributes()
{
    m_cnfg->clear();
}

bool AbstractPlugin::loadLibrary() const
{
    QApplication::addLibraryPath (WNTR_PLUGIN_PATH);
    const QString plgnLibrary = m_sttngs->value ("Version/Library").toString();
    const QString plgPth = QString (WNTR_PLUGIN_PATH) + "/lib" + plgnLibrary + ".so";
    m_plgnLdr = new QPluginLoader (plgPth, Factory::instance());
    m_plgnLdr->setLoadHints (QLibrary::ResolveAllSymbolsHint);
    m_plgnLdr->load();

    if (!m_plgnLdr->isLoaded())
        qDebug() << "(plugin) [AbstractPlugin] Error loading library"
                 << plgPth << ":" << m_plgnLdr->errorString();

    return m_plgnLdr->isLoaded();
}

void AbstractPlugin::doStart()
{
    connect (Core::instance(), SIGNAL (stopped()) , this , SLOT (doStop()));
    start();
    emit started();
}

void AbstractPlugin::doStop()
{
    stop();
    emit stopped();
}

bool AbstractPlugin::loadRequiredComponents() const
{
    if (!isSupported ()) {
        qWarning() << "(plugin) [Factory] Plug-in" << name () << "is incompatible with this version of Wintermute.";
        return false;
    }
    else
        qDebug() << "(plugin) [Factory] Plug-in" << name () << "v." << version() << "is compatible with this version of Wintermute.";

    if (!loadPackages()) {
        qWarning() << "(plugin) [Factory] Can't load dependency packages for plug-in" << name() << ".";
        return false;
    }

    if (!loadPlugins()) {
        qWarning() << "(plugin) [Factory] Can't load dependency plug-ins for plug-in" << name() << ".";
        return false;
    }

    return true;
}

AbstractPlugin::~AbstractPlugin()
{
    m_plgnLdr->unload();
    m_plgnLdr->deleteLater();
    m_sttngs->deleteLater();
}

void AbstractPlugin::loadSettings (const QString& p_uuid)
{
    m_sttngs = Factory::getPluginSettings (p_uuid);
    m_cnfg = new QSettings ("Synthetic Intellect Institute", p_uuid);
}

#include "abstractplugin.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
