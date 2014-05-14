/**
 * @author Jacky Alciné <me@jalcine.me>
 * @copyright © 2011, 2012, 2013, 2014 Jacky Alciné <me@jalcine.me>
 * @if 0
 * Wintermute is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
 * @endif
 **/

#include <QtCore/QStringList>
#include <qjson/serializer.h>
#include <qjson/parser.h>
#include "Wintermute/logging.hpp"
#include "Wintermute/Procedure/module.hpp"

using QJson::Serializer;
using Wintermute::Procedure::Module;

const Module::Definition Module::Definition::Null =
  Module::Definition::compose(QString::null, QString::null, -1);

const int Module::Definition::MetaTypeId =
  qRegisterMetaType<Module::Definition>("Wintermute::Procedure::Module::Definition");

Module::Definition::Definition(const quint64& a_pid, const QString& a_package,
                               const QString& a_domain) : pid (a_pid), package(a_package), domain(a_domain)
{
}

Module::Definition::Definition() : pid(0), package(QString::null),
  domain(QString::null)
{
}

bool
Module::Definition::operator==(const Definition& definition) const
{
  if ( definition.package != this->package ) {
    return false;
  }

  if ( definition.domain  != this->domain ) {
    return false;
  }

  return true;
}

void
Module::Definition::operator=(const Definition& definition)
{
  package = definition.package;
  domain = definition.domain;
  pid = definition.pid;
}

// TODO Error checking on the JSON.
Module::Definition::operator QString() const
{
  Q_ASSERT ( valid() );
  QVariantMap params;
  Serializer jsonSerializer;
  jsonSerializer.setIndentMode(QJson::IndentCompact);
  params.insert("pid", pid);
  params.insert("package", package);
  params.insert("domain", domain);
  return jsonSerializer.serialize(params, 0);
}

Module::Definition
Module::Definition::fromString( const QString& string )
{
  Q_ASSERT ( string.isNull() );
  Q_ASSERT ( string.isEmpty() );
  QVariantMap map;
  QJson::Parser jsonParser;
  map = jsonParser.parse(string.toLocal8Bit(), 0).toMap();
  Q_ASSERT ( jsonParser.errorString().isEmpty() );
  Q_ASSERT ( jsonParser.errorString().isNull() );

  if (!jsonParser.errorString().isNull()) {
    return Definition::Null;
  }

  return compose(map.value("domain").toString(),
                 map.value("package").toString(),
                 map.value("pid").toUInt());
}

Module::Definition
Module::Definition::compose(const QString& domain, const QString& package,
                            const quint64& pid)
{
  Definition aDef(pid, package, domain);
  return aDef;
}

bool
Module::Definition::valid() const
{
  Q_ASSERT ( !domain.isEmpty() );
  Q_ASSERT ( !package.isEmpty() );
  Q_ASSERT ( !domain.isNull() );
  Q_ASSERT ( !package.isNull() );

  if ( package.isNull() ) {
    return false;
  }

  if ( package.isEmpty() ) {
    return false;
  }

  if ( domain.isNull() ) {
    return false;
  }

  if ( domain.isEmpty() ) {
    return false;
  }

  return true;
}

uint
qHash(const Module::Definition& definition)
{
  return qHash(static_cast<const QString>(definition));
}
