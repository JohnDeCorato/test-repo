#include "touchinputfilter.h"
#include "touchapplication.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QWindow>
#include <QScreen>
#include <windows.h>
#include <QDebug>

#ifdef Q_OS_WIN

static qreal HimetricToPix;
#define MAX_N_POINTERS 10
static POINTER_INFO pointerInfo[MAX_N_POINTERS];
static POINTER_PEN_INFO penPointerInfo[MAX_N_POINTERS];

static void processPenInfo(const POINTER_PEN_INFO& ppi, QEvent::Type eventtype)
{
  QTabletEvent::PointerType ptrtype
      = (ppi.penFlags & PEN_FLAG_ERASER) ? QTabletEvent::Eraser : QTabletEvent::Pen;

  // unfortunately, there doesn't seem to be any reliable way to figure out himetric to pixel mapping,
  //  so just calculate it from first point we see
  const POINT pix = ppi.pointerInfo.ptPixelLocation;
  const POINT him = ppi.pointerInfo.ptHimetricLocation;
  qreal x = him.x*HimetricToPix;
  qreal y = him.y*HimetricToPix;
  if(pix.x < x - 1 || pix.x > x + 1 || pix.y < y - 1 || pix.y > y + 1) {
    HimetricToPix = qreal(pix.x)/him.x;
    x = him.x*HimetricToPix;
    y = him.y*HimetricToPix;
  }
  // Confirmed that HIMETRIC is higher resolution than pixel location on Surface Pro: saw different HIMETRIC
  //  locations for the same pixel loc, including updates to HIMETRIC loc with no change in pixel loc
  //qDebug("Pix: %d %d; HIMETRIC: %d %d", pix.x, pix.y, him.x, him.y);

  TouchInputFilter::instance()->notifyTabletEvent(eventtype, QPointF(x, y), ppi.pressure/1024.0, ptrtype,
      ppi.penFlags & PEN_FLAG_BARREL, int(ppi.pointerInfo.sourceDevice));
}

// ideally, we wouldn't process history unless mode is STROKE
static void processPenHistory(UINT32 ptrid)
{
  UINT32 historycount = MAX_N_POINTERS;
  POINTER_PEN_INFO* ppi = &penPointerInfo[0];
  if(GetPointerPenInfoHistory(ptrid, &historycount, ppi)) {
    if(historycount > MAX_N_POINTERS) {
      // need more room ... we want to get all history at once since it's returned newest first!
      ppi = new POINTER_PEN_INFO[historycount];
      GetPointerPenInfoHistory(ptrid, &historycount, ppi);
    }
    // process items oldest to newest
    for(int ii = historycount - 1; ii >= 0; ii--)
      processPenInfo(ppi[ii], QEvent::TabletMove);
    if(ppi != &penPointerInfo[0])
      delete[] ppi;
  }
}

static bool processPointerFrame(UINT32 ptrid, Qt::TouchPointState eventtype)
{
  UINT32 pointercount = MAX_N_POINTERS;
  if(GetPointerFrameInfo(ptrid, &pointercount, &pointerInfo[0])) {
    QList<QTouchEvent::TouchPoint> pts;
    for(unsigned int ii = 0; ii < pointercount; ii++) {
      if(pointerInfo[ii].pointerType != PT_TOUCH)
        continue;
      QTouchEvent::TouchPoint pt;
      pt.setId(pointerInfo[ii].pointerId);
      pt.setState(pointerInfo[ii].pointerId == ptrid ? eventtype : Qt::TouchPointMoved);
      pt.setScreenPos(QPointF(pointerInfo[ii].ptPixelLocation.x, pointerInfo[ii].ptPixelLocation.y));
      pt.setPressure(1);
      pts.append(pt);
    }
    if(pts.empty())
      return false;
    //event.t = pointerInfo[0].performanceCount;
    TouchInputFilter::instance()->notifyTouchEvent(eventtype, pts);
    return true;
  }
  return false;
}

static void initWMPointer()
{

  // Attempt to get HIMETRIC to pixel conversion factor; on Surface Pro, result is close, but not quite
  // 1 HIMETRIC = 0.01 mm
  QWidget* screen = QApplication::desktop()->screen(0);
  // this is equiv to GetDeviceCaps(HORZRES)/GetDeviceCaps(HORZSIZE)
  HimetricToPix = screen->width()/qreal(100*screen->widthMM());
}

