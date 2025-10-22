#ifndef SPRITEWIDGET_H
#define SPRITEWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QVector>

class SpriteWidget : public QWidget {
    Q_OBJECT

public:
    explicit SpriteWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void updateAnimation();

private:
    QVector<QPixmap> stayRight;
    QVector<QPixmap> stayLeft;
    QVector<QPixmap> runRight;
    QVector<QPixmap> runLeft;

    QTimer timer;
    int currentFrame = 0;

    int x = 0;
    int y = 0;
    int targetX = 0;
    int spriteWidth;

    bool running = false;
    bool facingLeft = false;
};


#endif //SPRITEWIDGET_H