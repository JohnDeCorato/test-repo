#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "sketchwidget.h"

class QHBoxLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private slots:
    void saveSketch();
    void loadSketch();
    void openModelFile();

private:
    void buildMenus();
    void buildWidgets();

    SketchWidget *mSketchWidget;

    QHBoxLayout *frameLayout;
};

#endif // MAINWINDOW_H
