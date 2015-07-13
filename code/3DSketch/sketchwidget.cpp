#define _USE_MATH_DEFINES
#include "sketchwidget.h"

#include <QMouseEvent>
#include <iostream>

#include <math.h>

int color = 0;

SketchWidget::SketchWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
    //setSurfaceType(OpenGLSurface);


    mLayer1 = new Layer("Layer 1");
    mLayer2 = new Layer("Layer 2");

    manager.addLayer("Layer 1");

    mLayer = mLayer1;
    stroking = false;

    mLineWidth = 0.1f;

    const float zNear = 1.0f, zFar = 100.0f, fov =  60.0f;
    mCamera = new Camera(QVector3D(0,0,-2), QVector3D(0,0,-3), 0, 0, fov, zNear, zFar);
}

SketchWidget::~SketchWidget()
{
    makeCurrent();

    doneCurrent();
}

void SketchWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
    bool intersect = manager.stroke(mousePressPosition.x(), mousePressPosition.y(), mCamera, mLineWidth, rayCaster, triangles);
//    QVector3D d = rayCaster.generateRayDirection(mousePressPosition, (float)this->width(),
//                                                 (float)this->height(), projection, mCamera);
//    QVector3D* intersect = rayCaster.findIntersection(mCamera->position(), d, triangles);
    if (intersect)
    {

        qDebug() << "Beginning Stroke";
        stroking = true;
//        currentStroke = mLayer->createNewStroke(Qt::black);
//        currentStroke->addPoint(*intersect);


    }
    else
    {
        qDebug() << "No intersection found. No stroke created.";
    }

    lastMousePosition = mousePressPosition;
}

void SketchWidget::mouseMoveEvent(QMouseEvent *e)
{
    mousePressPosition = QVector2D(e->localPos());
    if (stroking)
    {

        if (notAdjecnt(mousePressPosition, lastMousePosition, 0))
        {
            bool intersect = manager.stroke(mousePressPosition.x(), mousePressPosition.y(), mCamera, mLineWidth, rayCaster, triangles);
            //QVector3D d = rayCaster.generateRayDirection(mousePressPosition, (float)this->width(),
                                                         //(float)this->height(),projection, mCamera);
            //QVector3D* intersect = rayCaster.findIntersection(mCamera->position(), d, triangles);
            //if (intersect != NULL)
            //{
                //currentStroke->addPoint(*intersect);
                //lastMousePosition = mousePressPosition;
            //}
            //else
            //{
                //mLayer->endCurrentStroke();
                //qDebug() << "Ending Stroke";
                //stroking = false;


            //}
            //update();
        }
    }
    else
    {
        if (e->buttons() == Qt::LeftButton)
        {
            mCamera->rotate(mousePressPosition.x() - lastMousePosition.x(),
                            mousePressPosition.y() - lastMousePosition.y());
        }
        else if (e->buttons() == Qt::RightButton)
        {
            mCamera->translate(mousePressPosition.x() - lastMousePosition.x(),
                            mousePressPosition.y() - lastMousePosition.y());
        }
        lastMousePosition = mousePressPosition;
    }

}

void SketchWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (stroking)
    {
        manager.endStroke();
        //mLayer->endCurrentStroke();
        //qDebug() << "Size = " << currentStroke->getStroke().size();
        //if (currentStroke->getStroke().size() == 1)
        //{
            //mLayer->deleteLastStroke();
        //}

        qDebug() << "Ending Stroke";
        //qDebug() << "Stroke Count = " << mLayer->getStrokeCount();
        stroking = false;

    }
    //currentStroke = NULL;
    //update();
}

void SketchWidget::keyPressEvent(QKeyEvent *k)
{
    if (k->key() == Qt::Key_Space)
    {
        if(mLayer == mLayer1)
            mLayer = mLayer2;
        else
            mLayer = mLayer1;
        //update();
    }
    if (k->key() == Qt::Key_Tab)
    {
        if(currentShader == glLinesShader)
        {
            qDebug() << "Current Shader: VERTEX-BASED LINE SHADER";
            currentShader = vertexLineShader;
        }
        else if(currentShader == vertexLineShader)
        {
            qDebug() << "Current Shader: GEOMETRY-BASED LINE SHADER";
            currentShader = lineShader;
        }
        else
        {
            qDebug() << "Current Shader: DEFAULT GL_LINES";
            currentShader = glLinesShader;
        }
    }

    if (k->key() == Qt::Key_S)
    {
        ofstream file("test.txt");
        if (file.is_open())
        {
            manager.saveSketch(file);
            file.close();
            qDebug() << "File saved";
        }
    }

    if (k->key() == Qt::Key_L)
    {
        ifstream file("test.txt");
        if (file.is_open())
        {
            manager.loadSketch(file);
            file.close();
            qDebug() << "File loaded";
        }
    }
}

