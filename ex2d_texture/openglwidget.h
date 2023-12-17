#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <memory>
#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_0_Core>
#include <QFileDialog>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_0_Core
{
    Q_OBJECT

private:
    QTimer timer;
    bool blinkFlag{false};

public:

    GLuint vboVertices{0};
    GLuint vboIndices{0};

    GLuint vboCoordText{0};
    GLuint textureID{0};

    GLuint vao{0};

    std::vector<QVector4D> vertices;
    std::vector<unsigned int> indices;
    std::vector<QVector2D> coordText;


    GLuint shaderProgram;

    public:

    explicit OpenGLWidget(QWidget *parent = 0);
    ~OpenGLWidget();

    void createVBOs();
    void createShaders();

    void destroyVBOs();
    void destroyShaders();

    void loadTexture(const QImage &image);


signals:

public slots:
    void toggleBackgroundColor(bool);
    void loadTexture();


protected:
    void initializeGL();
    void resizeGL(int,int);
    void paintGL();

};

#endif // OPENGLWIDGET_H
