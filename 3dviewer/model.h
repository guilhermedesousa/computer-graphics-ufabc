#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <QVector4D>
#include <QVector3D>
#include <QString>
#include <QFile>
#include <QTextStream>

class Model
{
public:
    Model();
    ~Model() = default;

    std::vector<QVector4D> vertices;
    std::vector<unsigned> indices;

    unsigned numVertices{0};
    unsigned numFaces{0};

    unsigned vao{0};
    unsigned vboVertices{0};
    unsigned eboIndices{0};

    void readOFFFile(const QString &);

    void computeBBox();
    void rescaleModel();
    QVector3D centroidBB, minBB, maxBB;
    float diagonalBB{0.0f};

    int currentShader{0};

    std::vector<QString> vertexShaderFileName = {":/shaders/vzdepth.glsl"};
    std::vector<QString> fragmentShaderFileName = {":/shaders/fzdepth.glsl"};

    std::vector<unsigned> shaderProgram;
};

#endif // MODEL_H
