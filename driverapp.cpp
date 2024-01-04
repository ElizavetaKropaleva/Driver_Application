#include "driverapp.h"
#include "ui_driverapp.h"

DriverApp::DriverApp(QWidget *parent) : QMainWindow(parent), ui(new Ui::DriverApp)
{
    ui->setupUi(this);

    // повышаем привилегии приложения
    if (SetPrivilege()) {
        qDebug() << "SetPrivilege(): Success";
    } else {
        qDebug() << "SetPrivilege(): Error";
    }

    // добавляем сигналы для кнопок загрузки и выгрузки драйвера
    connect(ui->LoadButton, SIGNAL (clicked()), this, SLOT (load()));
    connect(ui->UnloadButton, SIGNAL (clicked()), this, SLOT (unload()));

    //
    connect(ui->SendButton, SIGNAL (clicked()), this, SLOT (sendMessage()));
}

void DriverApp::load()
{
    HRESULT resultLoad = FilterLoad(L"passThrough");

    // проверка ошибок
    if (IS_ERROR(resultLoad)) {
        std::string error = std::system_category().message(resultLoad);
        ui->statusLabel->setText("🛈 Ошибка: " + QString::fromLocal8Bit(error.c_str()));
    } else {
        ui->statusLabel->setText("🛈 Успех: драйвер успешно загружен");
    }
}

void DriverApp::unload()
{
    HRESULT resultUnload = FilterUnload(L"passThrough");

    // проверка ошибок
    if (IS_ERROR(resultUnload)) {
        std::string error = std::system_category().message(resultUnload);
        ui->statusLabel->setText("🛈 Ошибка: " + QString::fromLocal8Bit(error.c_str()));
    } else {
        ui->statusLabel->setText("🛈 Успех: драйвер успешно выгружен");
    }
}

bool DriverApp::connectPort()
{
    HRESULT hResult = FilterConnectCommunicationPort(L"\\passThrough", NULL, NULL, NULL, NULL, &hPort);

    // проверка ошибок
    if (IS_ERROR(hResult)) {
        std::string error = std::system_category().message(hResult);
        ui->statusLabel->setText("🛈 Ошибка: " + QString::fromLocal8Bit(error.c_str()));
        return false;
    }

    return true;
}

void DriverApp::disconnectPort()
{
    if (NULL != hPort) {
        CloseHandle(hPort);
        hPort = NULL;
    }
}

void DriverApp::sendMessage()
{
    if (connectPort()) {
        DWORD BytesReturned;
        char key[16];
        char iv[16];
        char text[32];

        QString text_key = ui->KeyField->text();
        strncpy(key, qPrintable(text_key), 15);
        qDebug() << key;

        QString text_iv = ui->IvField->text();
        strncpy(iv, qPrintable(text_iv), 15);
        qDebug() << iv;

        strcpy(text, key);
        strcat(text, iv);
        qDebug() << text;

        HRESULT hResult = FilterSendMessage(hPort, text, MESSAGE_SIZE, NULL, NULL, &BytesReturned);

        // проверка ошибок
        if (IS_ERROR(hResult)) {
            std::string error = std::system_category().message(hResult);
            ui->statusLabel->setText("🛈 Ошибка: " + QString::fromLocal8Bit(error.c_str()));
        } else {
            ui->statusLabel->setText("🛈 Успех: сообщение отправлено");
        }

        disconnectPort();
    }
}

bool DriverApp::SetPrivilege()
{
    HANDLE token; // дескриптор токена доступа
    HANDLE currentProcess;
    TOKEN_PRIVILEGES tokenPrivilege; // привилегии для токена доступа
    LUID luid; // локальный уникальный идентификатор

    currentProcess = GetCurrentProcess();
    OpenProcessToken(currentProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token); // открываем маркер доступа, связанный с процессом

    if (!LookupPrivilegeValue(NULL, SE_LOAD_DRIVER_NAME, &luid)) { // извлекаеv LUID для локального представления указанного имени привилегии
        return FALSE;
    }

    tokenPrivilege.PrivilegeCount = 1;
    tokenPrivilege.Privileges[0].Luid = luid;
    tokenPrivilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; // включении привилегии

    if (!AdjustTokenPrivileges(token, FALSE, &tokenPrivilege, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES) NULL, (PDWORD) NULL)) { // включаем привилегии в указанном токене доступа
        return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED){
        return FALSE;
    }

    return TRUE;
}

DriverApp::~DriverApp()
{
    delete ui;
}


