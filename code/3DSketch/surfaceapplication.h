#ifndef SURFACEAPPLICATION_H
#define SURFACEAPPLICATION_H

#include <QApplication>


class SurfaceApplication : public QApplication
{
public:
    SurfaceApplication(int& argc, char** argv);

    bool notify(QObject *receiver, QEvent *event);

    static int tabletButtons() { return mTabletButtons; }
    static void setTabletButtons(int btns) { mTabletButtons = btns; }

private:
    bool sendMouseEvent(QObject* receiver, QEvent::Type mevtype, QPoint globalPos, Qt::KeyboardModifiers modifiers);
    QObject* getRecvWindow(QObject* candidate);

    int activeTouchId;
    int acceptCount;
    enum {None, PassThrough, TouchInput, TabletInput} inputState;
    static int mTabletButtons;
};

#endif // SURFACEAPPLICATION_H
