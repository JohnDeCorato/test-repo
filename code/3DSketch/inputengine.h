#ifndef INPUTENGINE_H
#define INPUTENGINE_H

#include <windows.h>
#include <WinUser.h>
#include <QDebug>

#include <QEvent>
#include <QWindow>
#include <QApplication>
#include <QCoreApplication>
#include <QAbstractNativeEventFilter>
#include <QTabletEvent>

//////////////////////////////////////////////
/// INPUT ENGINE
//////////////////////////////////////////////
///
/// This class contains code to override the event
/// handling of a given window in order to handle
/// pen and touch input. For a non-QT application,
/// you want to call overrideWindowProcedure and
/// implement your own versions of handlePenInput and
/// handleTouchInput. You will also want to delete the
/// code below those two functions. For a QT Application,
/// you can replace QApplication with SurfaceApplication.
/// Please refer to that class if necissary.

LRESULT CALLBACK NewWndProc(HWND hwnd, UINT uMsg, WPARAM mParam, LPARAM lParam);

// METHOD 1: Override a Window
int overrideWindowProcedure(HWND & hWnd);

// Input Management Functions
static void handlePenInput(POINTER_PEN_INFO& ppi, QEvent::Type eventtype);
static void handlePenHistory(UINT32 ptrid);
static bool handleTouchInput(UINT32 ptrid, Qt::TouchPointState eventtype);

////////////////////////////////////
/// QT STUFF
////////////////////////////////////
class SurfaceApplication;

class SurfaceHelperObject : public QObject
{
    Q_OBJECT

private slots:
    void tabletWindowDestroyed();
    void touchWindowDestroyed();
};


// Sends input to QT Application as standard events. ONLY WORKS ON WINDOWS
class InputFilter : public QAbstractNativeEventFilter
{
    friend class SurfaceHelperObject;
public:
    InputFilter();
    ~InputFilter();

    static InputFilter* instace() { return mInstance; }
    void notifyTouchEvent(Qt::TouchPointStates touchstate, const QList<QTouchEvent::TouchPoint>& _points);
    void notifyTabletEvent(QEvent::Type eventtype, const QPointF& globalpos, qreal pressure, QTabletEvent::PointerType ptrtype, int buttons, int deviceid);

    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);

protected:
    QWindow* tabletTarget;
    QWindow* touchTarget;
    SurfaceApplication* surfaceApp;
    QTouchDevice touchDevice;
    SurfaceHelperObject* helperObject;

    static InputFilter* mInstance;
};

#endif // INPUTENGINE_H
