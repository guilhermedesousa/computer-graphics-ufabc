#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget{parent}
{

}


OpenGLWidget::~OpenGLWidget()
{
    destroyVBOs();
    destroyShaders();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1,1,1,1);

    connect(&timer, &QTimer::timeout,this,[&](){update();});
    timer.start(10);

    qDebug("OpenGL Version: %s", glGetString(GL_VERSION));
    qDebug("GSL Version %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    createShaders();
    createVBOs();
}

void OpenGLWidget::resizeGL(int w, int h)
{

}

void OpenGLWidget::drawAxis()
{
    makeCurrent();

    glUseProgram(shaderProgram);
    mModel.setToIdentity();
    auto locModelMatrix{glGetUniformLocation(shaderProgram,"mModel")};
    glUniformMatrix4fv(locModelMatrix,1,GL_FALSE,mModel.data());

    glBindVertexArray(vaoAxis);
    glVertexAttrib4f(1,1.0f,0.0f,0.0f,0.0f); // red axis
    glDrawArrays(GL_LINES,0,2);
    glVertexAttrib4f(1,0.0f,1.0f,0.0f,0.0f); // green axis
    glDrawArrays(GL_LINES,2,2);
}

void OpenGLWidget::scaleExample()
{
    makeCurrent();
    mModel.setToIdentity();

    double scale[] = {1,1};
    static double percentage=1;
    mModel.scale(scale[0]*percentage,scale[1]*percentage,1);
    percentage-=0.005;
}

void OpenGLWidget::rotationOrbitExample()
{
    makeCurrent();
    mModel.setToIdentity();
    static int angle=0;

    mModel.rotate(angle++,QVector3D(0,0,1));

    if(angle>359) angle=0;
}

void OpenGLWidget::rotationCentroidExample()
{
    makeCurrent();
    mModel.setToIdentity();
    QVector4D centroid;
    for (QVector4D x : vertices)
        centroid+=x;
    centroid=(1.0f/static_cast<float>(vertices.size()))*centroid;

    static int angle=0;

    mModel.translate(centroid.toVector3D());
    mModel.rotate(angle++,QVector3D(0,0,1));
    mModel.translate(-centroid.toVector3D());
    if(angle>359) angle=0;
}

void OpenGLWidget::nonComutativeExample()
{
    makeCurrent();
    mModel.setToIdentity();

    QVector4D centroid;
    for (QVector4D x : vertices)
        centroid+=x;

    centroid=(1.0f/static_cast<float>(vertices.size()))*centroid;
    static int angle=0;
    mModel.translate(-centroid.toVector3D());
    mModel.rotate(angle++,QVector3D(0,0,1));
    mModel.translate(centroid.toVector3D());
    if(angle>359) angle=0;
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxis();

    glBindVertexArray(vao);
    auto locModelMatrix{glGetUniformLocation(shaderProgram,"mModel")};
    glUniformMatrix4fv(locModelMatrix,1,GL_FALSE,mModel.data());
    glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);

    //rotationOrbitExample();
    //scaleExample();
    //rotationCentroidExample();
    nonComutativeExample();

    glUniformMatrix4fv(locModelMatrix,1,GL_FALSE,mModel.data());
    glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
}

void OpenGLWidget::toggleDarkMode(bool changeToDarkMode)
{
    makeCurrent();
    if (changeToDarkMode) {
        glClearColor(0,0,0,1);
    } else {
        glClearColor(1,1,1,1);
    }
    update();
}

