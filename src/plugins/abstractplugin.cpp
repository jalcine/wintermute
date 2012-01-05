// Local
#include "config.hpp"
#include "abstractplugin.hpp"
#include "factory.hpp"

// Qt
#include <QApplication>
#include <QDebug>
#include <QFile>

// Namespaces
using namespace Wintermute::Plugins;

// Constructors
AbstractPlugin::AbstractPlugin(QPluginLoader *p_pl) : QObject(p_pl), m_plgnLdr(p_pl), m_settings(NULL) {}

AbstractPlugin::AbstractPlugin(const AbstractPlugin &p_pb): QObject(p_pb.m_plgnLdr),
                   m_plgnLdr(p_pb.m_plgnLdr), m_settings(p_pb.m_settings), m_config(p_pb.m_config) {}

// Access methods
QString AbstractPlugin::author() const { return m_settings->value ("Description/Author").toString(); }

QString AbstractPlugin::name() const { return m_settings->value ("Description/Name").toString(); }

QString AbstractPlugin::vendorName() const { return m_settings->value ("Description/Vendor").toString(); }

QString AbstractPlugin::uuid() const { return m_settings->value ("Version/UUID").toString(); }

QString AbstractPlugin::description() const { return m_settings->value ("Description/Blurb").toString(); }

QString AbstractPlugin::webPage() const { return m_settings->value ("Description/WebPage").toString(); }

double AbstractPlugin::version() const { return m_settings->value ("Version/Plugin").toDouble(); }

double AbstractPlugin::compatVersion() const { return m_settings->value ("Version/Compat", WNTR_VERSION).toDouble(); }

bool AbstractPlugin::isSupported() const { return WNTR_VERSION >= compatVersion(); }

QStringList AbstractPlugin::plugins() const
{
    QStringList l_dep = m_settings->value("Depends/Plugins").toStringList();
    l_dep.removeDuplicates();
    l_dep.removeAll("None");
    return l_dep;
}

bool AbstractPlugin::loadPlugins() const
{
    const QStringList l_plgnLst = this->plugins();
    qDebug () << "(core) [AbstractPlugin] Loading plug-ins for"
              << name () << ";"
              << l_plgnLst.length () << "plugin(s) to be loaded.";

    foreach (const QString l_plgn, l_plgnLst)
    {
        const QString l_plgnUuid = l_plgn.split(" ").at(0);
        if (Factory::loadedPlugins().contains(l_plgnUuid))
            qDebug () << "(core) [AbstractPlugin] Dependency"
                      << Factory::attribute(l_plgnUuid,"Description/Name").toString()
                      << "already loaded.";
        else {
            Factory::GenericPlugin *l_gnrc = new Factory::GenericPlugin(l_plgnUuid);
            qDebug() << "(core) [AbstractPlugin] Loading dependency"
                     << l_gnrc->name() << "...";

            if (l_gnrc->loadLibrary() && QFile::exists(l_gnrc->m_plgnLdr->fileName()))
                qWarning() << "(core) [AbstractPlugin] Loaded symbols of plug-in"
                           << Factory::attribute(l_plgnUuid,"Description/Name").toString() << ".";
            else {
                qWarning() << "(core) [AbstractPlugin] Unable to load symbols of depedency"
                           << l_gnrc->name() << ":" << l_gnrc->m_plgnLdr->errorString();
                return false;
            }
        }
    }
    return true;
}

