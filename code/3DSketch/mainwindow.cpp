#include "mainwindow.h"
//#include "planewidget.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    mPlaneWindow = new PlaneWindow(this);

    buildMenus();
    buildWidgets();



    setWindowTitle(tr("3D Sketch"));
}

void MainWindow::buildMenus()
{
    // File Menu Options
    QMenu *fileMenu = menuBar()->addMenu(tr("File"));

    QAction *saveAction = fileMenu->addAction(tr("Save Sketch"));
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveSketch()));

    QAction *loadAction = fileMenu->addAction(tr("Open Sketch"));
    loadAction->setShortcut(QKeySequence::Open);
    connect(loadAction, SIGNAL(triggered()), this, SLOT(loadSketch()));

    QAction *exitAction = fileMenu->addAction(tr("Exit"));
    exitAction->setShortcut(QKeySequence::Quit);

    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    // Sketching Menu Options
    QMenu *sketchMenu = menuBar()->addMenu(tr("Sketching"));

    QAction *widthAction = sketchMenu->addAction(tr("Change Line Width"));

    QAction *modelAction = sketchMenu->addAction(tr("Load OBJ File for Sketch Surface"));
    connect(modelAction, SIGNAL(triggered()), this, SLOT(openModelFile()));

    QAction *planeAction = sketchMenu->addAction(tr("Change Drawing Plane"));
    connect(planeAction, SIGNAL(triggered()), this, SLOT(openPlaneEditor()));

}

void MainWindow::buildWidgets()
{
    QFrame *frame = new QFrame;
    frameLayout = new QHBoxLayout(frame);

    mSketchWidget = new SketchWidget();


    frameLayout->addWidget(mSketchWidget);

    setCentralWidget(frame);
}

void MainWindow::openPlaneEditor()
{
    QPoint pos = mPlaneWindow->pos();
    pos.setX(0);
    pos.setY(0);
    mPlaneWindow->move(pos);
    mPlaneWindow->show();
}

void MainWindow::saveSketch()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Sketch"), tr(""), tr("3D Sketch (*.3dsk)"));
    mSketchWidget->saveSketch(fileName);
}

void MainWindow::loadSketch()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Sketch"), tr(""), tr("3D Sketch (*.3dsk)"));
    mSketchWidget->loadSketch(fileName);
}

void MainWindow::openModelFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load OBJ"), tr(""), tr("OBJ File (*.obj)"));
    mSketchWidget->loadModel(fileName);
}

void MainWindow::updatePlane()
{
    mSketchWidget->setPlane(mPlaneWindow->getPlane());
}
