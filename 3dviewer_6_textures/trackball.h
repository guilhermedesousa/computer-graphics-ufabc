#ifndef TRACKBALL_H
#define TRACKBALL_H

#include <QVector3D>
#include <QVector2D>
#include <QQuaternion>
#include <QElapsedTimer>
#include <cmath>

class TrackBall
{
public:
    TrackBall();

    void mouseMove(const QPointF&);
    void mousePress(const QPointF&);
    void mouseRelease(const QPointF&);
    void resizeViewport(int,int);
    QQuaternion getRotation();

private:
    QQuaternion currrentQuaternion;
    QVector3D axis;
    float velocity{0.0f};
    QVector3D lastPos3D;
    QElapsedTimer lastTime;
    bool trackingMouse{false};
    float viewportWidth;
    float viewportHeight;

    const float rad2deg{180.0f/3.1415f};

    QVector2D screenToNormalizedSquare(const QPointF&);
};

#endif
