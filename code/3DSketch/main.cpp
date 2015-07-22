#include "sketchwidget.h"
#include "mainwindow.h"
#include "inputengine.h"
#include "surfaceapplication.h"
#include "touchapplication.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QFileDialog>
#include <QString>

#include "GestureWorksCore.h"
#include "GWCUtils.h"

int main(int argc, char *argv[])
{
    // Create our Custom Qt Application
    SurfaceApplication app(argc, argv);

    // Enables the mouse as a pointing device. Might be useful for debugging
    // but can update slowly on large movements
    //
    //EnableMouseInPointer(true);

    // Load GestureWorks Library to Detect Gestures from Touch Input

    // TODO: Deal with this somehow
    ifstream file("gestureworks_location.txt");
    QString dir;
    if (!file.good())
    {
        dir = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(0, QString("Select Gestureworks Directory"),"../",QFileDialog::ShowDirsOnly
                                              | QFileDialog::DontResolveSymlinks));
        qDebug() << dir + "\\GestureWorksCore32.dll";
    }
    else
    {
        string line;
        getline(file, line);
        dir = QString::fromStdString(line);
        qDebug() << dir;
    }

    if (loadGestureWorks(dir + "\\GestureWorksCore32.dll"))
    {
        qDebug() << "Error loading gestureworks dll";
    }
    else
    {
        qDebug() << "Gestureworks dll loaded";
        if (!file.good())
        {
            ofstream out("gestureworks_location.txt");
            out << dir.toStdString();
        }
    }

    if (!loadGML((dir + "\\basic_manipulation.gml").toStdString()))
    {
        qDebug() << "Could not find gml file";
    }

    initializeGestureWorks(1920,1080);

    qDebug() << "Adding gestures";
    registerTouchObject("sketch");
    addGesture("sketch", "n-drag");
    addGesture("sketch", "n-rotate");
    addGesture("sketch", "n-scale");




    // OpenGL Settings
    QSurfaceFormat format;
    format.setSamples(25);
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("3D Sketch");
    app.setApplicationVersion("0.7");



#ifndef QT_NO_OPENGL
    //SketchWidget widget;
    //widget.show();

    MainWindow w;
    w.show();


    // Setting up our application to handle our custom input manager
    //HWND hWnd = (HWND) w.winId();
    //overrideWindowProcedure(hWnd);

    //registerWindowForTouchByName("3D Sketch");

    qDebug() << "********************";
    qDebug() << "3D Sketch Debug Info";
    qDebug() << "********************";
#else
    QLabel note("OpenGL Support required");
    note.show();
#endif


    return app.exec();
}
