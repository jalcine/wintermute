#include "application.hpp"
#include "Wintermute/globals.hpp"
#include <QtCore/QCoreApplication>

using Wintermute::Application;
using Wintermute::ApplicationPrivate;

class ApplicationPrivate {
  public:
    QSharedPointer<QCoreApplication> app;

    ApplicationPrivate(int &argc, char **argv) {
      app = QSharedPointer<QCoreApplication>(new QCoreApplication(argc,argv));
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

Application*
Application::instance() {
  return Application::self;
}

int
Application::run(int &argc, char **argv){
  if (Application::instance() == 0){
    Application::self = new Application(argc,argv);
    return Application::instance()->d_ptr->app->exec();
  }
  return -1;
}

Application::~Application(){
}

#include "application.moc"
