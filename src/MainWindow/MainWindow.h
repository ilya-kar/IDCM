#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "PciInfo.h"

namespace Ui { class MainWindow; }

class BatteryInfo;

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    void chooseWindow() const;

private:
    Ui::MainWindow *ui;
    BatteryInfo *batteryInfo;
    PciInfo *pciInfo;
    QMovie *movie;
};

#endif //MAINWINDOW_H