bool AbstractPlugin::hasPlugins() const
{
    const QStringList l_deps = this->plugins();
    foreach (const QString l_dep, l_deps)
    {
        const QString l_depName = l_dep.split(" ").at(0);
        const QString l_depComparison = l_dep.split(" ").at(1);
        const QString l_depVersion = l_dep.split(" ").at(2);

        if (Factory::allPlugins().contains(l_depName)) {
            if (!Factory::loadedPlugins().contains(l_depName))
                qDebug() << "(core) [AbstractPlugin] Dependency" << l_depName
                         << "of" << this->name () << "isn't loaded.";

            const AbstractPlugin *l_plgn = Factory::s_plugins.value(l_depName);
            if (l_depComparison == "==") {
                if (!(l_depVersion.toDouble() == l_plgn->version())) {
                    qDebug() << "(core) [AbstractPlugin] " << this->name()
                             << "requires" << l_depName
                             << "to have a version of" << l_depVersion;
                    return false;
                }
            } else if (l_depComparison == ">") {
                if (!(l_depVersion.toDouble () > l_plgn->version())) {
                    qDebug() << "(core) [AbstractPlugin] " << this->name ()
                             << "requires" << l_depName
                             << "to have a version greater than" << l_depVersion;
                    return false;
                }
            } else if (l_depComparison == "<") {
                if (!(l_depVersion.toDouble () < l_plgn->version ())) {
                    qDebug() << "(core) [AbstractPlugin] " << this->name()
                             << "requires" << l_depName << "to have a version less than" << l_depVersion;
                    return false;
                }
            } else if (l_depComparison == ">=") {
                if (!(l_depVersion.toDouble () >= l_plgn->version ())) {
                    qDebug() << "(core) [AbstractPlugin] " << this->name()
                             << "requires" << l_depName
                             << "to have a version of at least" << l_depVersion;
                    return false;
                }
            } else if (l_depComparison == "==") {
                if (!(l_depVersion.toDouble () > l_plgn->version ())) {
                        qDebug() << "(core) [AbstractPlugin] " << this->name()
                                 << "requires" << l_depName << "to have a version of at most" << l_depVersion;
                        return false;
                    }
            } else {
                qDebug() << "(core) [AbstractPlugin] <" << this->name()
                         << "> : Invalid version string ("
                         << l_depComparison << ").";
            }
        }
    }

    return true;
}

QStringList AbstractPlugin::packages() const
{
    QStringList l_dep = m_settings->value("Depends/Packages").toStringList();
    l_dep.removeDuplicates();
    l_dep.removeAll("None");
    return l_dep;
}

/// @note This method requires code from QPackageKit.
/// @todo Add code using QPackageKit's API to detect if packages are installed on a system and if not, install them.
/// @todo Decide if we should use triples of (QString, ConstrType, Int) or (QString, ConstrType, QString) for dependency name, constraint
/// type and version, respectively.
bool AbstractPlugin::loadPackages() const
{
    const QStringList l_deps = this->packages();
    //qDebug () << "(core) [AbstractPlugin] Loading packages for" << name () << ";" << l_deps.length () << "package(s) to be loaded.";
    foreach (const QString l_dep, l_deps) {
        const QString l_depName = l_dep.split(" ").at(0);
    }

    return true;
}

/// @note This method requires code from QPackageKit.
/// @todo Add code using QPackageKit's API to detect if packages are installed on a system.
bool AbstractPlugin::hasPackages() const
{
    const QStringList l_deps = this->packages();

    foreach (const QString l_dep, l_deps) {
        const QString l_depName = l_dep.split(" ").at(0);
        const QString l_depComparison = l_dep.split(" ").at(1);
        const QString l_depVersion = l_dep.split(" ").at(2);
    }

    return true;
}

/// @note This method searches the plug-in's configuration for default settings. \o/
QVariant AbstractPlugin::attribute(const QString &p_attrPath) const
{
    QVariant l_val = m_config->value(p_attrPath);

    if (l_val.isNull() || !l_val.isValid())
        l_val = m_config->value("Configuration/" + const_cast<QString *>(&p_attrPath)->replace("/",":"));

    return l_val;
}

void AbstractPlugin::setAttribute(const QString &p_attrPath, const QVariant &p_attrVal)
{
    m_config->setValue(p_attrPath, p_attrVal);
}

/// @todo Allow resetting of specific attributes.
void AbstractPlugin::resetAttributes()
{
    m_config->clear();
}

bool AbstractPlugin::loadLibrary() const
{
    QApplication::addLibraryPath(WNTR_PLUGIN_PATH);
    const QString l_plgnLibrary = m_settings->value("Version/Library").toString();
    const QString l_plgPth = QString(WNTR_PLUGIN_PATH) + "/lib" + l_plgnLibrary + ".so";
    m_plgnLdr = new QPluginLoader ( l_plgPth, Factory::instance() );
    m_plgnLdr->setLoadHints( QLibrary::ResolveAllSymbolsHint );
    m_plgnLdr->load();
    if (!m_plgnLdr->isLoaded())
        qDebug() << "(plugin) [AbstractPlugin] Error loading library"
                 << l_plgPth << ":" << m_plgnLdr->errorString();
    return m_plgnLdr->isLoaded();
}

AbstractPlugin::~AbstractPlugin()
{
    m_plgnLdr->unload();
    m_plgnLdr->deleteLater();
    m_settings->deleteLater();
}
