#-------------------------------------------------
#
# Project created by QtCreator 2015-06-03T22:27:56
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = 3DSketch
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    stroke.cpp \
    raycaster.cpp \
    camera.cpp \
    layer.cpp \
    lineshader.cpp

HEADERS  += mainwindow.h \
    stroke.h \
    raycaster.h \
    camera.h \
    layer.h \
    lineshader.h

RESOURCES += \
    resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Embree/ -lembree
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Embree/ -lembree

INCLUDEPATH += $$PWD/../Embree/include
DEPENDPATH += $$PWD/../Embree/include
