#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>

class Camera
{
public:
    Camera();
    // vamos "ajustar" o eye depois quando trabalharmos com
    // projection -> se modelo cair fora do (-1,-,1-,1) (1,1,1) cortará

    QVector3D eye{QVector3D(0,0,2)}; // posicao na cena
    QVector3D center{QVector3D(0,0,0)}; // para onde olhar (origem)
    QVector3D up{QVector3D(0,1,0)}; // direcao da camera

    QMatrix4x4 viewMatrix; // escapsula as propriedades da camera
    QMatrix4x4 projectionMatrix;

    void createViewMatrix();
    void resizeViewPort(int width, int height, bool ortho=false);

signals:

};

#endif // CAMERA_H
