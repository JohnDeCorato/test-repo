#include "surfaceapplication.h"
#include "inputengine.h"

#include <QWindow>
#include <QWidget>
#include <QTabletEvent>

int SurfaceApplication::mTabletButtons = 0;

SurfaceApplication::SurfaceApplication(int &argc, char **argv)
    : QApplication(argc, argv),
      inputState(None)
{
    QCoreApplication::setAttribute(Qt::AA_SynthesizeMouseForUnhandledTouchEvents, false);
    acceptCount = 0;

    installNativeEventFilter(new InputFilter);
}

bool SurfaceApplication::sendMouseEvent(QObject *receiver, QEvent::Type mevtype, QPoint globalPos, Qt::KeyboardModifiers modifiers)
{
    QPoint localPos = globalPos;

    if (receiver->isWidgetType())
    {
        localPos = static_cast<QWidget*>(receiver)->mapFromGlobal(globalPos);
    }
    else if (receiver->isWindowType())
    {
        localPos = static_cast<QWindow*>(receiver)->mapFromGlobal(globalPos);
    }

    QMouseEvent* mouseEvent = new QMouseEvent(mevtype, localPos, globalPos,
                                              mevtype == QEvent::MouseMove ? Qt::NoButton : Qt::LeftButton,
                                              mevtype == QEvent::MouseButtonRelease ? Qt::NoButton : Qt::LeftButton, modifiers);

    if (mevtype == QEvent::MouseButtonRelease)
    {
        // Set low priority so the release event is processed last
        postEvent(receiver, mouseEvent, Qt::LowEventPriority);

        // Press->Drag->Release on menu workaround
        QPoint offscreen(-10000,-10000);
        QMouseEvent* ev2 = new QMouseEvent(QEvent::MouseMove, offscreen, offscreen, Qt::NoButton, Qt::NoButton, modifiers);
        postEvent(receiver, ev2, Qt::LowEventPriority);
    }
    else
    {
        postEvent(receiver, mouseEvent);
    }

    return true;
}

QObject* SurfaceApplication::getRecvWindow(QObject *candidate)
{
    if (candidate->isWindowType())
    {
        QWidget* popup = activePopupWidget();
        if (!popup)
        {
            popup = activeModalWidget();
        }
        if (popup)
        {
            QWindow* w = popup->windowHandle();
            if (w)
            {
                return w;
            }
        }
    }
    return candidate;
}

bool SurfaceApplication::notify(QObject *receiver, QEvent * event)
{
    QEvent::Type evtype = event->type();

    // Only handle touch events we create
    if (evtype == QEvent::TouchBegin ||
            evtype == QEvent::TouchUpdate ||
            evtype == QEvent::TouchEnd)
    {
        QTouchEvent *te = static_cast<QTouchEvent*>(event);
        if (te->device()->name()!= QString("WM_POINTER"))
            return true;
    }

    // Try to pass TabletPress/TouchBegin event and see if a widget accepts
    if ((evtype == QEvent::TabletPress || evtype == QEvent::TouchBegin) && inputState == None)
    {
        if (receiver->isWindowType())
        {
            int prevacceptcount = acceptCount;
            receiver = getRecvWindow(receiver);
            QApplication::notify(receiver, event);
            if (acceptCount > prevacceptcount)
            {
                acceptCount = prevacceptcount;
                inputState = PassThrough;
                return true;
            }

            // If fails, fall through and send mouse event
            // Send Tablet release for consistency
            if (evtype == QEvent::TabletPress)
            {
                QTabletEvent* tev = static_cast<QTabletEvent*>(event);
                QTabletEvent rlev(QEvent::TabletRelease, tev->posF(), tev->globalPosF(), tev->device(),
                                         tev->pointerType(), 0, 0, 0, 0, 0, 0, tev->modifiers(), tev->uniqueId());
                QApplication::notify(receiver, &rlev);
            }
        }
        else
        {
            event->setAccepted(false);
            bool res = QApplication::notify(receiver, event);
            if(event->isAccepted())
            {
                acceptCount++;
            }
            return res;
        }
    }

    // Event Emulation
    switch(evtype)
    {
    // Mouse Events
    case QEvent::MouseButtonRelease:
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
        // Handle Qt oddities
        if(inputState != None && event->spontaneous() && receiver->isWindowType())
        {
            return true;
        }
        break;

    // Tablet Event Emulaation
    case QEvent::TabletRelease:
        if (inputState == PassThrough)
        {
            inputState = None;
        }
    case QEvent::TabletMove:
    case QEvent::TabletPress:
    {
        receiver = getRecvWindow(receiver);
        QTabletEvent* tabletEvent = static_cast<QTabletEvent*>(event);
        QEvent::Type mevtype = QEvent::MouseMove;
        if (inputState == None && evtype == QEvent::TabletPress)
        {
            mevtype = QEvent::MouseButtonPress;
            inputState = TabletInput;
        }
        else if (inputState != TabletInput) // Break if PassThrough
        {
            break;
        }
        if (evtype == QEvent::TabletRelease)
        {
            mevtype = QEvent::MouseButtonRelease;
            inputState = None;
        }
        return sendMouseEvent(receiver, mevtype, tabletEvent->globalPos(), tabletEvent->modifiers());
    }

    // Touch Event Emulation
    case QEvent::TouchCancel:
        evtype = QEvent::TouchEnd;
    case QEvent::TouchEnd:
        if (inputState == PassThrough)
        {
            inputState = None;
        }
    case QEvent::TouchUpdate:
    case QEvent::TouchBegin:
    {
        receiver = getRecvWindow(receiver);
        QTouchEvent* touchEvent = static_cast<QTouchEvent*>(event);
        QEvent::Type mevtype = QEvent::MouseMove;
        if (inputState == None && evtype == QEvent::TouchBegin
                && touchEvent->touchPoints().size() == 1 && touchEvent->device()->type() != QTouchDevice::TouchPad)
        {
            activeTouchId = touchEvent->touchPoints().first().id();
            mevtype = QEvent::MouseButtonPress;
            inputState = TouchInput;
        }
        else if (inputState != TouchInput) // Break if PassThrough
        {
            break;
        }
        if (evtype == QEvent::TouchEnd)
        {
            inputState = None;
        }
        event->setAccepted(true);
        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
        for(int i = 0; i < touchPoints.count(); ++i)
        {
            const QTouchEvent::TouchPoint& touchpt = touchPoints.at(i);
            if(touchpt.id() == activeTouchId) {
                if(touchpt.state() == Qt::TouchPointReleased)
                {
                  mevtype = QEvent::MouseButtonRelease;
                  activeTouchId = -1;
                }
                return sendMouseEvent(receiver, mevtype, touchpt.screenPos().toPoint(), touchEvent->modifiers());
            }
        }
        return true;
    }
    default:
        break;
    }

    return QApplication::notify(receiver, event);
}
