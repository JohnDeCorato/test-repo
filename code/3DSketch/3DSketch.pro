#-------------------------------------------------
#
# Project created by QtCreator 2015-06-03T22:27:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3DSketch
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    stroke.cpp \
    raycaster.cpp \
    camera.cpp \
    layer.cpp

HEADERS  += mainwindow.h \
    stroke.h \
    raycaster.h \
    camera.h \
    layer.h

RESOURCES += \
    resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../Program Files/Intel/Embree 2.5.1/lib/ -lembree
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../Program Files/Intel/Embree 2.5.1/lib/ -lembreed

INCLUDEPATH += $$PWD/../../../../../../../Program Files/Intel/Embree 2.5.1/include
DEPENDPATH += $$PWD/../../../../../../../Program Files/Intel/Embree 2.5.1/include
