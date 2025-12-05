#include <QMediaDevices>
#include <QMediaDevices>
#include <QDir>
#include "qhotkey.h"
#include "CameraInfo.h"
#include "ui_CameraInfo.h"

CameraInfo::CameraInfo(QWidget *parent) : QDialog(parent), ui(new Ui::CameraInfo) {
    ui->setupUi(this);

    camera = new QCamera(this);
    captureSession = new QMediaCaptureSession(this);
    imageCapture = new QImageCapture(this);
    mediaRecorder = new QMediaRecorder(this);
    restoreHotkey = new QHotkey(QKeySequence("Ctrl+Alt+O"), true, this);
    takePhoto = new QHotkey(QKeySequence("Ctrl+Alt+P"), true, this);
    makeVideo = new QHotkey(QKeySequence("Ctrl+Alt+V"), true, this);

    ui->stackedWidgetCamera->setCurrentIndex(1);
    initConnections();
    getCameraInfo();
    initCamera();
    makeDirs();
}

CameraInfo::~CameraInfo() {
    delete ui;
}

void CameraInfo::initConnections() {
    connect(camera, &QCamera::errorOccurred, this, &CameraInfo::onCameraError);
    connect(ui->pushButtonPhoto, &QPushButton::clicked, this, &CameraInfo::onButtonPhoto);
    connect(ui->pushButtonHidden, &QPushButton::clicked, this, [this] {
        isHiddenMode = true;
        hide();
    });
    connect(restoreHotkey, &QHotkey::activated, this, &CameraInfo::onGlobalHotkeyPressed);
    connect(takePhoto, &QHotkey::activated, this, &CameraInfo::onHiddenPhoto);
    connect(makeVideo, &QHotkey::activated, this, &CameraInfo::onHiddenVideo);
    connect(ui->pushButtonVideo, &QPushButton::clicked, this, &CameraInfo::onButtonVideo);
}

void CameraInfo::getCameraInfo() const {
    const auto cameras = QMediaDevices::videoInputs();
    if (cameras.empty()) {
        ui->stackedWidgetCamera->setCurrentIndex(0);
        ui->labelSetStatus->setText("Камера отсутствует");
        ui->labelSetName->setText("N/A");
        ui->labelSetVid->setText("N/A");
        ui->labelSetPid->setText("N/A");
        ui->labelSetMi->setText("N/A");
        ui->labelSetGuid->setText("N/A");
        return;
    }

    const QCameraDevice &cameraDevice = cameras.first();

    ui->labelSetName->setText(cameraDevice.description());

    CameraIdInfo cameraIdInfo = parseCameraId(cameraDevice.id());
    ui->labelSetVid->setText(cameraIdInfo.vid);
    ui->labelSetPid->setText(cameraIdInfo.pid);
    ui->labelSetMi->setText(cameraIdInfo.mi);
    ui->labelSetGuid->setText(cameraIdInfo.guid);

    for (const auto &format : cameraDevice.videoFormats()) {
        QSize resolution = format.resolution();
        int minFps = format.minFrameRate();
        int maxFps = format.maxFrameRate();

        QVideoFrameFormat::PixelFormat pixelFormat = format.pixelFormat();
        QString pixelFmtStr = QVideoFrameFormat::pixelFormatToString(pixelFormat);

        QString line = QString("%1x%2 @ %3-%4 fps, %5")
                            .arg(resolution.width())
                            .arg(resolution.height())
                            .arg(minFps)
                            .arg(maxFps)
                            .arg(pixelFmtStr);

        ui->listWidgetFormats->addItem(line);
    }
}

CameraIdInfo CameraInfo::parseCameraId(const QString &deviceId) const {
    CameraIdInfo info;

    {
        QRegularExpression re("vid_([0-9a-fA-F]{4})");
        auto m = re.match(deviceId);
        if (m.hasMatch()) info.vid = m.captured(1);
    }

    {
        QRegularExpression re("pid_([0-9a-fA-F]{4})");
        auto m = re.match(deviceId);
        if (m.hasMatch()) info.pid = m.captured(1);
    }

    {
        QRegularExpression re("mi_([0-9a-fA-F]{2})");
        auto m = re.match(deviceId);
        if (m.hasMatch()) info.mi = m.captured(1);
    }

    {
        QRegularExpression re("\\{([0-9a-fA-F\\-]{36})\\}");
        auto m = re.match(deviceId);
        if (m.hasMatch()) info.guid = m.captured(1);
    }

    return info;
}

void CameraInfo::initCamera() const {
    const auto cameras = QMediaDevices::videoInputs();
    if (cameras.empty()) return;

    camera->setCameraDevice(cameras.first());
    captureSession->setCamera(camera);
    captureSession->setVideoOutput(ui->videoWidget);
    captureSession->setImageCapture(imageCapture);
    captureSession->setRecorder(mediaRecorder);

    camera->start();
}

void CameraInfo::onCameraError(QCamera::Error error) const {
    switch (error) {
        case QCamera::CameraError:
            ui->labelSetStatus->setText("Камера занята другим приложением или доступ запрещен");
            ui->stackedWidgetCamera->setCurrentIndex(0);
            break;
        default: break;
    }
}

void CameraInfo::makeDirs() const {
    QString appDir = QCoreApplication::applicationDirPath();
    QDir().mkpath(appDir + "/camera/videos");
    QDir().mkpath(appDir + "/camera/photos");
}

void CameraInfo::onButtonPhoto() const {
    QString dir = QCoreApplication::applicationDirPath() + "/camera/photos/";
    QString filename = dir + "image_"
            + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".jpg";
    imageCapture->captureToFile(filename);
}

void CameraInfo::onButtonVideo() {
    if (!isRecording) startRecording();
    else stopRecording();
}

void CameraInfo::startRecording() {
    QString dir = QCoreApplication::applicationDirPath() + "/camera/videos/";
    QString filename = dir + "video_"
            + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".mp4";

    mediaRecorder->setOutputLocation(QUrl::fromLocalFile(filename));

    ui->pushButtonPhoto->setEnabled(false);
    ui->pushButtonHidden->setEnabled(false);
    ui->pushButtonVideo->setText("Остановить запись");
    isRecording = true;

    mediaRecorder->record();
}

void CameraInfo::stopRecording() {
    mediaRecorder->stop();
    ui->pushButtonPhoto->setEnabled(true);
    ui->pushButtonHidden->setEnabled(true);
    ui->pushButtonVideo->setText("Снять видео");
    isRecording = false;
}

void CameraInfo::onHiddenPhoto() {
    if (!isHiddenMode) return;
    onButtonPhoto();
}

void CameraInfo::onGlobalHotkeyPressed() {
    if (!isHiddenMode) return;
    isHiddenMode = false;
    if (isRecording) stopRecording();
    showNormal();
    raise();
    activateWindow();
}

void CameraInfo::onHiddenVideo() {
    if (!isHiddenMode) return;
    onButtonVideo();
}