void SketchWidget::wheelEvent(QWheelEvent *e)
{
    QPoint numDegrees = e->angleDelta();
    mCamera->zoom(numDegrees.y());
}

bool SketchWidget::event(QEvent *e)
{
    return QWidget::event(e);
}

void SketchWidget::timerEvent(QTimerEvent *)
{
    update();
}

void SketchWidget::initializeGL()
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

void SketchWidget::initShaders()
{
//    // Compile vertex shader
//    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vs"))
//        close();

//    // Compile fragment shader
//    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.fs"))
//        close();

//    // Link shader pipeline
//    if (!program.link())
//        close();

//    // Bind shader pipeline for use
//    if (!program.bind())
//        close();
    glLinesShader = new GLLinesShader();
    glLinesShader->init();

    vertexLineShader = new VertexBasedLineShader();
    vertexLineShader->init();

    lineShader = new LineShader();
    lineShader->init();

    currentShader = lineShader;

    geomShader = new SinglePassWireframeShader();
    geomShader->init();


}

void SketchWidget::initGeometry()
{
    triangles.append(QVector3D(2,2,-5));
    triangles.append(QVector3D(-2,2,-5));
    triangles.append(QVector3D(0,-2,-5));
    triangles.append(QVector3D(0,-2,-5));
    triangles.append(QVector3D(-2,2,-5));
    triangles.append(QVector3D(-2,-2,-5));

    geomShader->bindBuffers(triangles);
}

void SketchWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    aspect = float(w) / float(h ? h : 1);

    //const float zNear = 1.0f, zFar = 100.0f, fov =  60.0f;

    // Reset projection
    //projection.setToIdentity();

    // Set perspective projection
    //projection.perspective(fov, aspect, zNear, zFar);
    mCamera->setProjectionMatrix(float(w), float (h));
    //projection = mCamera->fovToPerspective(fov, aspect, zNear, zFar);
}

void SketchWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    program.setUniformValue("mvp_matrix", projection * mCamera->toMatrix());
//    program.setUniformValue("color", QVector4D(0,0,0,1));
//    program.setUniformValue("aspect", aspect);

//    program.setUniformValue("thickness", 0.1f);
//    program.setUniformValue("miter", 1);
//    mLayer->drawLayer(&program);
    //qDebug() << "Regular: " << projection;
    //qDebug() << "Camera: " << mCamera->getProjectionMatrix();

    currentShader->bind();
    currentShader->setUniformValue("mvp_matrix", mCamera->getMVPMatrix());
    //currentShader->setUniformValue("color", QVector4D(0,0,0,1));
    currentShader->setUniformValue("aspect", aspect);

    //currentShader->setUniformValue("thickness", 0.2f);
    currentShader->setUniformValue("miter", 1);
    currentShader->setUniformValue("join_type", 0);

    Layer* l = manager.getLayerByName("Layer 1");

    if (currentShader == glLinesShader)
    {
        renderLayer(l, mCamera, glLinesShader);
    }
    else if(currentShader == vertexLineShader)
    {
        renderLayer(l, mCamera, vertexLineShader);
    }
    else if (currentShader == lineShader)
    {
        renderLayer(l, mCamera, lineShader);
    }
    //mLayer->drawLayer(glLinesShader);

    geomShader->bind();
    geomShader->bindBuffers(triangles);
    geomShader->setUniformValue("mvp_matrix", mCamera->getMVPMatrix());
    geomShader->setUniformValue("color", QVector3D(0.8f,0.8f,0.8f));
    geomShader->setUniformValue("aspect", aspect);
    geomShader->setUniformValue("wireframe_width", 0.002f);
    geomShader->render();

}

bool SketchWidget::notAdjecnt(QVector2D p0, QVector2D p1, int threshhold)
{
    return std::abs(p1.x() - p0.x()) + std::abs(p1.y() - p0.y()) > threshhold;
}
