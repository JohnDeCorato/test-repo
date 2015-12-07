#include "planewidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include "mainwindow.h"

PlaneWidget::PlaneWidget(MainWindow *w, QWidget *parent)
    : QWidget(parent)
{
    createSpinBoxes();

    m_button = new QPushButton("Update Plane", this);
    connect(m_button, SIGNAL(released()), w, SLOT(updatePlane()));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(point1);
    layout->addWidget(point2);
    layout->addWidget(point3);
    layout->addWidget(point4);
    layout->addWidget(m_button);
    setLayout(layout);

    setWindowTitle(tr("Plane Editor"));
}

QVector<QVector3D> PlaneWidget::getPlane(){
    QVector3D p1 = QVector3D(point1x->value(),point1y->value(),point1z->value());
    QVector3D p2 = QVector3D(point2x->value(),point2y->value(),point2z->value());
    QVector3D p3 = QVector3D(point3x->value(),point3y->value(),point3z->value());
    QVector3D p4 = QVector3D(point4x->value(),point4y->value(),point4z->value());

    QVector<QVector3D> points;
    points.append(p1);
    points.append(p2);
    points.append(p3);
    points.append(p2);
    points.append(p3);
    points.append(p4);

    qDebug() << "hi";

    return points;
}

void PlaneWidget::createSpinBoxes()
{
    QLabel *p1Label = new QLabel(tr("Point 1"));
    point1x = new QDoubleSpinBox;
    point1x->setRange(-20.0, 20.0);
    point1x->setSingleStep(1.0);
    point1x->setValue(2.0);
    point1y = new QDoubleSpinBox;
    point1y->setRange(-20.0, 20.0);
    point1y->setSingleStep(1.0);
    point1y->setValue(2.0);
    point1z = new QDoubleSpinBox;
    point1z->setRange(-20.0, 20.0);
    point1z->setSingleStep(1.0);
    point1z->setValue(-5.0);

    QVBoxLayout *p1Layout = new QVBoxLayout;
    p1Layout->addWidget(p1Label);
    p1Layout->addWidget(point1x);
    p1Layout->addWidget(point1y);
    p1Layout->addWidget(point1z);

    point1 = new QGroupBox();
    point1->setLayout(p1Layout);



    QLabel *p2Label = new QLabel(tr("Point 2"));
    point2x = new QDoubleSpinBox;
    point2x->setRange(-20.0, 20.0);
    point2x->setSingleStep(1.0);
    point2x->setValue(2.0);
    point2y = new QDoubleSpinBox;
    point2y->setRange(-20.0, 20.0);
    point2y->setSingleStep(1.0);
    point2y->setValue(-2.0);
    point2z = new QDoubleSpinBox;
    point2z->setRange(-20.0, 20.0);
    point2z->setSingleStep(1.0);
    point2z->setValue(-5.0);

    QVBoxLayout *p2Layout = new QVBoxLayout;
    p2Layout->addWidget(p2Label);
    p2Layout->addWidget(point2x);
    p2Layout->addWidget(point2y);
    p2Layout->addWidget(point2z);

    point2 = new QGroupBox();
    point2->setLayout(p2Layout);

    QLabel *p3Label = new QLabel(tr("Point 3"));
    point3x = new QDoubleSpinBox;
    point3x->setRange(-20.0, 20.0);
    point3x->setSingleStep(1.0);
    point3x->setValue(-2.0);
    point3y = new QDoubleSpinBox;
    point3y->setRange(-20.0, 20.0);
    point3y->setSingleStep(1.0);
    point3y->setValue(2.0);
    point3z = new QDoubleSpinBox;
    point3z->setRange(-20.0, 20.0);
    point3z->setSingleStep(1.0);
    point3z->setValue(-5.0);

    QVBoxLayout *p3Layout = new QVBoxLayout;
    p3Layout->addWidget(p3Label);
    p3Layout->addWidget(point3x);
    p3Layout->addWidget(point3y);
    p3Layout->addWidget(point3z);

    point3 = new QGroupBox();
    point3->setLayout(p3Layout);

    QLabel *p4Label = new QLabel(tr("Point 4"));
    point4x = new QDoubleSpinBox;
    point4x->setRange(-20.0, 20.0);
    point4x->setSingleStep(1.0);
    point4x->setValue(-2.0);
    point4y = new QDoubleSpinBox;
    point4y->setRange(-20.0, 20.0);
    point4y->setSingleStep(1.0);
    point4y->setValue(-2.0);
    point4z = new QDoubleSpinBox;
    point4z->setRange(-20.0, 20.0);
    point4z->setSingleStep(1.0);
    point4z->setValue(-5.0);

    QVBoxLayout *p4Layout = new QVBoxLayout;
    p4Layout->addWidget(p4Label);
    p4Layout->addWidget(point4x);
    p4Layout->addWidget(point4y);
    p4Layout->addWidget(point4z);

    point4 = new QGroupBox();
    point4->setLayout(p4Layout);
}

