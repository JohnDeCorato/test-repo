#-------------------------------------------------
#
# Project created by QtCreator 2015-06-03T22:27:56
#
#-------------------------------------------------

QT       += core gui widgets opengl

TARGET = 3DSketch
TEMPLATE = app


SOURCES += main.cpp\
    stroke.cpp \
    raycaster.cpp \
    camera.cpp \
    layer.cpp \
    shaderprogram.cpp \
    lineshader.cpp \
    gllinesshader.cpp \
    layerrenderer.cpp \
    vertexbasedlineshader.cpp \
    singlepasswireframeshader.cpp \
    sketchmanager.cpp \
    sketchwidget.cpp \
    mainwindow.cpp \
    inputengine.cpp \
    surfaceapplication.cpp \
    GestureWorksCore.cpp \
    GWCUtils.cpp \
    gestureengine.cpp \
    touchapplication.cpp \
    touchinputfilter.cpp

HEADERS  += \
    stroke.h \
    raycaster.h \
    camera.h \
    layer.h \
    shaderprogram.h \
    lineshader.h \
    gllinesshader.h \
    layerrenderer.h \
    vertexbasedlineshader.h \
    singlepasswireframeshader.h \
    sketchmanager.h \
    sketchwidget.h \
    mainwindow.h \
    inputengine.h \
    surfaceapplication.h \
    GestureWorksCore.h \
    GWCUtils.h \
    gestureengine.h \
    touchapplication.h \
    touchinputfilter.h

RESOURCES += \
    resources.qrc



win32: LIBS += -L$$PWD/../Embree/lib/ -lembree

INCLUDEPATH += $$PWD/../Embree/include
DEPENDPATH += $$PWD/../Embree/include
