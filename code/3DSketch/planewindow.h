#ifndef PLANEWINDOW_H
#define PLANEWINDOW_H

#include <QMainWindow>
#include "planewidget.h"
#include <QHBoxLayout>

class MainWindow;

class PlaneWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit PlaneWindow(MainWindow *w, QWidget *parent = 0);
    QVector<QVector3D> getPlane();

signals:

public slots:

private:
    PlaneWidget *mPlaneWidget;
    QHBoxLayout *frameLayout;
};

#endif // PLANEWINDOW_H
