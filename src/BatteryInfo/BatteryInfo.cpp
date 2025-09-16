#include <QTimer>
#include "ui_BatteryInfo.h"
#include "BatteryInfo.h"

BatteryInfo::BatteryInfo(QWidget *parent) : QDialog(parent), ui(new Ui::BatteryInfo), sps(0) {
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &BatteryInfo::updateBatteryInfo);
    updateBatteryInfo();
    timer->start(1000);
}

BatteryInfo::~BatteryInfo() {
    delete ui;
}

void BatteryInfo::updateBatteryInfo() {
    GetSystemPowerStatus(&sps);
    updateACLineStatus(sps.ACLineStatus);
    updatePowerState(sps.BatteryFlag);
    updateLifePercent(sps.BatteryLifePercent);
    updateLifeTime(sps.BatteryLifeTime);
}

void BatteryInfo::updateACLineStatus(const BYTE status) const {
    if (!status) {
        ui->powerSource->setText("От аккумулятора");
        ui->powerState->setText("Разрядка");
    }
    else if (status == 1) {
        ui->powerSource->setText("От сети");
        ui->powerState->setText("Зарядка");
    }
    else {
        ui->powerSource->setText("Неизвестно");
        ui->powerState->setText("Неизвестно");
    }
}

void BatteryInfo::updatePowerState(const BYTE state) const {
    if (state == 128) ui->powerState->setText("Аккумулятор отсуствует");
    else if (state == 255) ui->powerState->setText("Неизвестно");
}

void BatteryInfo::updateLifePercent(const BYTE percent) const {
    if (percent == 255) ui->powerCapacity->setText("Неизвестно");
    else if (percent == 100) ui->powerCapacity->setText("Полностью заряжен");
    else ui->powerCapacity->setText(QString::number(percent));
}

void BatteryInfo::updateLifeTime(const DWORD time) const {
    if (time == -1) ui->powerTime->setText("");
    else {
        DWORD hours = time / 3600;
        int minutes = (time / 60) % 60;
        int seconds = time % 60;

        QString timeStr = QString("%1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));

        ui->powerTime->setText(timeStr);
    }
}