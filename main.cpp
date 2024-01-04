#include "driverapp.h"
#include "trayicon.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DriverApp w;
    w.show();

    TrayIcon icon;

    return a.exec();
}
