#include "sketchwidget.h"
#include "mainwindow.h"
#include "inputengine.h"
#include "surfaceapplication.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    // Enables the mouse as a pointing device. Might be useful for debugging
    // but can update slowly on large movements
    //
    //EnableMouseInPointer(true);

    SurfaceApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(25);
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("3D Sketch");
    app.setApplicationVersion("0.6");
#ifndef QT_NO_OPENGL
    //SketchWidget widget;
    //widget.show();

    MainWindow w;
    w.show();

    // Setting up our application to handle our custom input manager
    //HWND hWnd = (HWND) w.winId();
    //overrideWindowProcedure(hWnd);

    qDebug() << "********************";
    qDebug() << "3D Sketch Debug Info";
    qDebug() << "********************";
#else
    QLabel note("OpenGL Support required");
    note.show();
#endif

    return app.exec();
}