static bool winInputEvent(MSG* m, long* result)
{
  static UINT32 penPointerId = NULL;
  //static bool scribbling = false;
  // winTabEvent(m, result);
  if(!GetPointerInfo)
    return false;
  switch(m->message) {
  // WM_POINTER:
  // WM_POINTERDOWN with type PT_PEN: ignore all other pointers, use GetPointerPenInfoHistory
  // otherwise, use GetPointerFrameInfo (discard history)
  case WM_POINTERDOWN:
    if(GetPointerInfo(GET_POINTERID_WPARAM(m->wParam), &pointerInfo[0])) {
      if(pointerInfo[0].pointerType == PT_PEN) {
        penPointerId = pointerInfo[0].pointerId;
        if(GetPointerPenInfo(penPointerId, &penPointerInfo[0]))
          processPenInfo(penPointerInfo[0], QEvent::TabletPress);
        return true;
      }
      else
        return processPointerFrame(GET_POINTERID_WPARAM(m->wParam), Qt::TouchPointPressed);
    }
    break;
  case WM_POINTERUPDATE:
    if(penPointerId && penPointerId == GET_POINTERID_WPARAM(m->wParam)) {
      processPenHistory(penPointerId);
      return true;
    }
    else
      return processPointerFrame(GET_POINTERID_WPARAM(m->wParam), Qt::TouchPointMoved);
    break;
  case WM_POINTERUP:
    if(penPointerId && penPointerId == GET_POINTERID_WPARAM(m->wParam)) {
      if(GetPointerPenInfo(penPointerId, &penPointerInfo[0]))
        processPenInfo(penPointerInfo[0], QEvent::TabletRelease);
      penPointerId = NULL;
      return true;
    }
    else
      return processPointerFrame(GET_POINTERID_WPARAM(m->wParam), Qt::TouchPointReleased);
  default:
    break;
  }
  return false;
}

void initInput()
{
  initWMPointer();
  // Qt 4 supports tablet input
#ifdef USE_WINTAB
  hTab = NULL;
  //initWinTab();
  LoadWintab();
#endif
}

WinInputFilter::WinInputFilter()
{
  initInput();
}

bool WinInputFilter::nativeEventFilter(const QByteArray& eventType, void* message, long* result)
{
  return
#ifdef USE_WINTAB
    winTabEvent((MSG*)message,  result) ||
#endif
    winInputEvent((MSG*)message,  result);
}

#endif // Q_OS_WIN


TouchInputFilter* TouchInputFilter::m_instance = NULL;

TouchInputFilter::TouchInputFilter() : tabletTarget(NULL), touchTarget(NULL)
{
  touchApp = static_cast<TouchApplication*>(QApplication::instance());
  m_instance = this;
  helperObject = new TouchHelperObject;
  // using a QTouchEvent with NULL touch device results in crash
  touchDevice.setName("WM_POINTER");
  touchDevice.setType(QTouchDevice::TouchScreen);
  touchDevice.setCapabilities(QTouchDevice::Position | QTouchDevice::Pressure);
}

TouchInputFilter::~TouchInputFilter()
{
  delete helperObject;
}

// functions for direct injection of tablet and touch events (only used on Windows at the moment)

void TouchInputFilter::notifyTabletEvent(QEvent::Type eventtype,
    const QPointF& globalpos, qreal pressure, QTabletEvent::PointerType ptrtype, int buttons, int deviceid)
{
  if(eventtype == QEvent::TabletPress || !tabletTarget) {
    tabletTarget = QGuiApplication::topLevelAt(globalpos.toPoint());
    if(!tabletTarget)
      return;
    QObject::connect(tabletTarget, SIGNAL(destroyed()), helperObject, SLOT(tabletWindowDestroyed()));
  }
  QWindow* window = tabletTarget;
  if(eventtype == QEvent::TabletRelease) {
    QObject::disconnect(tabletTarget, SIGNAL(destroyed()), helperObject, SLOT(tabletWindowDestroyed()));
    tabletTarget = NULL;
  }

  QPointF localpos = window->mapFromGlobal(globalpos.toPoint()) + (globalpos - globalpos.toPoint());
  QTabletEvent tabletevent(eventtype, localpos, globalpos, deviceid , ptrtype,
                           pressure, 0, 0, 0, 0, 0, QApplication::keyboardModifiers(), deviceid);
  touchApp->setTabletButtons(buttons);
  touchApp->notify(window, &tabletevent);
}

