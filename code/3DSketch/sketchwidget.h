#ifndef SKETCHWIDGET_H
#define SKETCHWIDGET_H

#include "stroke.h"
#include "camera.h"
#include "raycaster.h"
#include "layer.h"
#include "layerrenderer.h"
#include "gllinesshader.h"
#include "vertexbasedlineshader.h"
#include "lineshader.h"
#include "singlepasswireframeshader.h"
#include "sketchmanager.h"

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTouchEvent>


class SketchWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit SketchWidget(QWidget *parent = 0);
    ~SketchWidget();

    void setLineWidth(float w);
    void saveSketch();
    void loadSketch();

protected:
    // Mouse / Keyboard Event Functions
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *k) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *e) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;

    // Tablet Event Functions
    void tabletPressEvent(QTabletEvent *e);
    void tabletMoveEvent(QTabletEvent *e);
    void tabletReleaseEvent(QTabletEvent *e);

    // Touch Event Functions
    void touchBeginEvent(QTouchEvent *e);
    void touchUpdateEvent(QTouchEvent *e);
    void touchEndEvent(QTouchEvent *e);

    // Event Handler
    bool event(QEvent *e) Q_DECL_OVERRIDE;

    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

    void initShaders();
    void initGeometry();

private:
    // Checks if two mouse positions are not in pixels between the threshhold
    bool notAdjecnt(QVector2D p0, QVector2D p1, int threshhold);

    QBasicTimer timer;
    QOpenGLShaderProgram program;

    QMatrix4x4 projection;
    QMatrix4x4 rasterProj;

    QVector2D lastMousePosition;
    QVector2D mousePressPosition;

    QVector2D currPos;
    QVector2D lastPos;

    Camera *mCamera;
    RayCaster rayCaster;
    QVector<QVector3D> triangles;

    SketchManager manager;



    Layer *mLayer;
    Layer *mLayer1;
    Layer *mLayer2;
    Stroke* currentStroke;
    bool stroking;

    float aspect;

    GLLinesShader *glLinesShader;
    VertexBasedLineShader *vertexLineShader;
    LineShader *lineShader;
    ShaderProgram *currentShader;

    SinglePassWireframeShader *geomShader;

    float mLineWidth;

};

inline void SketchWidget::setLineWidth(float w) { mLineWidth = w; }

#endif // SKETCHWIDGET_H
