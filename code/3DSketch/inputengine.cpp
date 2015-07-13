#include "inputengine.h"
#include "surfaceapplication.h"

// NON QT Variables
WNDPROC OldWinProc = NULL;
qreal HimetricToPix;
#define MAX_PEN_POINTERS 10
#define MAX_TOUCH_POINTS 10
static POINTER_INFO pointerInfo[MAX_TOUCH_POINTS];
static POINTER_PEN_INFO penPointerInfo[MAX_PEN_POINTERS];


// NON QT FUNCTIONS
int overrideWindowProcedure(HWND & hWnd)
{
    OldWinProc = (WNDPROC)GetWindowLong(hWnd, GWL_WNDPROC);

    qDebug() << "************************************";
    qDebug() << "Displaying System Touch Capabilities";
    qDebug() << "************************************";

    bool test = WINVER >= 0x0603;
    qDebug() << test;

    int value = GetSystemMetrics(SM_DIGITIZER);

    if (value == 0)
    {
        qDebug() << "None";
    }
    if (value & NID_INTEGRATED_TOUCH)
    {
        qDebug() << "Integrated Touch";
    }
    if (value & NID_INTEGRATED_PEN)
    {
        qDebug() << "Integrated Pen";
    }

    if(!OldWinProc)
        return (int) OldWinProc;

    return SetWindowLong(hWnd, GWL_WNDPROC, (LONG) (WNDPROC) NewWndProc);
}

LRESULT CALLBACK NewWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    HDC hdc;

    // HANDLE WINDOWS EVENTS HERE
    // TODO: If touch or pen input, handle. Otherwise, pass on.
    // Not completely implemented since mostly working on QT version
    // Implemented simple example for POINTERUPDATE, others are similar.
    // Can check QT code for further details

    switch (uMsg)
    {
        case WM_POINTERDOWN:
        break;
        case WM_POINTERUPDATE:
            // Get Pointer Data
            POINTER_INFO pointerInfo;
            if (GetPointerInfo(GET_POINTERID_WPARAM(wParam), &pointerInfo))
            {
                switch (pointerInfo.pointerType)
                {
                    case PT_PEN:
                        // ADD PEN CODE HERE
                        qDebug() << "Using Pen";
                        POINTER_PEN_INFO* ppi;
                        GetPointerPenInfo(GET_POINTERID_WPARAM(wParam), ppi);
                        handlePenInput(ppi[0], QTabletEvent::TabletMove);
                    break;
                    case PT_TOUCH:
                        // ADD TOUCH CODE HERE
                        qDebug() << "Using Touch";
                        handleTouchInput(GET_POINTERID_WPARAM(wParam), Qt::TouchPointMoved);
                    break;
                    case PT_MOUSE:
                        qDebug() << "Using Mouse";
                        //return 0;
                    break;
                }

            }
        break;
        case WM_POINTERUP:
        break;

    }

    return CallWindowProc(OldWinProc, hwnd, uMsg, wParam, lParam);
}


// Main Functions For Event Hanlding. QT Specific Implementation.

// Handle a single Pen Input
static void handlePenInput(POINTER_PEN_INFO &ppi, QEvent::Type eventtype)
{
    QTabletEvent::PointerType ptrtype = (ppi.penFlags & PEN_FLAG_ERASER) ? QTabletEvent::Eraser : QTabletEvent::Pen;

    // Attempt at Dynamic Himetric to Pixel Mapping
    // Totally a hack. Should probably force calibration somehow for this to be accurate

    const POINT pix = ppi.pointerInfo.ptPixelLocation;
    const POINT him = ppi.pointerInfo.ptHimetricLocation;

    qreal x = him.x * HimetricToPix;
    qreal y = him.y * HimetricToPix;

    // Correction
    if (pix.x < x - 1 || pix.x > x+ 1 || pix.y < y - 1 || pix.y > y + 1)
    {
        HimetricToPix = qreal(pix.x)/him.x;
        x = him.x * HimetricToPix;
        y = him.y * HimetricToPix;
    }

    // QT Specific Function
    InputFilter::instace()->notifyTabletEvent(eventtype, QPointF(x,y), ppi.pressure/1024.0, ptrtype,
                                              ppi.penFlags & PEN_FLAG_BARREL, int(ppi.pointerInfo.sourceDevice));
}

