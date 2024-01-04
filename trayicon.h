#ifndef TRAYICON_H
#define TRAYICON_H

#include <QMainWindow>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <string>
#include <system_error>
#include <windows.h>
#include <fltuser.h>
#include "qcoreapplication.h"

class TrayIcon : public QMainWindow
{
    Q_OBJECT

public:
    explicit TrayIcon(QWidget *parent = nullptr);

private slots:
    void load();
    void unload();

};

#endif // TRAYICON_H