void OpenGLWidget::createShaders()
{
    makeCurrent();
    destroyShaders();
    QFile vs(":/shaders/vshader1.glsl");
    QFile fs(":/shaders/fshader1.glsl");
    if (!vs.open((QFile::ReadOnly | QFile::Text))) return; //mensagem de erro - inserir
    if (!fs.open((QFile::ReadOnly | QFile::Text))) return; //mensagem de erro - inserir

    auto byteArrayVs{vs.readAll()};
    auto byteArrayFs{fs.readAll()};

    const char *c_strVs{byteArrayVs};
    const char *c_strFs{byteArrayFs};

    vs.close();
    fs.close();

    GLuint vertexShader{glCreateShader(GL_VERTEX_SHADER)};
    glShaderSource(vertexShader,1,&c_strVs,0);
    glCompileShader(vertexShader);

    GLint isCompiled{0};
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength{0};
        glGetShaderiv(vertexShader,GL_INFO_LOG_LENGTH,&maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader,maxLength,&maxLength,&infoLog[0]);
        qDebug("%s",&infoLog[0]);
        glDeleteShader(vertexShader);
        return;
    }

    GLuint fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};
    glShaderSource(fragmentShader,1,&c_strFs,0);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength{0};
        glGetShaderiv(fragmentShader,GL_INFO_LOG_LENGTH,&maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader,maxLength,&maxLength,&infoLog[0]);
        qDebug("%s",&infoLog[0]);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint isLinked{0};
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&isLinked);

    if (isLinked == GL_FALSE)
    {
        GLint maxLength{0};
        glGetProgramiv(shaderProgram,GL_INFO_LOG_LENGTH,&maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(shaderProgram,maxLength,&maxLength,&infoLog[0]);
        qDebug("%s",&infoLog[0]);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(shaderProgram);
        return;
    }

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void OpenGLWidget::destroyShaders()
{
    makeCurrent();
    glDeleteProgram(shaderProgram);
}

void OpenGLWidget::createVBOs()
{
    makeCurrent();
    destroyVBOs();

    vertices.resize(4);
    colors.resize(4);
    indices.resize(6);

    float posx{0.3f};
    float posy{0.3f};
    float offset{0.2f};
    vertices[0] = QVector4D(posx - offset, posy - offset,0,1);
    vertices[1] = QVector4D(posx + offset, posy - offset,0,1);
    vertices[2] = QVector4D(posx + offset, posy + offset,0,1);
    vertices[3] = QVector4D(posx - offset, posy + offset,0,1);

    colors[0] = QVector4D(1,0,0,1);
    colors[1] = QVector4D(0,1,0,1);
    colors[2] = QVector4D(0,0,1,1);
    colors[3] = QVector4D(1,1,0,1);

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 3;
    indices[5] = 0;

    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    glGenBuffers(1,&vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(QVector4D),vertices.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1,&vboColors);
    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(QVector4D),colors.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,0,nullptr);
    glEnableVertexAttribArray(1);

    glGenBuffers(1,&eboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,eboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof (GLuint), indices.data(),GL_STATIC_DRAW);

    glGenVertexArrays(1,&vaoAxis);
    glBindVertexArray(vaoAxis);

    //forma "raiz" de mandar os dados

    float v[] = {-1.0f, 0.0f, 0, 1,
                  1.0f, 0.0f, 0, 1,
                  0.0f, -1.0f, 0, 1,
                 0.0f, 1.0f, 0, 1};

    glGenBuffers(1,&vboVerticesAxis);
    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesAxis);
    glBufferData(GL_ARRAY_BUFFER, 4*4*sizeof(float),v,GL_STATIC_DRAW);
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,nullptr);
    glEnableVertexAttribArray(0);
}

void OpenGLWidget::destroyVBOs()
{
    makeCurrent();

    glDeleteBuffers(1,&vboVertices);
    glDeleteBuffers(1,&vboColors);
    glDeleteBuffers(1,&eboIndices);
    glDeleteVertexArrays(1,&vao);
    glDeleteBuffers(1,&vboVerticesAxis);
    glDeleteBuffers(1,&vaoAxis);

    vboVertices = 0;
    vboColors = 0;
    eboIndices = 0;
    vao = 0;
    vboVerticesAxis = 0;
    vaoAxis = 0;
}
