#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Graphics building software");
    app.setApplicationVersion("dev_" + QDateTime::currentDateTime().toString("yyyyMMddHHmmss"));
    app.setWindowIcon(QIcon(":/img/plot.png"));

    MainWindow win;

    win.loadSettings();

    win.init();
    win.show();

    win.initTable();

    return app.exec();
}
