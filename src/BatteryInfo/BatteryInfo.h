#ifndef IDCM_BATTERYINFO_H
#define IDCM_BATTERYINFO_H

#include <QDialog>

namespace Ui { class BatteryInfo; }

class BatteryInfo : public QDialog {
    Q_OBJECT

public:
    explicit BatteryInfo(QWidget *parent = nullptr);
    ~BatteryInfo() override;

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

public slots:
    void updateBatteryInfo();

private:
    static QString getCurPowerMode();
    static QString getBatteryType();
    void goSleep();
    void goHibernate();
    void reset() const;
    static const QString INITIAL_LINE;
    Ui::BatteryInfo *ui;
    QTimer *timer;
};

#endif //IDCM_BATTERYINFO_H