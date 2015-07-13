#include "mainwindow.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
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

    QAction *loadAction = fileMenu->addAction(tr("Open Sketch"));
    loadAction->setShortcut(QKeySequence::Open);

    QAction *exitAction = fileMenu->addAction(tr("Exit"));
    exitAction->setShortcut(QKeySequence::Quit);

    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    // Sketching Menu Options
    QMenu *sketchMenu = menuBar()->addMenu(tr("Sketching"));

    QAction *widthAction = sketchMenu->addAction(tr("Change Line Width"));

}

void MainWindow::buildWidgets()
{
    QFrame *frame = new QFrame;
    frameLayout = new QHBoxLayout(frame);

    mSketchWidget = new SketchWidget();

    frameLayout->addWidget(mSketchWidget);

    setCentralWidget(frame);
}

