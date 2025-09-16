#ifndef IDCM_BATTERYINFO_H
#define IDCM_BATTERYINFO_H

#include <QDialog>
#include <windows.h>

namespace Ui { class BatteryInfo; }

class BatteryInfo : public QDialog {
    Q_OBJECT

public:
    explicit BatteryInfo(QWidget *parent = nullptr);
    ~BatteryInfo() override;

public slots:
    void updateBatteryInfo();

private:
    void updateACLineStatus(const BYTE status) const;
    void updatePowerState(const BYTE state) const;
    void updateLifePercent(const BYTE percent) const;
    void updateLifeTime(const DWORD time) const;
    Ui::BatteryInfo *ui;
    SYSTEM_POWER_STATUS sps;
};

#endif //IDCM_BATTERYINFO_H