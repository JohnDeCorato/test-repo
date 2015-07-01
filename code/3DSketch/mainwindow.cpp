#define _USE_MATH_DEFINES
#include "mainwindow.h"

#include <QMouseEvent>
#include <iostream>

#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
    QOpenGLWidget(parent)
{
    //setSurfaceType(OpenGLSurface);


    mLayer1 = new Layer();
    mLayer2 = new Layer();

    mLayer = mLayer1;
    stroking = false;

    mCamera = new Camera(QVector3D(0,0,0), QVector3D(0,0,-1),0,0);
}

MainWindow::~MainWindow()
{
    makeCurrent();

    doneCurrent();
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
    QVector3D d = rayCaster.generateRayDirection(mousePressPosition, (float)this->width(),
                                                 (float)this->height(), projection, mCamera);
    QVector3D* intersect = rayCaster.findIntersection(mCamera->position(), d, triangles);
    if (intersect != NULL)
    {
        qDebug() << "Beginning Stroke";
        stroking = true;
        currentStroke = mLayer->createNewStroke();
        currentStroke->addPoint(*intersect);
        lastMousePosition = mousePressPosition;
    }
    else
    {
        qDebug() << "No intersection found. No stroke created.";
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (stroking)
    {
        mousePressPosition = QVector2D(e->localPos());
        if (notAdjecnt(mousePressPosition, lastMousePosition, 5))
        {
            QVector3D d = rayCaster.generateRayDirection(mousePressPosition, (float)this->width(),
                                                         (float)this->height(),projection, mCamera);
            QVector3D* intersect = rayCaster.findIntersection(mCamera->toMatrix()*QVector3D(0.0,0.0,0.0), d, triangles);
            if (intersect != NULL)
            {
                currentStroke->addPoint(*intersect);
                lastMousePosition = mousePressPosition;
            }
            else
            {
                currentStroke->endStroke();
                qDebug() << "Ending Stroke";
                stroking = false;
                currentStroke = NULL;
            }
            update();
        }
    }

}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if (stroking)
    {
        qDebug() << "Ending Stroke";
        qDebug() << "Stroke Count = " << mLayer->getStrokeCount();
        currentStroke->endStroke();
        stroking = false;
        currentStroke = NULL;
    }

    update();
}

void MainWindow::keyPressEvent(QKeyEvent *k)
{
    if (k->key() == Qt::Key_Space)
    {
        if(mLayer == mLayer1)
            mLayer = mLayer2;
        else
            mLayer = mLayer1;
        update();
    }
}

bool MainWindow::event(QEvent *e)
{
    return QWidget::event(e);
}

void MainWindow::timerEvent(QTimerEvent *)
{
    update();
}

void MainWindow::initializeGL()
{

    initializeOpenGLFunctions();

    qDebug() << "Widget OpenGl: " << format().majorVersion() << "." << format().minorVersion();
    qDebug() << "Context valid: " << context()->isValid();
    qDebug() << "Really used OpenGl: " << context()->format().majorVersion() << "." << context()->format().minorVersion();
    qDebug() << "OpenGl information: VENDOR:       " << (const char*)glGetString(GL_VENDOR);
    qDebug() << "                    RENDERDER:    " << (const char*)glGetString(GL_RENDERER);
    qDebug() << "                    VERSION:      " << (const char*)glGetString(GL_VERSION);
    qDebug() << "                    GLSL VERSION: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

    glClearColor(1,1,1,1);

    initShaders();
    initGeometry();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);

    timer.start(12, this);
}

void MainWindow::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/lines.vs"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/lines.fs"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}

void MainWindow::initGeometry()
{
    triangles.append(QVector3D(3,3,-5));
    triangles.append(QVector3D(-3,3,-5));
    triangles.append(QVector3D(0,-3,-5));
}

void MainWindow::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    aspect = float(w) / float(h ? h : 1);

    const float zNear = 1.0f, zFar = 100.0f, fov =  60.0f;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
    //projection = mCamera->fovToPerspective(fov, aspect, zNear, zFar);
}

void MainWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.setUniformValue("mvp_matrix", projection * mCamera->toMatrix());
    program.setUniformValue("color", QVector4D(0,0,0,1));
    program.setUniformValue("aspect", aspect);

    program.setUniformValue("thickness", 0.1f);
    program.setUniformValue("miter", 1);
    mLayer->drawLayer(&program);

}

bool MainWindow::notAdjecnt(QVector2D p0, QVector2D p1, int threshhold)
{
    return std::abs(p1.x() - p0.x()) + std::abs(p1.y() - p0.y()) > threshhold;
}
