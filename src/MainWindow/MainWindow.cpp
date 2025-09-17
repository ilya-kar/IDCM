#include "ui_MainWindow.h"
#include "BatteryInfo.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    ui = new Ui::MainWindow;
    batteryInfo = new BatteryInfo(this);
    ui->setupUi(this);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::chooseWindow);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::chooseWindow() const {
    switch (ui->labSelect->currentIndex()) {
        case 0:
            batteryInfo->open();
            break;
        default:
            break;
    }
}
