#include "SpriteWidget.h"

SpriteWidget::SpriteWidget(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_TranslucentBackground);

    for (int i = 1; i <= 6; i++) {
        QPixmap pix(QString(":/sprites/stay%1.png").arg(i));
        stayRight.append(pix);
        stayLeft.append(pix.transformed(QTransform().scale(-1, 1)));
    }

    for (int i = 1; i <= 7; i++) {
        QPixmap pix(QString(":/sprites/run%1.png").arg(i));
        runRight.append(pix);
        runLeft.append(pix.transformed(QTransform().scale(-1, 1)));
    }

    spriteWidth = stayLeft[0].width();

    connect(&timer, &QTimer::timeout, this, &SpriteWidget::updateAnimation);
    timer.start(67);
}

void SpriteWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHints(QPainter::SmoothPixmapTransform);

    const QVector<QPixmap> &frames = running
        ? (facingLeft ? runLeft : runRight)
        : (facingLeft ? stayLeft : stayRight);

    const QPixmap &frame = frames[currentFrame];
    p.drawPixmap(x, y, frame);
}

void SpriteWidget::mousePressEvent(QMouseEvent *event) {
    int newTargetX = event->pos().x();

    if (std::abs(newTargetX - x) < 30) return;
    if (newTargetX + spriteWidth > width()) newTargetX = width() - spriteWidth;

    targetX = newTargetX;
    running = true;
    facingLeft = targetX < x;
}

void SpriteWidget::updateAnimation() {
    if (!running) {
        currentFrame = (currentFrame + 1) % (facingLeft ? stayLeft.size() : stayRight.size());
        update();
        return;
    }

    int step = 15;
    if (x < targetX) x += step;
    else if (x > targetX) x -= step;

    if (std::abs(targetX - x) < step) {
        x = targetX;
        running = false;
        currentFrame = 0;
    } else {
        currentFrame = (currentFrame + 1) % (facingLeft ? runLeft.size() : runRight.size());
    }

    update();
}