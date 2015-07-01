#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stroke.h"
#include "camera.h"
#include "raycaster.h"
#include "layer.h"

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>


class MainWindow : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *k) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;
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
    Camera *mCamera;
    RayCaster rayCaster;
    QVector<QVector3D> triangles;

    Layer *mLayer;
    Layer *mLayer1;
    Layer *mLayer2;
    Stroke* currentStroke;
    bool stroking;

    float aspect;

};

#endif // MAINWINDOW_H
