#include "trackball.h"

TrackBall::TrackBall()
{
    lastTime.start();
}

void TrackBall::mouseMove(const QPointF &p)
{
    if (!trackingMouse) return;

    auto msecs = lastTime.restart();
    if (msecs)
    {
        auto currentPos3D{QVector3D(screenToNormalizedSquare(p),0.0)};
        auto lenSqr = currentPos3D.lengthSquared();
        //se (x,y) cai fora da esfera, mantem z=0, cc z = sqrt(1 - x^2 - y^2)
        (lenSqr >= 1.0f) ? currentPos3D.normalize() : currentPos3D.setZ(sqrt(1.0f - lenSqr));
        //theta aprox |sin(theta)| = ||u x v|| se ||u|| = ||v|| = 1;
        axis = QVector3D::crossProduct(lastPos3D, currentPos3D);
        auto angle{rad2deg * axis.length()};
        velocity = angle/msecs;
        axis.normalize();
        currrentQuaternion = QQuaternion::fromAxisAndAngle(axis,angle)*currrentQuaternion;

        lastPos3D = currentPos3D;
    }
}

void TrackBall::mousePress(const QPointF &p)
{
    currrentQuaternion = getRotation();
    trackingMouse = true;

    lastTime.restart();

    lastPos3D = QVector3D(screenToNormalizedSquare(p),0.0f);

    auto lenSqr = lastPos3D.lengthSquared();
    (lenSqr >= 1.0f) ? lastPos3D.normalize() : lastPos3D.setZ(sqrt(1.0f - lenSqr));

    velocity = 0.0;
}

void TrackBall::mouseRelease(const QPointF &p)
{
    mouseMove(p);
    trackingMouse = false;
}

void TrackBall::resizeViewport(int width, int height)
{
    viewportWidth  = static_cast<float>(width);
    viewportHeight = static_cast<float>(height);
}

QQuaternion TrackBall::getRotation()
{
    if (trackingMouse)  return currrentQuaternion;

    auto angle{velocity * lastTime.elapsed()};
    return QQuaternion::fromAxisAndAngle(axis, angle) * currrentQuaternion;
}

QVector2D TrackBall::screenToNormalizedSquare(const QPointF &p)
{
    return QVector2D(2.0 * p.x() / viewportWidth - 1.0,
                     1.0 - 2.0 * p.y() / viewportHeight);
}
