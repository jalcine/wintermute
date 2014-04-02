/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2011 - 2013 Jacky Alciné <me@jalcine.me>
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

#include <QCommandLine>
#include <QtCore/QVariantMap>
#include "logging.hpp"
#include "application.hpp"
#include "private/arguments.hpp"
#include "arguments.hpp"

using Wintermute::Arguments;
using Wintermute::ArgumentsPrivate;

Arguments* Arguments::self = 0;

// TODO: Add a means of registering new arguments to the command line?
Arguments::Arguments() : QObject ( Application::instance() ),
  d_ptr ( new ArgumentsPrivate ( this ) )
{
  Q_D ( Arguments );
  // Make this the object of concern.
  d->args->setParent ( this );
  // Connect necessary slots.
  connect ( d->args, SIGNAL ( switchFound ( const QString& ) ),
            this, SLOT ( switchFound ( const QString& ) ) );
  connect ( d->args, SIGNAL ( paramFound ( const QString&, const QVariant& ) ),
            this, SLOT ( paramFound ( const QString&, const QVariant& ) ) );
  connect ( d->args, SIGNAL ( optionFound ( const QString&, const QVariant& ) ),
            this, SLOT ( optionFound ( const QString&, const QVariant& ) ) );
  connect ( d->args, SIGNAL ( parseError ( const QString& ) ),
            this, SLOT ( parseError ( const QString& ) ) );
  // Read command line arguments.
  d->args->parse();
}

Arguments*
Arguments::instance()
{
  if ( !self ) {
    self = new Arguments;
  }
  return self;
}

bool
Arguments::hasArgument ( const QString& argumentName ) const
{
  Q_D ( const Arguments );
  return d->arguments.contains ( argumentName );
}

QVariant
Arguments::argument ( const QString& argumentName ) const
{
  Q_D ( const Arguments );
  if ( d->arguments.contains ( argumentName ) ) {
    return d->arguments.value ( argumentName );
  }
  return QVariant();
}

QVariantMap
Arguments::arguments() const
{
  Q_D ( const Arguments );
  return d->arguments;
}

void
Arguments::switchFound ( const QString& switchName )
{
  Q_D ( Arguments );
  d->arguments.insert ( switchName, true );
}

void
Arguments::paramFound ( const QString&  parameterName, const QVariant& parameterValue )
{
  Q_D ( Arguments );
  d->arguments.insert ( parameterName, parameterValue );
}

void
Arguments::optionFound ( const QString&  optionName, const QVariant& optionValue )
{
  Q_D ( Arguments );
  d->arguments.insert ( optionName, optionValue );
}

void
Arguments::parseError ( const QString& error )
{
  Wintermute::Logger* log = wlog ( this );
  log->error ( QString ( "Unrecognized command-line arguments. (%1)" ).arg ( error ) );
  wntrApp->stop();
  exit ( 1 );
}

void
Arguments::addOption ( const QChar& optionName, const QString& longOptionName, const QString& description, QCommandLine::Flags flag )
{
  Q_D ( Arguments );
  d->args->addOption ( optionName, longOptionName, description, flag );
}

void
Arguments::addSwitch ( const QChar& switchName, const QString& longSwitchName, const QString& description, QCommandLine::Flags flag )
{
  Q_D ( Arguments );
  d->args->addSwitch ( switchName, longSwitchName, description, flag );
}

void
Arguments::addParameter ( const QString& parameterName, const QString& description, QCommandLine::Flags flag )
{
  Q_D ( Arguments );
  d->args->addParam ( parameterName, description, flag );
}

Arguments::~Arguments()
{
}
