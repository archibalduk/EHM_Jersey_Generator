#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // Application details
    QCoreApplication::setApplicationName("EHM Jersey Generator");
    QCoreApplication::setApplicationVersion("1.0.0-beta.2");
    QCoreApplication::setOrganizationName("EHM The Blue Line");
    QCoreApplication::setOrganizationDomain("ehmtheblueline.com");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
