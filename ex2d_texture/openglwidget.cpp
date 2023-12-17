#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent)
{
}

OpenGLWidget::~OpenGLWidget()
{
    destroyVBOs();
    destroyShaders();
}


void OpenGLWidget :: initializeGL ()
{
    initializeOpenGLFunctions ();
    qDebug ("OpenGL version : %s", glGetString ( GL_VERSION ));
    qDebug ("GLSL %s", glGetString ( GL_SHADING_LANGUAGE_VERSION ));
    createShaders ();
    createVBOs ();
}

void OpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width , height);
}


void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);

    if (textureID)
    {        
        auto locColorTexture = glGetUniformLocation(shaderProgram,"colorTexture");
        glUniform1i(locColorTexture, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void OpenGLWidget::toggleBackgroundColor(bool changeColor)
{
    makeCurrent();
    if(changeColor)
        glClearColor(0,0,0,1);
    else
        glClearColor(1,1,1,1);
    update();

}

void OpenGLWidget :: createShaders ()
{
    // makeCurrent ();
    destroyShaders ();
    QString vertexShaderFile (":/shaders/vshader1.glsl");
    QString fragmentShaderFile (":/shaders/fshader1.glsl");
    QFile vs( vertexShaderFile );
    QFile fs( fragmentShaderFile );
    vs. open ( QFile :: ReadOnly | QFile :: Text );
    fs. open ( QFile :: ReadOnly | QFile :: Text );
    QTextStream streamVs (& vs), streamFs (& fs);
    QString qtStringVs = streamVs . readAll ();
    QString qtStringFs = streamFs . readAll ();
    std :: string stdStringVs = qtStringVs . toStdString ();
    std :: string stdStringFs = qtStringFs . toStdString ();

    // Create an empty vertex shader handle
    GLuint vertexShader = 0;
    vertexShader = glCreateShader( GL_VERTEX_SHADER );
    // Send the vertex shader source code to GL
    const GLchar * source = stdStringVs . c_str ();
    glShaderSource( vertexShader , 1, & source , 0);
    // Compile the vertex shader
    glCompileShader( vertexShader );
    GLint isCompiled = 0;
    glGetShaderiv( vertexShader , GL_COMPILE_STATUS , & isCompiled );
    if ( isCompiled == GL_FALSE )
    {
        GLint maxLength = 0;
        glGetShaderiv ( vertexShader , GL_INFO_LOG_LENGTH , & maxLength );
        // The maxLength includes the NULL character
        std :: vector <GLchar > infoLog ( maxLength );
        glGetShaderInfoLog( vertexShader , maxLength , & maxLength , & infoLog [0]) ;
        qDebug ("%s", & infoLog [0]) ;
        glDeleteShader( vertexShader );
    return ;
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = 0;
    fragmentShader = glCreateShader ( GL_FRAGMENT_SHADER );
    // Send the fragment shader source code to GL
    // Note that std :: string ’s . c_str is NULL character terminated .
    source = stdStringFs . c_str ();
    glShaderSource ( fragmentShader , 1, & source , 0);
    // Compile the fragment shader
    glCompileShader ( fragmentShader );
    glGetShaderiv ( fragmentShader , GL_COMPILE_STATUS , & isCompiled );
    if ( isCompiled == GL_FALSE )
    {
        GLint maxLength = 0;
        glGetShaderiv ( fragmentShader , GL_INFO_LOG_LENGTH , & maxLength );
        std :: vector <GLchar > infoLog ( maxLength );
        glGetShaderInfoLog ( fragmentShader , maxLength , & maxLength ,  & infoLog [0]) ;
        qDebug ("%s", & infoLog [0]) ;
        glDeleteShader ( fragmentShader );
        glDeleteShader ( vertexShader );
        return ;
    }

    shaderProgram = glCreateProgram();
    // Attach our shaders to our program
    glAttachShader( shaderProgram , vertexShader );
    glAttachShader ( shaderProgram , fragmentShader );
    // Link our program
    glLinkProgram( shaderProgram );
    // Note the different functions here : glGetProgram * instead of glGetShader *.
    GLint isLinked = 0;
    glGetProgramiv( shaderProgram , GL_LINK_STATUS , ( int *)& isLinked );
    if ( isLinked == GL_FALSE )
    {
        GLint maxLength = 0;
        glGetProgramiv ( shaderProgram , GL_INFO_LOG_LENGTH , & maxLength );
        // The maxLength includes the NULL character
        std :: vector <GLchar > infoLog ( maxLength );
        glGetProgramInfoLog ( shaderProgram , maxLength , & maxLength ,   & infoLog [0]) ;
        qDebug ("%s", & infoLog [0]) ;
        glDeleteProgram ( shaderProgram );
        glDeleteShader ( vertexShader );
        glDeleteShader ( fragmentShader );
        return ;
    }
    glDetachShader( shaderProgram , vertexShader );
    glDetachShader ( shaderProgram , fragmentShader );
    glDeleteShader( vertexShader );
    glDeleteShader ( fragmentShader );
    vs. close ();
    fs. close ();
}


void OpenGLWidget::destroyShaders() {
    makeCurrent();
    glDeleteProgram(shaderProgram);
}


void OpenGLWidget :: createVBOs ()
{
    makeCurrent ();
    destroyVBOs ();
    vertices.resize(4);
    coordText.resize(4);
    indices.resize(2 * 3);

    vertices [0] = QVector4D( -1, -1, 0, 1);
    vertices [1] = QVector4D(  1, -1, 0, 1);
    vertices [2] = QVector4D(  1,  1, 0, 1);
    vertices [3] = QVector4D( -1,  1, 0, 1);


    coordText[3] = QVector2D(0,0);
    coordText[2] = QVector2D(1,0);
    coordText[1] = QVector2D(1,1);
    coordText[0] = QVector2D(0,1);

    indices [0] = 0;
    indices [1] = 1;
    indices [2] = 2;
    indices [3] = 2;
    indices [4] = 3;
    indices [5] = 0;

    glGenVertexArrays(1, &vao );
    glBindVertexArray( vao);

    glGenBuffers(1, & vboVertices );
    glBindBuffer( GL_ARRAY_BUFFER , vboVertices );
    glBufferData( GL_ARRAY_BUFFER , 4 * sizeof (QVector4D), vertices.data(), GL_STATIC_DRAW );
    glVertexAttribPointer(0, 4, GL_FLOAT , GL_FALSE , 0, nullptr );
    glEnableVertexAttribArray(0) ;

    glGenBuffers (1, & vboCoordText );
    glBindBuffer ( GL_ARRAY_BUFFER , vboCoordText );
    glBufferData ( GL_ARRAY_BUFFER , 4 * sizeof (QVector2D), coordText.data() ,  GL_STATIC_DRAW );
    glVertexAttribPointer (1, 2, GL_FLOAT , GL_FALSE , 0, nullptr );
    glEnableVertexAttribArray (1) ;

    glGenBuffers (1, & vboIndices );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vboIndices );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER , 6*sizeof ( unsigned int ), indices.data() , GL_STATIC_DRAW );
}


void OpenGLWidget :: destroyVBOs ()
{

    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboCoordText);
    glDeleteBuffers(1, &vboIndices);
    glDeleteVertexArrays(1, &vao);

    vboVertices = 0;
    vboIndices = 0;
    vboCoordText = 0;
    vao = 0;
}
void OpenGLWidget::loadTexture()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        QString("Open Image File"), "",
        QString("JPEG (*.jpg *.jpeg);;PNG (*.png)"), 0);
    if (!fileName.isEmpty())
    {
        QImage image;
        image.load(fileName);
        image = image.convertToFormat(QImage::Format_RGBA8888);

        loadTexture(image);
    }
}

void OpenGLWidget::loadTexture(const QImage &image)
{
    makeCurrent();
    if (textureID)
        glDeleteTextures(1, &textureID);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(),
        image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    update();
}
