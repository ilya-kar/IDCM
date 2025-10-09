#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

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
    void initMovie();
    Ui::MainWindow *ui;
    BatteryInfo *batteryInfo;
    QMovie *movie;
};

#endif //MAINWINDOW_H