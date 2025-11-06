#ifndef DISKINFO_H
#define DISKINFO_H

#include <QDialog>

namespace Ui { class DiskInfo; }

class DiskInfo : public QDialog {
    Q_OBJECT

public:
    explicit DiskInfo(QWidget *parent = nullptr);
    ~DiskInfo() override;
    void setDevices();
    void addPage(QStringList fields) const;
    static QString bitmaskToList(quint32 mask, int maxMode);
    void showEvent(QShowEvent *event) override;

private:
    Ui::DiskInfo *ui;
    bool isDevicesSet = false;
};

#endif //DISKINFO_H