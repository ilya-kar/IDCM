#ifndef PCIINFO_H
#define PCIINFO_H

#include <QDialog>

namespace Ui {class PciInfo; }

class PciInfo : public QDialog {
    Q_OBJECT

public:
    explicit PciInfo(QWidget *parent = nullptr);
    ~PciInfo() override;
    void setDevices();
    void showEvent(QShowEvent *event) override;

private:
    Ui::PciInfo *ui;
    bool isDevicesSet = false;
};

#endif //PCIINFO_H