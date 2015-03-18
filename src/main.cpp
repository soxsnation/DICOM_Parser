#include "mainwindow.h"
#include <QtGui/QApplication>

//using namespace Decimal::DICOM_Parser;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
