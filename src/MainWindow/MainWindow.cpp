#include  <QMovie>
#include "ui_MainWindow.h"
#include "BatteryInfo.h"
#include "MainWindow.h"
#include "PciInfo.h"
#include "DiskInfo.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow) {
    batteryInfo = new BatteryInfo(this);
    pciInfo = new PciInfo(this);
    diskInfo = new DiskInfo(this);
    movie = new QMovie(":/images/tom.gif", QByteArray(), this);

    ui->setupUi(this);
    setFixedSize(size());
    movie->setCacheMode(QMovie::CacheAll);
    ui->labelMovie->setMovie(movie);
    movie->start();

    connect(ui->pushButtonStart, &QPushButton::clicked, this, &MainWindow::chooseWindow);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::chooseWindow() const {
    movie->setPaused(true);
    switch (ui->comboBoxLabSelect->currentIndex()) {
        case 0:
            batteryInfo->exec();
            break;
        case 1:
            pciInfo->exec();
            break;
        case 2:
            diskInfo->exec();
            break;
        default:
            break;
    }
    movie->setPaused(false);
}