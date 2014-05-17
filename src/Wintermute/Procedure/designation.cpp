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
#include "Wintermute/Procedure/designation.hpp"

using QJson::Serializer;
using Wintermute::Procedure::Designation;

const Designation Designation::Null =
  Designation::compose(QString::null, QString::null, 0);

const int Designation::MetaTypeId =
  qRegisterMetaType<Designation>("Wintermute::Procedure::Designation");

Designation::Designation(const quint64& a_pid, const QString& a_package,
                         const QString& a_domain) : pid (a_pid), package(a_package), domain(a_domain)
{
}

Designation::Designation() : pid(0), package(QString::null),
  domain(QString::null)
{
}

bool
Designation::operator==(const Designation& designation) const
{
  if ( designation.package != this->package ) {
    return false;
  }

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

bool
Designation::operator!=(const Designation& designation) const
{
  return !(*this == designation);
}

void
Designation::operator=(const Designation& designation)
{
  package = designation.package;
  domain = designation.domain;
  pid = designation.pid;
  qDebug() << "Transferred" << designation.package << "~" << package
           << ";" << designation.domain << "~" << domain
           << ";" << designation.pid << "~" << pid;
}

// FIXME: Error checking on the JSON.
Designation::operator QString() const
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

Designation
Designation::fromString( const QString& string )
{
  Q_ASSERT ( string.isNull() );
  Q_ASSERT ( string.isEmpty() );
  QVariantMap map;
  QJson::Parser jsonParser;
  map = jsonParser.parse(string.toLocal8Bit(), 0).toMap();
  Q_ASSERT ( jsonParser.errorString().isEmpty() );
  Q_ASSERT ( jsonParser.errorString().isNull() );

  if (!jsonParser.errorString().isNull()) {
    return Designation::Null;
  }

  return compose(map.value("domain").toString(),
                 map.value("package").toString(),
                 map.value("pid").toUInt());
}

Designation
Designation::compose(const QString& domain, const QString& package,
                     const quint64& pid)
{
  Designation aDef(pid, package, domain);
  return aDef;
}

bool
Designation::isNull() const
{
  return *this == Designation::Null;
}

bool
Designation::valid() const
{
  Q_ASSERT ( domain.isEmpty() == false );
  Q_ASSERT ( package.isEmpty() == false );
  Q_ASSERT ( domain.isNull() == false );
  Q_ASSERT ( package.isNull() == false );
  return !domain.isEmpty() && !domain.isNull() &&
         !package.isEmpty() && !package.isNull();
}
