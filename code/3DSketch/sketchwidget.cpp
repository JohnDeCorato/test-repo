#define _USE_MATH_DEFINES
#include "sketchwidget.h"

#include <QMouseEvent>
#include <surfaceapplication.h>
#include <iostream>
#include <gestureengine.h>

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

    setAttribute(Qt::WA_AcceptTouchEvents);
}

SketchWidget::~SketchWidget()
{
    makeCurrent();

    doneCurrent();
}

void SketchWidget::mousePressEvent(QMouseEvent *e)
{
    setFocus();
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
    //mLineWidth = 0.02f;
    qDebug() << mLineWidth;
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

void SketchWidget::tabletPressEvent(QTabletEvent *e)
{
    setFocus();
    currPos = QVector2D(e->pos());
    bool intersect = manager.stroke(currPos.x(), currPos.y(), mCamera, mLineWidth, rayCaster, triangles);

    qDebug() << "Beginning Stroke";

    lastPos = currPos;
}

void SketchWidget::tabletMoveEvent(QTabletEvent *e)
{
    currPos = QVector2D(e->pos());

    if (notAdjecnt(currPos, lastPos, 2.0f))
    {
        bool intersect = manager.stroke(currPos.x(), currPos.y(), mCamera, mLineWidth, rayCaster, triangles);
        lastPos = currPos;
    }
}

void SketchWidget::tabletReleaseEvent(QTabletEvent *e)
{
    manager.endStroke();
}

void SketchWidget::touchBeginEvent(QTouchEvent *e)
{
    setFocus();
    for (int i = 0; i < e->touchPoints().size(); i++)
    {
        qDebug() << "Adding Touch Point";
        GestureEngine::instance()->addTouchPoint(e->touchPoints()[i]);
    }
    GestureEngine::instance()->getGestures();
}

void SketchWidget::touchUpdateEvent(QTouchEvent *e)
{
    for (int i = 0; i < e->touchPoints().size(); i++)
    {
        if (e->touchPoints()[i].state() & Qt::TouchPointPressed)
        {
            qDebug() << "Adding Touch Point";
            GestureEngine::instance()->addTouchPoint(e->touchPoints()[i]);
        }
        else if (e->touchPoints()[i].state() & Qt::TouchPointReleased)
        {
            qDebug() << "Removing Touch Point";
            GestureEngine::instance()->removeTouchPoint(e->touchPoints()[i]);
        }
        else
        {
            //qDebug() << "Updating";
            GestureEngine::instance()->updateTouchPoint(e->touchPoints()[i]);
        }
    }
    std::vector<gwc::GestureEvent> gestures = GestureEngine::instance()->getGestures();
    for(std::vector<gwc::GestureEvent>::iterator gesture_it = gestures.begin(); gesture_it != gestures.end(); gesture_it++)
    {
        if (gesture_it->gesture_id == "n-drag")
        {
            float dx = gesture_it->values["drag_dx"];
            float dy = gesture_it->values["drag_dy"];

            if (gesture_it->n == 1)
                mCamera->rotate(dx, dy);

            if (gesture_it->n == 2)
                mCamera->translate(dx, dy);
        }

        if (gesture_it->gesture_id == "n-scale")
        {
            float dx = gesture_it->values["scale_dsx"];

            if (gesture_it->n == 2)
                mCamera->zoom(dx);
        }
    }
}

void SketchWidget::touchEndEvent(QTouchEvent *e)
{
    for (int i = 0; i < e->touchPoints().size(); i++)
    {
        qDebug() << "Removing Touch Point";
        GestureEngine::instance()->removeTouchPoint(e->touchPoints()[i]);
    }
    qDebug() << "Touch End";
}

bool SketchWidget::event(QEvent *e)
{
    switch (e->type())
    {
    case QEvent::TabletPress:
    {
        e->accept();
        QTabletEvent *te = static_cast<QTabletEvent*>(e);
        tabletPressEvent(te);
        return true;
    }
    case QEvent::TabletMove:
    {
        e->accept();
        QTabletEvent *te = static_cast<QTabletEvent*>(e);
        tabletMoveEvent(te);
        return true;
    }
    case QEvent::TabletRelease:
    {
        e->accept();
        QTabletEvent *te = static_cast<QTabletEvent*>(e);
        tabletReleaseEvent(te);
        return true;
    }
    case QEvent::TouchBegin:
    {
        e->accept();
        QTouchEvent *te = static_cast<QTouchEvent*>(e);
        touchBeginEvent(te);
        return true;
    }
    case QEvent::TouchUpdate:
    {
        e->accept();
        QTouchEvent *te = static_cast<QTouchEvent*>(e);
        touchUpdateEvent(te);
        return true;
    }
    case QEvent::TouchEnd:
    {
        e->accept();
        QTouchEvent *te = static_cast<QTouchEvent*>(e);
        touchEndEvent(te);
        return true;
    }
    case QEvent::KeyPress:
    {
        qDebug() << "hi";
    }
    default:
        return QWidget::event(e);
    }
}

void SketchWidget::timerEvent(QTimerEvent *)
{
    update();
}

void SketchWidget::saveSketch(QString fileName)
{
    ofstream file(fileName.toStdString().c_str());
    if (file.is_open())
    {
        manager.saveSketch(file);
        file.close();
        qDebug() << "File saved";
    }
}

void SketchWidget::loadSketch(QString fileName)
{
    ifstream file(fileName.toStdString().c_str());
    if (file.is_open())
    {
        manager.loadSketch(file);
        file.close();
        qDebug() << "File loaded";
    }
}

void SketchWidget::loadModel(QString fileName)
{
    manager.loadOBJ(fileName.toStdString());
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

    glClearColor(0.7,0.7,0.7,0.7);

    initShaders();
    initGeometry();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_CULL_FACE);

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

void SketchWidget::setPlane(QVector<QVector3D> points)
{
    triangles = points;
    qDebug() << points.length();
    geomShader->bindBuffers(triangles);
}

void SketchWidget::initGeometry()
{

//    triangles.append(QVector3D(2,2,-5));
//    triangles.append(QVector3D(-2,2,-5));
//    triangles.append(QVector3D(2,-2,-5));
//    triangles.append(QVector3D(-2,2,-5));
//    triangles.append(QVector3D(2,-2,-5));
//    triangles.append(QVector3D(-2,-2,-5));
//    triangles.append(QVector3D(2,2,-9));
//    triangles.append(QVector3D(-2,2,-9));
//    triangles.append(QVector3D(2,-2,-9));
//    triangles.append(QVector3D(-2,2,-9));
//    triangles.append(QVector3D(2,-2,-9));
//    triangles.append(QVector3D(-2,-2,-9));
//    triangles.append(QVector3D(2,-2,-5));
//    triangles.append(QVector3D(-2,-2,-5));
//    triangles.append(QVector3D(2,-2,-9));
//    triangles.append(QVector3D(-2,-2,-5));
//    triangles.append(QVector3D(2,-2,-9));
//    triangles.append(QVector3D(-2,-2,-9));

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

    //currentShader->setUniformValue("thickness", .1f);
    currentShader->setUniformValue("miter", 1);
    currentShader->setUniformValue("join_type", 0);

    Layer* l = manager.getLayerByName("Layer 1");
    renderLayer(l, mCamera, currentShader);
    //mLayer->drawLayer(glLinesShader);

    geomShader->bind();
    geomShader->bindBuffers(triangles);
    geomShader->setUniformValue("mvp_matrix", mCamera->getMVPMatrix());
    geomShader->setUniformValue("color", QVector3D(0.8f,0.8f,0.8f));
    geomShader->setUniformValue("aspect", aspect);
    geomShader->setUniformValue("wireframe_width", 0.002f);
    geomShader->render();

}

bool SketchWidget::notAdjecnt(QVector2D p0, QVector2D p1, float threshhold)
{
    return std::abs(p1.x() - p0.x()) + std::abs(p1.y() - p0.y()) > threshhold;
}
