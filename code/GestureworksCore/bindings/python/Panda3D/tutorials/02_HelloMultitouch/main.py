from gwc_python.core import GestureWorksCore
from gwc_python.GWCUtils import TOUCHREMOVED, TOUCHADDED

from direct.gui.OnscreenImage import OnscreenImage
from direct.gui.OnscreenText import OnscreenText
from direct.showbase.ShowBase import ShowBase
from direct.task import Task
from pandac.PandaModules import LineSegs 
from pandac.PandaModules import deg2Rad
from pandac.PandaModules import NodePath
from pandac.PandaModules import Vec3
from panda3d.core import TransparencyAttrib
from panda3d.core import TextNode

SCREEN_WIDTH = 1920
SCREEN_HEIGHT = 1080

class MultitouchApp(ShowBase):
    
    def __init__(self, gw):
        self.gw = gw
        self.active_points = {}
        self.touch_images = {}
        self.touch_text = {}
        
        ShowBase.__init__(self)
        self.build()
        self.taskMgr.add(self.updateGestureworks, "updateGestureworksTask")
        
    def processTouchEvents(self, touches):
        for touch in touches:
            if touch.status != TOUCHREMOVED:
                self.active_points.update({touch.point_id: touch})
            else:
                self.active_points.pop(touch.point_id)
    
    def clearScreen(self):
        for single_image in self.touch_images:
            self.touch_images[single_image].destroy()
        for single_text in self.touch_text:
            self.touch_text[single_text].destroy()              
 
    def drawTouchPoints(self):
        self.win
        for touch in self.active_points.values():
            
            # We need to convert Gestureworks coordinates to Panda3D coordinates
            touch_x = float((touch.position.x - SCREEN_WIDTH/2) / SCREEN_WIDTH) * 4
            touch_y = float((SCREEN_HEIGHT/2 - touch.position.y) / SCREEN_HEIGHT) * 2

            if touch.status != TOUCHREMOVED:
                # Draw circles
                self.touch_images[touch.point_id] = OnscreenImage('ring_black.png', pos=(touch_x,0,touch_y), scale=.05)
                self.touch_images[touch.point_id].setTransparency(TransparencyAttrib.MAlpha)
                
                # Draw the touchpoint info
                label = 'ID: %d\nX: %d | Y: %d' %(touch.point_id,touch.position.x,touch.position.y)
                self.touch_text[touch.point_id] = OnscreenText(label,pos=(touch_x+0.1,touch_y), scale=.05,align=TextNode.ALeft)                 
 
    def updateGestureworks(self, task):            
        self.gw.processFrame()
        point_events = gw.consumePointEvents()
        self.processTouchEvents(point_events)
        self.clearScreen();
        self.drawTouchPoints()
        return Task.cont
    
    def build(self):
        if not self.gw.registerWindow('Panda'):
            print('Unable to register touch window')
            exit()
 
if __name__ == '__main__':
    gw = GestureWorksCore('C:\\GestureworksCore\\GestureworksCore32.dll')
    if not gw.loaded_dll: 
        print 'Unable to load GestureWorksCore'
        exit()
    gw.initializeGestureWorks(SCREEN_WIDTH, SCREEN_HEIGHT)
    app = MultitouchApp(gw)
    app.run()