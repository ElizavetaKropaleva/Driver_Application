#include "trayicon.h"

TrayIcon::TrayIcon(QWidget *parent) : QMainWindow{parent}
{
    QMenu *menu = new QMenu();
    menu->addAction("Загрузить драйвер", this, SLOT(load()));
    menu->addAction("Выгрузить драйвер", this, SLOT(unload()));

    QSystemTrayIcon *icon = new QSystemTrayIcon();
    icon->setIcon(QIcon(QCoreApplication::applicationDirPath() + "/icon.png"));
    icon->setContextMenu(menu);
    icon->setToolTip("Управление драйвером passThrough");
    icon->show();
}

void TrayIcon::load()
{
    HRESULT resultLoad = FilterLoad(L"passThrough");
    if (IS_ERROR(resultLoad)) {
        std::string error = std::system_category().message(resultLoad);
        QMessageBox::critical(nullptr, "Ошибка", QString::fromLocal8Bit(error.c_str()));
    } else {
        QMessageBox::information(nullptr, "Успех", "Драйвер успешно загружен");
    }
}

void TrayIcon::unload()
{
    HRESULT resultUnload = FilterUnload(L"passThrough");
    if (IS_ERROR(resultUnload)) {
        std::string error = std::system_category().message(resultUnload);
        QMessageBox::critical(nullptr, "Ошибка", QString::fromLocal8Bit(error.c_str()));
    } else {
        QMessageBox::information(nullptr, "Успех", "Драйвер успешно выгружен");
    }
}
