#ifndef DRIVERAPP_H
#define DRIVERAPP_H

#include <QMainWindow>
#include <string>
#include <system_error>
#include <cstring>
#include <windows.h>
#include <fltuser.h>

QT_BEGIN_NAMESPACE
namespace Ui { class DriverApp; }
QT_END_NAMESPACE

class DriverApp : public QMainWindow
{
    Q_OBJECT

public:
    DriverApp(QWidget *parent = nullptr);
    ~DriverApp();
    bool SetPrivilege();
    bool connectPort();
    void disconnectPort();

private slots:
    void load();
    void unload();
    void sendMessage();

private:
    Ui::DriverApp *ui;
    HANDLE hPort = NULL;
    int MESSAGE_SIZE = 32;
};
#endif // DRIVERAPP_H
