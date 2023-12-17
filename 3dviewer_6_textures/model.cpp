#include "model.h"

Model::Model()
{
    shaderProgram.resize(fragmentShaderFileName.size());
    modelMatrix.setToIdentity();
}

void Model::readOFFFile(const QString &fileName)
{
    QFile data(fileName);
    data.open(QFile::ReadOnly);

    if(!data.isOpen())
    {
        qWarning("Cannot open file");
        return;
    }

    QTextStream stream(&data);
    QString notUsed;

    stream >> notUsed;//OFF
    stream >> numVertices >> numFaces >> notUsed;

    vertices.resize(numVertices);
    indices.resize(3*numFaces);

    float x,y,z;
    //vertices
    for(size_t i{0}; i < numVertices; ++i)
    {
        stream >> x >> y >> z;
        vertices[i] = QVector4D(x,y,z,1);
    }
    //faces
    unsigned a,b,c;
    for(size_t i{0}; i < numFaces; ++i)
    {
        stream >> notUsed >> a >> b >> c;
        indices[i*3 + 0] = a;
        indices[i*3 + 1] = b;
        indices[i*3 + 2] = c;
    }
    computeBBox();

    createNormals();
    createTextCoords();
    createTangents();

    data.close();
}

void Model::computeBBox()
{
    auto minLim = std::numeric_limits<float>::lowest();
    auto maxLim = std::numeric_limits<float>::max();

    maxBB = QVector3D(minLim,minLim,minLim);
    minBB = QVector3D(maxLim,maxLim,maxLim);

    for(size_t i{0}; i < numVertices; i++)
    {
        maxBB.setX(std::max(maxBB.x(), vertices[i].x()));
        maxBB.setY(std::max(maxBB.y(), vertices[i].y()));
        maxBB.setZ(std::max(maxBB.z(), vertices[i].z()));
        minBB.setX(std::min(minBB.x(), vertices[i].x()));
        minBB.setY(std::min(minBB.y(), vertices[i].y()));
        minBB.setZ(std::min(minBB.z(), vertices[i].z()));
    }

    centroidBB = 0.5*(maxBB+minBB);
    diagonalBB = (maxBB - minBB).length();
}

void Model::rescaleModelMatrix()
{
    float invdiag{2.5f/diagonalBB};
    modelMatrix.scale(invdiag,invdiag, invdiag);
    modelMatrix.translate(-centroidBB);
}

void Model::createNormals()
{
    normals.resize(numVertices);

    for (size_t i{0}; i < numFaces; ++i)
    {
        auto a{QVector3D(vertices[indices[i * 3 + 0]])};
        auto b{QVector3D(vertices[indices[i * 3 + 1]])};
        auto c{QVector3D(vertices[indices[i * 3 + 2]])};

        auto faceNormal{QVector3D::crossProduct((b - a), (c - b)).normalized()};

        normals[indices[i * 3 + 0]] += faceNormal;
        normals[indices[i * 3 + 1]] += faceNormal;
        normals[indices[i * 3 + 2]] += faceNormal;
    }

    for (size_t i{0}; i < numVertices; ++i)
    {
        normals[i].normalize();
    }
}


void Model::createTextCoords()
{
    textureCoords.resize(numVertices);

    // Compute minimum and maximum values
    auto minz{std::numeric_limits<float>::max()};
    auto maxz{std::numeric_limits<float>::lowest()};

    for (size_t i{0}; i < numVertices; ++i)
    {
        minz = std::min(vertices[i].z(), minz);
        maxz = std::max(vertices[i].z(), maxz);
    }

    for (size_t i{0}; i < numVertices; ++i)
    {
        auto s = (std::atan2(vertices[i].y(), vertices[i].x()) + M_PI) / (2 * M_PI);
        auto t = 1.0f - (vertices[i].z() - minz) / (maxz - minz);
        textureCoords[i] = QVector2D(s, t);
    }
}

void Model::createTangents()
{
    tangents.resize(numVertices);

    std::vector<QVector3D> bitangents(numVertices);

    for (size_t i{0}; i < numFaces ; ++i)
    {
        unsigned  i1{indices[i * 3 + 0]};
        unsigned  i2{indices[i * 3 + 1]};
        unsigned  i3{indices[i * 3 + 2]};

        QVector3D E{vertices[i1].toVector3D()};
        QVector3D F{vertices[i2].toVector3D()};
        QVector3D G{vertices[i3].toVector3D()};

        QVector2D stE{textureCoords[i1]};
        QVector2D stF{textureCoords[i2]};
        QVector2D stG{textureCoords[i3]};

        QVector3D P{F - E};
        QVector3D Q{G - E};

        QVector2D st1{stF - stE};
        QVector2D st2{stG - stE};

        QMatrix2x2 M;
        M(0, 0) =  st2.y();
        M(0, 1) = -st1.y();
        M(1, 0) = -st2.x();
        M(1, 1) =  st1.x();
        M *= (1.0 / (st1.x() * st2.y() - st2.x() * st1.y()));

        QVector4D T{QVector4D (M(0, 0) * P.x() + M(0, 1) * Q.x(),
                               M(0, 0) * P.y() + M(0, 1) * Q.y(),
                               M(0, 0) * P.z() + M(0, 1) * Q.z(), 0.0)};

        QVector3D B{QVector3D (M(1, 0) * P.x() + M(1, 1) * Q.x(),
                               M(1, 0) * P.y() + M(1, 1) * Q.y(),
                               M(1, 0) * P.z() + M(1, 1) * Q.z())};

        tangents[i1] += T;
        tangents[i2] += T;
        tangents[i3] += T;

        bitangents[i1] += B;
        bitangents[i2] += B;
        bitangents[i3] += B;
    }

    for (size_t i{0}; i < numVertices; ++i)
    {
        const QVector3D &n{normals[i]};
        const QVector4D &t{tangents[i]};
        tangents[i] = QVector4D((t.toVector3D() - n * QVector3D::dotProduct(n, t.toVector3D())).normalized(),t.w());
        QVector3D b{QVector3D::crossProduct(n, t.toVector3D())};
        auto hand{QVector3D::dotProduct(b, bitangents[i])};
        tangents[i].setW((hand < 0.0) ? -1.0 : 1.0);;
    }
}
