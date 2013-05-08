/**
 * vim: ft=cpp
 * Copyright (C) 2013 Jacky Alcine <jacky.alcine@thesii.org>
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

#include "arguments.hpp"
#include "application.hpp"
#include "logging.hpp"
#include <QtCore/QVariantMap>
#include <QCommandLine>

using Wintermute::Arguments;
using Wintermute::ArgumentsPrivate;

namespace Wintermute {
  class ArgumentsPrivate {
    public:
      QCommandLine* args;
      QVariantMap arguments;

      ArgumentsPrivate(QObject* parent) : args (new QCommandLine(parent)){
        args->enableVersion(true);
        args->enableHelp(true);
      };

      void interpret() {
      };
  };
}

Arguments* Arguments::self = 0;

// TODO: Add a means of registering new arguments to the command line?
Arguments::Arguments() : QObject(Application::instance()), d_ptr(new ArgumentsPrivate(this)) {
  Q_D(Arguments);

  // Wipe the object, just to be safe.
  // TODO: Determine if this is even necessary.
  d->arguments.clear();

  // Make this the object of concern.
  d->args->setParent(this);

  // Connect necessary slots.
  connect(d->args,SIGNAL(switchFound(const QString&)),
      this,SLOT(switchFound(const QString &)));
  connect(d->args,SIGNAL(paramFound(const QString&, const QVariant&)),
      this,SLOT(paramFound(const QString &, const QVariant&)));
  connect(d->args,SIGNAL(optionFound(const QString&, const QVariant&)),
      this,SLOT(optionFound(const QString &, const QVariant&)));
  connect(d->args,SIGNAL(parseError(const QString&)),
      this,SLOT(parseError(const QString&)));

  // Read command line arguments.
  d->args->parse();
}

Arguments*
Arguments::instance(){
  if (!self)
    self = new Arguments;

  return self;
}

QVariant
Arguments::argument(const QString& argumentName) const {
  Q_D(const Arguments);

  if (d->arguments.contains(argumentName))
    return d->arguments.value(argumentName);

  return QVariant();
}

QVariantMap
Arguments::arguments() const {
  Q_D(const Arguments);

  return d->arguments;
}

void
Arguments::switchFound(const QString& switchName){
  Q_D(Arguments);

  d->arguments.insert(switchName, true);
}

void
Arguments::paramFound(const QString&  parameterName, const QVariant& parameterValue){
  Q_D(Arguments);
  d->arguments.insert(parameterName,parameterValue);
}

void
Arguments::optionFound(const QString&  optionName, const QVariant& optionValue){
  Q_D(Arguments);
  d->arguments.insert(optionName,optionValue);
}

void
Arguments::parseError(const QString& error){
  Wintermute::Logger* log = Wintermute::Logging::obtainLogger(this);
  Q_D(Arguments);

  log->error(QString("Malformed command-line arguments. (%1)").arg(error));

  d->args->showVersion();
  d->args->showHelp();
}

Arguments::~Arguments(){
}

#include "arguments.moc"
