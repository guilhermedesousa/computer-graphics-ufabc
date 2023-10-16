#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QtOpenGLWidgets/QOpenGLWidget> // herda a janela e substituiu o QWidget
#include <QOpenGLExtraFunctions> //inserido - herda as funções do OpenGL
#include <QTimer>
#include <QVector4D>
#include <QFile>
#include <QKeyEvent>
#include <QApplication>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions // substitui a heranca do QWidget
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget();

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    GLuint vboVertices{0};
    GLuint vboColors{0};
    GLuint eboIndices{0};
    GLuint vao{0};

    std::vector<QVector4D> vertices;
    std::vector<QVector4D> colors;
    std::vector<GLuint> indices;

    GLuint shaderProgram{0};

    void createVBOs();
    void createShaders();

    void destroyVBOs();
    void destroyShaders();

    void changeDiagonal();

    void keyPressEvent(QKeyEvent *event) override;

signals:

public slots:
    void toggleDarkMode(bool);

private:
//    QTimer timer;
//    bool blinkFlag{false};

};

#endif // OPENGLWIDGET_H
