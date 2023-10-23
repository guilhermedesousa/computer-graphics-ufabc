#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QtOpenGLWidgets/QOpenGLWidget> // herda a janela e substituiu o QWidget
#include <QOpenGLExtraFunctions> // inserido - herda as funções do OpenGL
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
    explicit OpenGLWidget(QWidget *parent = nullptr); // construtor
    ~OpenGLWidget(); // destrutor

    void initializeGL() override; // iniciar e configurar
    void resizeGL(int w, int h) override; // chama quando ha redimensionamento
    void paintGL() override; // renderizar as coisas

    // ids para buffers e arrays que guardam dados de render
    GLuint vboVertices{ 0 };
    GLuint vboColors{ 0 };
    GLuint eboIndices{ 0 };
    GLuint vao{ 0 };

    // vetores que contem os dados dos vertices, cores e geometria
    std::vector<QVector4D> vertices;
    std::vector<QVector4D> colors;
    std::vector<GLuint> indices;

    // controla como os objetos sao renderizados
    GLuint shaderProgram{ 0 };

    // criam buffers e programas de shader
    void createVBOs();
    void createShaders();

    // liberam memoria alocada para os recursos
    void destroyVBOs();
    void destroyShaders();

    // interacao no jogo
    void changeDiagonal();
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    // posicoes dos objetos
    float playerPosYOffset{ 0 }; // player displacement along Y axis
    float playerPosY{ 0 }; // current player position

    float targetPosYOffset{ 2.0f }; // target displacement along Y axis
    float targetPosY{ 0 }; // current target position

    // medir o tempo e atualizar o jogo de forma continua
    QTimer timer;
    QElapsedTimer elapsedTime;

    // rastrear acoes do jogo
    bool shooting{false};
    std::array<float, 2> projectilePos;
    int numHits{ 0 };

signals:
    // atualizar o texto dos hits
    void updateHitsLabel(QString);

public slots:
    // atualizar a animcacao do jogo
    void animate();
};

#endif // OPENGLWIDGET_H