// Handle a set of pen inputs
static void handlePenHistory(UINT32 ptrid)
{
    UINT32 count = MAX_PEN_POINTERS;
    POINTER_PEN_INFO* ppi = &penPointerInfo[0];
    if (GetPointerPenInfoHistory(ptrid, &count, ppi))
    {
        // Too much information backed up, need to allocate more memory
        if (count > MAX_PEN_POINTERS)
        {
            ppi = new POINTER_PEN_INFO[count];
            GetPointerPenInfoHistory(ptrid, &count, ppi);
        }

        // Process events oldest to newest
        for (int i = count - 1; i >=0; i--)
        {
            handlePenInput(ppi[i], QEvent::TabletMove);
        }

        // Delete memory if we allocated new memory
        if (ppi != &penPointerInfo[0])
        {
            delete[] ppi;
        }
    }
}

// Handle Multi-Touch Input
static bool handleTouchInput(UINT32 ptrid, Qt::TouchPointState eventtype)
{
    UINT32 count = MAX_TOUCH_POINTS;
    if (GetPointerFrameInfo(ptrid, &count, &pointerInfo[0]))
    {
        // Set up QT touch frame data structure
        QList<QTouchEvent::TouchPoint> pts;
        for (unsigned int i = 0; i < count; i++)
        {
            // Only handle touch events (sanity check)
            if (pointerInfo[i].pointerType != PT_TOUCH)
                continue;
            QTouchEvent::TouchPoint pt;
            pt.setId(pointerInfo[i].pointerId);
            pt.setState(pointerInfo[i].pointerId == ptrid ? eventtype : Qt::TouchPointMoved);
            pt.setScreenPos(QPointF(pointerInfo[i].ptPixelLocation.x, pointerInfo[i].ptPixelLocation.y));
            pt.setPressure(1);
            pts.append(pt);
        }
        if (pts.empty())
        {
            return false;
        }

        // Send the touch data to QT
        InputFilter::instace()->notifyTouchEvent(eventtype, pts);
    }
    return false;
}


/////////////////////////////////
/// QT SPECIFIC IMPLEMENTATION
/////////////////////////////////
static bool winInputEvent(MSG* m, long* result)
{
    static UINT32 penPointerId = NULL;

    switch (m->message)
    {
    case WM_POINTERDOWN:
        if (GetPointerInfo(GET_POINTERID_WPARAM(m->wParam), &pointerInfo[0]))
        {
            if (pointerInfo[0].pointerType == PT_PEN)
            {
                penPointerId = pointerInfo[0].pointerId;
                if (GetPointerPenInfo(penPointerId, &penPointerInfo[0]))
                {
                    handlePenInput(penPointerInfo[0], QEvent::TabletPress);
                }
                return true;
            }
            else
            {
                return handleTouchInput(GET_POINTERID_WPARAM(m->wParam), Qt::TouchPointPressed);
            }
        }
        break;
    case WM_POINTERUPDATE:
        if(penPointerId && penPointerId == GET_POINTERID_WPARAM(m->wParam))
        {
            handlePenHistory(penPointerId);
            return true;
        }
        else
        {
            return handleTouchInput(GET_POINTERID_WPARAM(m->wParam), Qt::TouchPointMoved);
        }
        break;
    case WM_POINTERUP:
        if (penPointerId && penPointerId == GET_POINTERID_WPARAM(m->wParam))
        {
            if (GetPointerPenInfo(penPointerId, &penPointerInfo[0]))
            {
                handlePenInput(penPointerInfo[0], QEvent::TabletRelease);
            }
            penPointerId = NULL;
            return true;
        }
        else
        {
            return handleTouchInput(GET_POINTERID_WPARAM(m->wParam), Qt::TouchPointReleased);
        }
    default:
        break;
    }
    return false;
}

bool InputFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    return winInputEvent((MSG*)message, result);
}

InputFilter* InputFilter::mInstance = NULL;

InputFilter::InputFilter()
    : tabletTarget(NULL),
      touchTarget(NULL)
{
    surfaceApp = static_cast<SurfaceApplication*>(QApplication::instance());
    mInstance = this;
    helperObject = new SurfaceHelperObject;

    touchDevice.setName("WM_POINTER");
    touchDevice.setType(QTouchDevice::TouchScreen);
    touchDevice.setCapabilities(QTouchDevice::Position | QTouchDevice::Pressure);
}

InputFilter::~InputFilter()
{
    delete helperObject;
}

void InputFilter::notifyTabletEvent(QEvent::Type eventtype, const QPointF &globalpos, qreal pressure, QTabletEvent::PointerType ptrtype, int buttons, int deviceid)
{

}

void InputFilter::notifyTouchEvent(Qt::TouchPointStates touchstate, const QList<QTouchEvent::TouchPoint> &_points)
{

}

void SurfaceHelperObject::tabletWindowDestroyed()
{
    InputFilter::instace()->tabletTarget = NULL;
}

void SurfaceHelperObject::touchWindowDestroyed()
{
    InputFilter::instace()->touchTarget = NULL;
}