void TouchInputFilter::notifyTouchEvent(
    Qt::TouchPointStates touchstate, const QList<QTouchEvent::TouchPoint>& _points)
{
  QList<QTouchEvent::TouchPoint> points = _points;
  QEvent::Type evtype = QEvent::TouchUpdate;
  if(touchstate == Qt::TouchPointPressed && !touchTarget) {
    touchTarget = QGuiApplication::topLevelAt(points[0].screenPos().toPoint());
    if(touchTarget)
      QObject::connect(touchTarget, SIGNAL(destroyed()), helperObject, SLOT(touchWindowDestroyed()));
    evtype = QEvent::TouchBegin;
  }
  if(!touchTarget)
    return;
  QWindow* window = touchTarget;
  if(touchstate == Qt::TouchPointReleased && points.count() == 1) {
    QObject::disconnect(touchTarget, SIGNAL(destroyed()), helperObject, SLOT(touchWindowDestroyed()));
    touchTarget = NULL;
    evtype = QEvent::TouchEnd;
  }
  if(points.count() > 1)
    touchstate |= Qt::TouchPointMoved;

  for(int ii = 0; ii < points.count(); ++ii) {
    points[ii].setPos(window->mapFromGlobal(points[ii].screenPos().toPoint()));
    // TODO: handle last, start position stuff by saving previous list of touch points
  }

  QTouchEvent touchevent(evtype, &touchDevice, QApplication::keyboardModifiers(), touchstate, points);
  touchApp->notify(window, &touchevent);
}

void TouchHelperObject::tabletWindowDestroyed()
{
  TouchInputFilter::instance()->tabletTarget = NULL;
}

void TouchHelperObject::touchWindowDestroyed()
{
  TouchInputFilter::instance()->touchTarget = NULL;
}


// see http://code.msdn.microsoft.com/windowsdesktop/Touch-Injection-Sample-444d9bf7/
/* #ifdef SCRIBBLE_TEST
bool ScribbleInput::injectTouch(Dim x, Dim y, Dim p, int event)
{
  static bool touchInjectionInited = false;
  static POINTER_TOUCH_INFO contact;
  if(!GetPointerInfo)
    return false;

  if(!touchInjectionInited) {
    InitializeTouchInjection(10, TOUCH_FEEDBACK_NONE);
    memset(&contact, 0, sizeof(POINTER_TOUCH_INFO));
    contact.pointerInfo.pointerType = PT_TOUCH; // we're sending touch input
    contact.pointerInfo.pointerId = 0;          // contact 0
    contact.touchFlags = TOUCH_FLAG_NONE;
    contact.touchMask = TOUCH_MASK_CONTACTAREA | TOUCH_MASK_ORIENTATION | TOUCH_MASK_PRESSURE;
    contact.orientation = 90;
    // set the contact area depending on thickness
    //contact.rcContact.top = 480 - 2;
    //contact.rcContact.bottom = 480 + 2;
    //contact.rcContact.left = 640 - 2;
    //contact.rcContact.right = 640 + 2;
    touchInjectionInited = true;
  }

  contact.pointerInfo.ptPixelLocation.x = x;
  contact.pointerInfo.ptPixelLocation.y = y;
  //contact.pointerInfo.ptHimetricLocation.x = x/HimetricToPix;
  //contact.pointerInfo.ptHimetricLocation.y = y/HimetricToPix;
  contact.pressure = p * 1024;
  if(event == INPUTEVENT_PRESS)
    contact.pointerInfo.pointerFlags = POINTER_FLAG_DOWN | POINTER_FLAG_INRANGE | POINTER_FLAG_INCONTACT;
  else if(event == INPUTEVENT_RELEASE)
    contact.pointerInfo.pointerFlags = POINTER_FLAG_UP;
  else
    contact.pointerInfo.pointerFlags = POINTER_FLAG_UPDATE | POINTER_FLAG_INRANGE | POINTER_FLAG_INCONTACT;
  if(InjectTouchInput(1, &contact) == 0)
    return false;
  // seems to be necessary for us to receive the touch event
  QApplication::processEvents();
  return true;
}
#endif */
