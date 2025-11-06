#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QScrollArea>
#include <QFormLayout>
#include "DiskInfo.h"
#include "ui_DiskInfo.h"

DiskInfo::DiskInfo(QWidget *parent) : QDialog(parent), ui(new Ui::DiskInfo) {
    ui->setupUi(this);

    setFixedSize(size());
    ui->splitter->setSizes({150, 500});

    connect(ui->listWidgetDevices, &QListWidget::currentRowChanged,
            ui->stackedWidgetDevices, &QStackedWidget::setCurrentIndex);
}

DiskInfo::~DiskInfo() {
    delete ui;
}

void DiskInfo::setDevices() {
    QFile file(":/data/diskinfo.csv");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл diskinfo.csv");
        return;
    }

    QTextStream in(&file);
    in.readLine();
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');
        addPage(fields);
    }

    isDevicesSet = true;
}

void DiskInfo::addPage(QStringList fields) const {
    QScrollArea *scroll = new QScrollArea;
    QWidget *container = new QWidget;
    QFormLayout *form = new QFormLayout(container);

    form->setContentsMargins(3, 0, 0, 0);
    form->setVerticalSpacing(3);
    container->setLayout(form);
    scroll->setWidget(container);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);

    form->addRow("Модель:", new QLabel(fields[0]));
    form->addRow("Серийный номер:", new QLabel(fields[1]));
    form->addRow("Версия прошивки:", new QLabel(fields[2]));

    QLabel *size = new QLabel;
    form->addRow("Размер (ГБ):", size);
    if (fields[3][0] == '0') size->setText("N/A");
    else size->setText(fields[3]);

    form->addRow("Тип интерфейса:", new QLabel(fields[4]));

    QString pioList = bitmaskToList(fields[5].toUInt(), 8);
    QString mwdmaList = bitmaskToList(fields[6].toUInt(), 3);
    QString umdaList = bitmaskToList(fields[7].toUInt(), 7);

    form->addRow("Поддерживаемые режимы PIO:", new QLabel(pioList));
    form->addRow("Поддерживаемые режимы MWDMA:", new QLabel(mwdmaList));
    form->addRow("Поддерживаемые режимы UMDA:", new QLabel(umdaList));

    ui->stackedWidgetDevices->addWidget(scroll);
    ui->listWidgetDevices->addItem(fields[0]);
}

QString DiskInfo::bitmaskToList(quint32 mask, int maxMode) {
    QStringList list;
    for (int i = 0; i < maxMode; i++) {
        if (mask & (1 << i)) list << QString::number(i);
    }
    return list.join(", ");
}

void DiskInfo::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);

    if (!isDevicesSet) setDevices();
}