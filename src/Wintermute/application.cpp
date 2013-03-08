#include "application.hpp"
#include "arguments.hpp"
#include "Wintermute/globals.hpp"
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

using Wintermute::Arguments;
using Wintermute::Application;
using Wintermute::ApplicationPrivate;

// TODO: Handle command-line arguments.
class ApplicationPrivate {
  public:
    QSharedPointer<QCoreApplication> app;

    ApplicationPrivate(int &argc, char **argv) {
      app = QSharedPointer<QCoreApplication>(new QCoreApplication(argc,argv));
    }

    void
    initialize(){
      Arguments* arguments = Arguments::self = new Arguments;
    }

    int
    exec(){
      return app->exec();
    }
};

Application* Application::self = 0;

Application::Application(int &argc, char **argv) : QObject(), d_ptr(new ApplicationPrivate(argc,argv)){
  Q_D(Application);

  // I am self!
  Application::self = qobject_cast<Application*>(this);

  // Define the application in Qt.
  d->app->setApplicationName("Wintermute");
  d->app->setApplicationVersion(WINTERMUTE_VERSION_EXACT);
  d->app->setOrganizationName("Synthetic Intellect Institute");
  d->app->setOrganizationDomain("thesii.org");
}

int
Application::run(int &argc, char **argv){
  if (Application::instance() == 0){
    // Define the application.
    Application::self = new Application(argc,argv);

    // Invoke the initialization code.
    self->d_ptr->initialize();

    // Begin the event loop.
    return self->d_ptr->exec();
  }
  return -1;
}

Application::~Application(){
}

#include "application.moc"
