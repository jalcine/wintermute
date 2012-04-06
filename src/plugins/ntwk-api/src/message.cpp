/*
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

 * @file message.cpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 * @date April 3, 2011, 10:10 AM
 */

#include "message.hpp"
#include <qjson/qobjecthelper.h>
#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <QtDebug>

namespace Wintermute {
namespace Network {
long Message::s_count = 0;

Message::Message () : m_attr() {
    __init ( );
}

Message::Message ( const Message &p_msg ) : m_attr ( p_msg.m_attr ) { }

Message::Message ( const QVariantMap& p_attr ) : m_attr ( p_attr ) {
    __init();
}

Message::Message ( const QString& property, const QVariant& value ) {
    __init ( );
    this->setAttribute ( property.toStdString().c_str(),value );
}

Message::~Message () { }

void Message::__init () {
    QDateTime now = QDateTime::currentDateTimeUtc ();
    qDebug() << "(ntwk) [Message] Generated message #" << Message::s_count << "; created on" << now << ".";
    this->setAttribute ( "TimeStamp" , now );
    Message::s_count++;
}

const QDateTime Message::creationTime () const {
    QVariant vrt = this->attribute ( "TimeStamp" ).toDateTime();
    if ( vrt.isValid () ) return vrt.toDateTime ();
    else QDateTime::currentDateTimeUtc();
}

const QString Message::type () const {
    QVariant vrt = this->attribute ( "MessageType" ).toDateTime();
    if ( vrt.isValid () ) return vrt.toString ();
    else return "Invalid";
}

const QString Message::toString () const {
    QVariant thisVrnt = qVariantFromValue ( *this );
    return thisVrnt.toString();
}

QVariant& Message::attribute ( const QString &p_attr ) const {
    return * ( new QVariant ( m_attr.value ( p_attr ) ) );
}

void Message::setAttribute ( const QString &p_attrPth, const QVariant &p_attrVal ) {
    m_attr.insert ( p_attrPth,p_attrVal );
}

QDataStream & operator >> ( QDataStream &p_strm, Message &p_msg ) {
    p_strm >> p_msg.m_attr;
    return p_strm;
}

QDataStream & operator << ( QDataStream &p_strm, const Message &p_msg ) {
    p_strm << p_msg.m_attr;
    return p_strm;
}

QDBusArgument& operator << ( QDBusArgument &p_arg, const Message& p_msg ) {
}

QDBusArgument& operator >> ( const QDBusArgument& p_arg, Message& p_msg ) {
}

Message* Message::fromString ( const QString& serializedText ) {
    return new Message ( QVariant::fromValue ( serializedText ).toMap() );
}

Message* Message::fromVariantMap ( const QVariantMap& p_attr ) {
    return new Message ( p_attr );
}
}
}

#include "message.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
