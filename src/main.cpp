#include "mainwindow.h"
#include "consts.h"

#include <QGuiApplication>
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
  QCoreApplication::setApplicationName(APP_NAME);
  QCoreApplication::setApplicationVersion(APP_VERSION);

  QApplication app(argc, argv);
  app.setWindowIcon(QIcon(APP_ICON));

  MainWindow win;
  win.loadSettings();
  win.init();
  win.show();
  win.initTable();

//  win.tst();

  return app.exec();
}
