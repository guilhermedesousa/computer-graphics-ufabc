#include "camera.h"

Camera::Camera()
{
    createViewMatrix();
}

void Camera::createViewMatrix()
{
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(eye,center,up);
}

void Camera::resizeViewPort(int width, int height,bool ortho)
{
    projectionMatrix.setToIdentity();
    auto aspectRatio{static_cast<float>(width)/static_cast<float>(height)};

    if(!ortho)
        projectionMatrix.perspective(60.0f,aspectRatio,1.0f,10.0f);
    else
        projectionMatrix.ortho(-aspectRatio,aspectRatio,-1,1,-4,4);
}

