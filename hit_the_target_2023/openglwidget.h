#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QtOpenGLWidgets/QOpenGLWidget> // herda a janela e substituiu o QWidget
#include <QOpenGLExtraFunctions> //inserido - herda as funções do OpenGL
#include <QTimer>
#include <QElapsedTimer>
#include <QVector4D>
#include <QFile>
#include <QKeyEvent>
#include <QApplication>
#include <array>
#include <cmath>

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
    void keyReleaseEvent(QKeyEvent *event) override;

    float playerPosYOffset{0}; // player displacement along Y axis
    float playerPosY{0}; // current player position

    float targetPosYOffset{2.0f}; // target displacement along Y axis
    float targetPosY{0}; // current target position

    QTimer timer;
    QElapsedTimer elapsedTime;

    bool shooting{false};
    std::array<float,2> projectilePos;
    int numHits{0};

signals:
    void updateHitsLabel(QString);

public slots:
    void animate();

private:
//    QTimer timer;
//    bool blinkFlag{false};

};

#endif // OPENGLWIDGET_H
