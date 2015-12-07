#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "sketchwidget.h"
#include "planewindow.h"

class QHBoxLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

public slots:
    void updatePlane();

private slots:
    void saveSketch();
    void loadSketch();
    void openModelFile();
    void openPlaneEditor();

private:
    void buildMenus();
    void buildWidgets();

    SketchWidget *mSketchWidget;

    QHBoxLayout *frameLayout;

    PlaneWindow *mPlaneWindow;
};

#endif // MAINWINDOW_H
