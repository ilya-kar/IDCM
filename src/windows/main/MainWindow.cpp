#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->clickButton, &QPushButton::clicked, ui->clickButton, [=]() {
        ui->clickButton->setText("Clicked!");
    });
}

