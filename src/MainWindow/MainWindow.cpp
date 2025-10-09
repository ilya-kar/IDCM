#include  <QMovie>
#include "ui_MainWindow.h"
#include "BatteryInfo.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    ui = new Ui::MainWindow;
    batteryInfo = new BatteryInfo(this);
    ui->setupUi(this);
    initMovie();
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
        default:
            break;
    }
    movie->setPaused(false);
}

void MainWindow::initMovie() {
    movie = new QMovie(":/images/tom.gif", QByteArray(), this);
    movie->setCacheMode(QMovie::CacheAll);
    ui->labelMovie->setMovie(movie);
    movie->start();
}