#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include "PciInfo.h"
#include "ui_PciInfo.h"

PciInfo::PciInfo(QWidget *parent) : QDialog(parent), ui(new Ui::PciInfo){
    ui->setupUi(this);
    setFixedSize(size());
    ui->verticalLayout->setStretch(1, 1);
    ui->verticalLayout->setStretch(0, 3);
}

PciInfo::~PciInfo() {
    delete ui;
}

void PciInfo::setDevices() {
    QFile file(":/data/pciinfo.csv");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл pciinfo.csv");
        return;
    }

    QTextStream in(&file);
    in.readLine();
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');
        int row = ui->tableWidgetDevices->rowCount();
        ui->tableWidgetDevices->insertRow(row);
        for (int col = 0; col < fields.size(); col++) {
            ui->tableWidgetDevices->setItem(row, col, new QTableWidgetItem(fields[col]));
        }
    }

    isDevicesSet = true;
}

void PciInfo::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);

    if (!isDevicesSet) {
        int width = ui->tableWidgetDevices->viewport()->width();
        auto header = ui->tableWidgetDevices->horizontalHeader();

        ui->tableWidgetDevices->setColumnWidth(0, static_cast<int>(width * 0.67));
        ui->tableWidgetDevices->setColumnWidth(1, static_cast<int>(width * 0.15));
        ui->tableWidgetDevices->setColumnWidth(2, static_cast<int>(width * 0.15));
        header->setSectionResizeMode(QHeaderView::Fixed);
        header->setSectionsClickable(false);

        setDevices();
    }
}