#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

namespace Ui { class MainWindow; }

class BatteryInfo;
class PciInfo;
class DiskInfo;

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
    DiskInfo *diskInfo;
    QMovie *movie;
};

#endif //MAINWINDOW_H