#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(25);
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("3D Sketch");
    app.setApplicationVersion("0.6");
#ifndef QT_NO_OPENGL
    MainWindow widget;
    widget.show();
#else
    QLabel note("OpenGL Support required");
    note.show();
#endif

    return app.exec();
}
