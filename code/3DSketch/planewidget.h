#ifndef PLANEWIDGET_H
#define PLANEWIDGET_H

#include "sketchwidget.h"
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QPushButton>

class MainWindow;

class PlaneWidget : public QWidget
{
    Q_OBJECT

public:
    PlaneWidget(MainWindow *w, QWidget *parent = 0);
    QVector<QVector3D> getPlane();

private:

    void createSpinBoxes();

    QDoubleSpinBox *point1x;
    QDoubleSpinBox *point1y;
    QDoubleSpinBox *point1z;
    QDoubleSpinBox *point2x;
    QDoubleSpinBox *point2y;
    QDoubleSpinBox *point2z;
    QDoubleSpinBox *point3x;
    QDoubleSpinBox *point3y;
    QDoubleSpinBox *point3z;
    QDoubleSpinBox *point4x;
    QDoubleSpinBox *point4y;
    QDoubleSpinBox *point4z;

    QGroupBox *point1;
    QGroupBox *point2;
    QGroupBox *point3;
    QGroupBox *point4;

    QPushButton *m_button;
};

#endif // PLANEWIDGET_H
