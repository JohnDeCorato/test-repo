#include "mainwindow.h"

#include <QMouseEvent>
#include <iostream>

#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
    QOpenGLWidget(parent)
{
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
        QVector3D d = rayCaster.generateRayDirection(mousePressPosition, (float)this->width(),
                                                     (float)this->height(),projection, mCamera);
        QVector3D* intersect = rayCaster.findIntersection(mCamera->toMatrix()*QVector3D(0.0,0.0,0.0), d, triangles);
        if (intersect != NULL)
        {
            currentStroke->addPoint(*intersect);
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

void MainWindow::timerEvent(QTimerEvent *)
{
    //update();
}

void MainWindow::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(1,1,1,1);

    initShaders();
    initGeometry();

    glEnable(GL_DEPTH_TEST);

    timer.start(12, this);
}

void MainWindow::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vs"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.fs"))
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
    triangles.append(QVector3D(2,2,-5));
    triangles.append(QVector3D(-2,2,-5));
    triangles.append(QVector3D(0,-2,-5));
}

void MainWindow::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);


    const qreal zNear = 1.0, zFar = 100.0, fov =  60.0;

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
    mLayer->drawLayer(&program);

}
