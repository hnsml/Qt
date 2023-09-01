#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Встановлення іконки для головного вікна програми
    QIcon icon(":/images/icon.png");
    a.setWindowIcon(icon);

    MainWindow w;
    w.show();
    return a.exec();
}
