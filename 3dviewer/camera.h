#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>

class Camera
{
public:
    Camera();
    // vamos "ajustar" o eye depois quando trabaalharmos com
    // projection -> se modelo cair fora do (-1,-,1-,1) (1,1,1) cortará

    QVector3D eye{QVector3D(0,0,-0.01)};
    QVector3D center{QVector3D(0,0,0)};
    QVector3D up{QVector3D(0,1,0)};

    QMatrix4x4 viewMatrix;

    void createViewMatrix();

signals:

};

#endif // CAMERA_H
