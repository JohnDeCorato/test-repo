#include "planewindow.h"
#include "mainwindow.h"

#include <QtWidgets>

PlaneWindow::PlaneWindow(MainWindow *w, QWidget *parent) : QMainWindow(parent)
{
    QFrame *frame = new QFrame;
    frameLayout = new QHBoxLayout(frame);

    mPlaneWidget = new PlaneWidget(w);


    frameLayout->addWidget(mPlaneWidget);

    setCentralWidget(frame);
}

QVector<QVector3D> PlaneWindow::getPlane()
{
    return mPlaneWidget->getPlane();
}
