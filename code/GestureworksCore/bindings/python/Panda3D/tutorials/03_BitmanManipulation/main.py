from gwc_python.core import GestureWorksCore
from gwc_python.GWCUtils import TOUCHADDED, TOUCHREMOVED, TOUCHUPDATE, rotateAboutCenter

from direct.gui.OnscreenImage import OnscreenImage
from direct.showbase.ShowBase import ShowBase
from direct.task import Task
from pandac.PandaModules import LineSegs 
from pandac.PandaModules import deg2Rad
from pandac.PandaModules import NodePath
from pandac.PandaModules import Vec3
from panda3d.core import TransparencyAttrib
from panda3d.core import TextNode

from math import radians

SCREEN_WIDTH = 1920
SCREEN_HEIGHT = 1080

class TouchObject():
    pass

class MultitouchApp(ShowBase):
    
    def __init__(self, gw):
        self.gw = gw
        self.touch_points = {}
        self.touch_objects = {}
        
        ShowBase.__init__(self)
        self.build()
        # schedule our processing cycle
        self.taskMgr.add(self.updateGestureworks, "updateGestureworksTask")
        
    def hitTest(self, x, y):
        for obj in self.touch_objects.values():
            (local_x, local_y) = rotateAboutCenter(x, y, obj.picture.getX(), obj.picture.getZ(), radians(-obj.picture.getR()))
            if (local_x > (obj.picture.getX() - obj.scale) and local_x < (obj.picture.getX() + obj.scale) ):
                if (local_y > (obj.picture.getZ() - obj.scale) and local_y < (obj.picture.getZ() + obj.scale) ):
                    return obj
         
    def processTouchEvents(self, touches):
        for touch in touches:
            # We need to convert Gestureworks coordinates to Panda3D coordinates
            touch_x = float((touch.position.x - SCREEN_WIDTH/2) / SCREEN_WIDTH) * 4
            touch_y = float((SCREEN_HEIGHT/2 - touch.position.y) / SCREEN_HEIGHT) * 2
            
            if touch.status == TOUCHADDED:
                obj = self.hitTest(touch_x, touch_y)
                if obj:
                    self.gw.addTouchPoint(obj.name, touch.point_id)
            elif touch.status == TOUCHREMOVED:
                # Handle touch removed 
                pass
            elif touch.status == TOUCHUPDATE:
                # Handle touch updates
                pass
            
    """Translate the object based on the event deltas"""            
    def handleDrag(self, obj, gesture_event):
        # We need to convert Gestureworks coordinates to Panda3D coordinates
        dx = gesture_event.values['drag_dx'] / SCREEN_WIDTH * 4
        dy = gesture_event.values['drag_dy'] / SCREEN_HEIGHT * 2
        obj.picture.setX(obj.picture.getX() + dx)
        obj.picture.setZ(obj.picture.getZ() - dy)
         
        # Make sure the objects stay on the screen
        obj.picture.setX(max(min(obj.picture.getX(), 1.90), -1.90))
        obj.picture.setZ(max(min(obj.picture.getZ(), 1), -1))
    
    """Rotate the object around the center of the event"""
    def handleRotate(self, obj, gesture_event):
        theta = gesture_event.values['rotate_dtheta']
        obj.picture.setR(obj.picture.getR() + theta)
        if gesture_event.n:
            # We need to convert Gestureworks coordinates to Panda3D coordinates
            gesture_x = float((gesture_event.x - SCREEN_WIDTH/2) / SCREEN_WIDTH) * 4
            gesture_y = float((SCREEN_HEIGHT/2 - gesture_event.y) / SCREEN_HEIGHT) * 2
            (new_x,new_y) = rotateAboutCenter(obj.picture.getX(), obj.picture.getZ(), gesture_x, gesture_y, radians(-theta))
            obj.picture.setX(new_x)
            obj.picture.setZ(new_y)        
    
    """Scale the object"""
    def handleScale(self, obj, gesture_event):
        dsx = gesture_event.values['scale_dsx']/4
        obj.scale += dsx
        obj.scale = max(min(obj.scale, 1.5), .125) 
        obj.picture.setScale(obj.scale)
                
    """Update our touch objects based on gesture events we receive"""
    def processGestureEvents(self, gesture_events):
        for e in gesture_events:
            obj = self.touch_objects[e.target]
            {'n-drag': self.handleDrag,
             'n-rotate': self.handleRotate,
             'n-scale': self.handleScale}[e.gesture_id](obj, e)
    
    """Tell GestureWorks to process a frame of data"""
    def updateGestureworks(self, task):
        self.gw.processFrame()
        point_events = gw.consumePointEvents()
        gesture_events = gw.consumeGestureEvents()
        self.processTouchEvents(point_events)
        self.processGestureEvents(gesture_events)
        
        return Task.cont
        
    def build(self):
        if not self.gw.registerWindow('Panda'):
            print('Unable to register touch window')
            exit()
        
        # Set background image
        OnscreenImage(parent=render2d, image="media/Logo_gestureworks_core_1920x1080_white.png",pos=(0,0,0))
        
        # Create our touch objects
        container_0 = TouchObject()
        container_0.name = 'object_0'
        container_0.picture = OnscreenImage(image='media/logo.png',pos=(-1,0,0),scale=0.25)
        container_0.scale = 0.25
        self.touch_objects.update({container_0.name: container_0})
        
        container_1 = TouchObject()
        container_1.name = 'object_1'
        container_1.picture = OnscreenImage(image='media/logo.png',pos=(1,0,0),scale=0.25)
        container_1.scale = 0.25
        self.touch_objects.update({container_1.name: container_1})
            
        for name in self.touch_objects:
            # Tell GestureWorks about our touch object and add gestures to it
            self.gw.registerTouchObject(name)
            self.gw.addGesture(name, 'n-drag')
            self.gw.addGesture(name, 'n-rotate')
            self.gw.addGesture(name, 'n-scale')
            
if __name__ == '__main__':
    # Initialize GestureWorksCore with the location of the library
    gw = GestureWorksCore('C:\\GestureWorksCore\\GestureworksCore32.dll')
    if not gw.loaded_dll: 
        print 'Unable to load GestureWorksCore'
        exit()
        
    try:
        # Load a basic GML file
        gw.loadGML('C:\\GestureWorksCore\\basic_manipulation.gml')
    except WindowsError, e:
        print 'Unable to load GML'
        exit()  
        
    gw.initializeGestureWorks(SCREEN_WIDTH, SCREEN_HEIGHT)
    app = MultitouchApp(gw)
    app.run()