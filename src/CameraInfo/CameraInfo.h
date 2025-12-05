#ifndef CAMERAINFO_H
#define CAMERAINFO_H

#include <QDialog>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QImageCapture>
#include <QMediaRecorder>
#include <QTimer>

namespace Ui { class CameraInfo; }

class QHotkey;

struct CameraIdInfo {
    QString vid;
    QString pid;
    QString mi;
    QString guid;
};

class CameraInfo : public QDialog {
    Q_OBJECT

public:
    explicit CameraInfo(QWidget *parent = nullptr);
    ~CameraInfo() override;
    void startRecording();
    void stopRecording();

private slots:
    void onCameraError(QCamera::Error error) const;
    void onButtonPhoto() const;
    void onButtonVideo();
    void onHiddenPhoto();
    void onGlobalHotkeyPressed();
    void onHiddenVideo();

private:
    void initConnections();
    void getCameraInfo() const;
    CameraIdInfo parseCameraId(const QString &deviceId) const;
    void initCamera() const;
    void makeDirs() const;

    Ui::CameraInfo *ui;
    QCamera *camera;
    QImageCapture *imageCapture;
    QMediaRecorder *mediaRecorder;
    QMediaCaptureSession *captureSession;

    QHotkey *restoreHotkey;
    QHotkey *takePhoto;
    QHotkey *makeVideo;

    bool isRecording = false;
    bool isHiddenMode = false;
};

#endif //CAMERAINFO_H