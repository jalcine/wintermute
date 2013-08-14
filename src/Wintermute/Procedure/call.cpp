/**
 * vim: ft=qt.cpp
 * Copyright (C) 2013 Jacky Alcine <me@jalcine.me>
 *
 * This file is part of Wintermute, the extensible AI platform.
 *
 *
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
**/

#include "Wintermute/Procedure/call.hpp"
#include "Wintermute/private/Procedure/call.hpp"

using Wintermute::Procedure::Call;

Call::Call(QObject* parent) : QObject(parent){
}

QVariant
Call::dispatch(QVariantList& arguments){
  // TODO: Find the module that would invoke this call.
  // TODO: Pass the calling work to it.
  return QVariant();
}

void
Call::setRecipient(const QString& moduleName){

}

Call::Type
Call::type() const {
  return Call::Method;
}

QString
Call::recipient() const {
  return QString();
}

QString
Call::toString() const {
  return QString();
}

Call::~Call() {
}

#include "Wintermute/Procedure/call.moc"
