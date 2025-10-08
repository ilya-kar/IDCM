#include <QTimer>
#include <QMessageBox>
#include <windows.h>
#include <powrprof.h>
#include <devguid.h>
#include <batclass.h>
#include <setupapi.h>
#include "ui_BatteryInfo.h"
#include "BatteryInfo.h"

const QString BatteryInfo::INITIAL_LINE = "Чтение...";

BatteryInfo::BatteryInfo(QWidget *parent) : QDialog(parent), ui(new Ui::BatteryInfo) {
    ui->setupUi(this);
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &BatteryInfo::updateBatteryInfo);
    connect(ui->pushButtonSleep, &QPushButton::clicked, this, [this] {
        goSleep();
    });
    connect(ui->pushButtonHibernate, &QPushButton::clicked, this, [this] {
        goHibernate();
    });
}

BatteryInfo::~BatteryInfo() {
    delete ui;
}

void BatteryInfo::updateBatteryInfo() {
    SYSTEM_POWER_STATUS sps;
    if (!GetSystemPowerStatus(&sps)) {
        ui->labelPowerSource->setText("-");
        ui->labelPowerState->setText("-");
        ui->labelPowerMode->setText("-");
        ui->labelCapacity->setText("-");
        ui->labelBatteryType->setText("-");
        ui->labelRemainingTime->setText("-");
        QMessageBox::critical(this, "Ошибка", "Не удалось получить информацию о батареи");
        timer->stop();
        return;
    }

    switch (sps.ACLineStatus) {
        case 0: ui->labelPowerSource->setText("От аккумулятора"); break;
        case 1: ui->labelPowerSource->setText("От сети"); break;
        default: ui->labelPowerSource->setText("Неизвестно"); break;
    }

    if (sps.BatteryFlag == 255) ui->labelPowerState->setText("Неизвестно");
    else if (sps.BatteryFlag & 128)  ui->labelPowerState->setText("-");
    else if (sps.ACLineStatus == 1) ui->labelPowerState->setText("Зарядка");
    else ui->labelPowerState->setText("Разрядка");

    if (sps.BatteryLifePercent == 255) {
        if (sps.BatteryFlag & 128) ui->labelCapacity->setText("-");
        else ui->labelCapacity->setText("Неизвестно");
    }
    else if (sps.BatteryLifePercent == 100) ui->labelCapacity->setText("Полностью заряжен");
    else ui->labelCapacity->setText(QString::number(sps.BatteryLifePercent));

    if (sps.BatteryLifeTime == -1) ui->labelRemainingTime->setText("-");
    else {
        DWORD hours = sps.BatteryLifeTime / 3600;
        DWORD minutes = (sps.BatteryLifeTime / 60) % 60;
        DWORD seconds = sps.BatteryLifeTime % 60;

        QString timeStr = QString("%1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));

        ui->labelRemainingTime->setText(timeStr);
    }

    ui->labelPowerMode->setText(getCurPowerMode());
}

void BatteryInfo::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);
    timer->start();
    QTimer::singleShot(1000, this, [this] {
        ui->labelBatteryType->setText(getBatteryType());
    });
}

void BatteryInfo::hideEvent(QHideEvent *event) {
    QDialog::hideEvent(event);
    reset();
    timer->stop();
}

void BatteryInfo::goSleep() {
    if (!SetSuspendState(FALSE, FALSE, FALSE)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось перейти в режим сна");
    }
}

void BatteryInfo::goHibernate() {
    if (!SetSuspendState(TRUE, FALSE, FALSE)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось перейти в режим гибернаци");
    }
}

QString BatteryInfo::getCurPowerMode() {
    GUID *activeScheme = nullptr;
    QString result = "Неизвестно";

    if (PowerGetActiveScheme(nullptr, &activeScheme) == ERROR_SUCCESS) {
        DWORD size;
        DWORD res = PowerReadFriendlyName(nullptr, activeScheme, nullptr,
                                          nullptr, nullptr, &size);

        if (res == ERROR_SUCCESS && size > 0) {
            std::vector<UCHAR> buffer(size);

            if (PowerReadFriendlyName(nullptr, activeScheme, nullptr,
                                      nullptr, buffer.data(), &size) == ERROR_SUCCESS) {
                result = QString::fromWCharArray(reinterpret_cast<WCHAR*>(buffer.data()));
            }
        }
    }

    if (activeScheme) LocalFree(activeScheme);

    return result;
}

QString BatteryInfo::getBatteryType() {
    QString result = "Аккумулятор отсутствует";

    HDEVINFO hdev = SetupDiGetClassDevs(&GUID_DEVCLASS_BATTERY, nullptr, nullptr,
                                        DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (hdev == INVALID_HANDLE_VALUE) return result;

    SP_DEVICE_INTERFACE_DATA did{};
    did.cbSize = sizeof(did);

    if (!SetupDiEnumDeviceInterfaces(hdev, nullptr, &GUID_DEVCLASS_BATTERY, 0, &did)) {
        SetupDiDestroyDeviceInfoList(hdev);
        return result;
    }

    result = "Неизвестно";

    DWORD required = 0;
    SetupDiGetDeviceInterfaceDetail(hdev, &did, nullptr, 0, &required, nullptr);

    std::vector<BYTE> buffer(required);
    auto pdidd = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(buffer.data());
    pdidd->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

    if (!SetupDiGetDeviceInterfaceDetail(hdev, &did, pdidd, required, &required, nullptr)) {
        SetupDiDestroyDeviceInfoList(hdev);
        return result;
    }

    HANDLE hBattery = CreateFile(pdidd->DevicePath, GENERIC_READ | GENERIC_WRITE,
                                 FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                                 OPEN_EXISTING, 0, nullptr);
    if (hBattery == INVALID_HANDLE_VALUE) {
        SetupDiDestroyDeviceInfoList(hdev);
        return result;
    }

    DWORD tag = 0, out;
    if (!DeviceIoControl(hBattery, IOCTL_BATTERY_QUERY_TAG, &tag, sizeof(tag),
                         &tag, sizeof(tag), &out, nullptr) || !tag) {
        CloseHandle(hBattery);
        SetupDiDestroyDeviceInfoList(hdev);
        return result;
    }

    BATTERY_QUERY_INFORMATION bqi{};
    bqi.BatteryTag = tag;
    bqi.InformationLevel = BatteryInformation;
    BATTERY_INFORMATION bi{};

    if (DeviceIoControl(hBattery, IOCTL_BATTERY_QUERY_INFORMATION, &bqi, sizeof(bqi),
                        &bi, sizeof(bi), &out, nullptr)) {
        char chem[5] = {};
        memcpy(chem, bi.Chemistry, 4);
        result = QString(chem);

        if (result == "LIon") result = "Li-ion";
        else if (result == "LiP") result = "Li-pol";
        else if (result == "NiMH") result = "NiMH";
        else if (result == "NiCd") result = "NiCd";
        else if (result == "PbAc") result = "Lead-acid";
        else if (result == "NiZn") result = "NiZn";
    }

    CloseHandle(hBattery);
    SetupDiDestroyDeviceInfoList(hdev);

    return result;
}

void BatteryInfo::reset() const {
    ui->labelPowerSource->setText(INITIAL_LINE);
    ui->labelPowerState->setText(INITIAL_LINE);
    ui->labelPowerMode->setText(INITIAL_LINE);
    ui->labelCapacity->setText(INITIAL_LINE);
    ui->labelBatteryType->setText(INITIAL_LINE);
    ui->labelRemainingTime->setText(INITIAL_LINE);
}