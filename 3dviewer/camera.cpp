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
