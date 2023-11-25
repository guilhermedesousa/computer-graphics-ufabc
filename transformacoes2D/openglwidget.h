#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QTimer>
#include <QVector4D>
#include <QFile>
#include <QKeyEvent>
#include <QApplication>
#include <QMatrix4x4>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget() override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void drawAxis();

    GLuint vboVertices{0};
    GLuint vboColors{0};
    GLuint eboIndices{0};
    GLuint vao{0};

    GLuint vaoAxis{0};
    GLuint vboVerticesAxis{0};

    QTimer timer;
    void rotationCentroidExample();
    void rotationOrbitExample();
    void scaleExample();
    void nonComutativeExample();

    std::vector<QVector4D> vertices;
    std::vector<QVector4D> colors;
    std::vector<GLuint> indices;

    QMatrix4x4 mModel;

    GLuint shaderProgram{0};

    void createVBOs();
    void createShaders();
    void destroyVBOs();
    void destroyShaders();

signals:

public slots:
    void toggleDarkMode(bool);

};

#endif // OPENGLWIDGET_H
