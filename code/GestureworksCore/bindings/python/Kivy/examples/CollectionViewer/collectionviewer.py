'''
////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:    collectionviewer.py
//  Authors:  Ideum
//
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////
'''


from gwc_python.core import GestureWorksCore
from gwc_python.GWCUtils import TOUCHADDED, TOUCHREMOVED, TOUCHUPDATE, rotateAboutCenter

import kivy
kivy.require('1.2.0')
 
from kivy.app import App
from kivy.uix.widget import NumericProperty, StringProperty
from kivy.uix.image import Image
from kivy.uix.scatter import Scatter
from kivy.uix.video import Video
from kivy.factory import Factory
from kivy.graphics.transformation import Matrix
from kivy.clock import Clock

from math import radians
from os.path import join
from os import getcwd
from random import randint


"""This will be the container for all of our touch objects"""
class TouchObject(Scatter):
    source = StringProperty(None)
    frame_pad = NumericProperty(None)
    
    # override kivy touch handling
    def on_touch_move(self, touch): return
    
class CollectionApp(App):
    
    """Initialize data.
    
    Takes a GestureWorksCore object
    
    """
    def __init__(self, gw):
        self.gw = gw
        self.objects = {}
        self.active_touches = {}
        self.last_active = []
        self.touch_points = {}

        Clock.schedule_interval(self.updateGW, 1./60.)
        super(CollectionApp, self).__init__()
        
    
    """Redraw a widget as the topmost element of its parent"""
    def _draw_on_top(self, widg, parent):
        parent.remove_widget(widg)
        parent.add_widget(widg)
    
    
    """Check if a point given by x and y has collided with an object
    
    Returns True if there was a collision
    
    """
    def hitTest(self, obj, x, y):
        
        y = self.root.height - y
        (local_x, local_y) = rotateAboutCenter(x, y, obj.center_x, obj.center_y, radians(-obj.rotation))

        if obj.name == 'clock':
            local_y += 60 * obj.scale # offset because buttons hang off the video widget

        return (abs(local_x - obj.center_x) <= obj.width*obj.scale/2) and (abs(local_y - obj.center_y) <= obj.height*obj.scale/2)   
    
    
    """Process each touch event from GestureWorks and check if new points should be added
    to touch objects.
    
    """    
    def processTouchEvents(self, touches):
        for touch in touches:
            if touch.status == TOUCHADDED:
                self.touch_points.update({touch.point_id: touch})
                hits = []
                for name in self.objects.keys():
                    obj = self.objects[name]['obj']
                    if not isinstance(obj, Scatter): continue 
                    if self.hitTest(obj, touch.position.x, touch.position.y):
                        hits.append(name)
                        
                # If both objects were hit, decide which one is on top
                if len(hits) > 1:
                    for hit in reversed(hits):
                        if self.objects[hit]['obj'].on_top:
                            self.gw.addTouchPoint(hit, touch.point_id)
                            
                elif len(hits) == 1:
                    for o in self.objects.values(): o['obj'].on_top = False
                    self.objects[hits[0]]['obj'].on_top = True
                    self._draw_on_top(self.objects[hits[0]]['obj'], self.root)
                    self.gw.addTouchPoint(hits[0], touch.point_id)       
                                
            elif touch.status == TOUCHUPDATE:
                self.touch_points.update({touch.point_id: touch})
            elif touch.status == TOUCHREMOVED:
                self.touch_points.pop(touch.point_id)
             
            
    """Translate the object based on the event deltas"""
    def handleDrag(self, obj, e): 
        obj.center_x += e.values['drag_dx']
        obj.center_y -= e.values['drag_dy']
        
        # Check window boundaries
        obj.center_x = max(min(obj.center_x, self.root.width), 0)
        obj.center_y = max(min(obj.center_y, self.root.height), 0)     
    
    
    """Rotate the object around the center of the event"""
    def handleRotate(self, obj, gesture_event):
        theta = gesture_event.values['rotate_dtheta']
        obj.rotation -= theta
        if gesture_event.n:
            obj.center = rotateAboutCenter(obj.center_x, obj.center_y, gesture_event.x, self.root.height - gesture_event.y, radians(-theta))
    
    """Scale the object"""
    def handleScale(self, obj, e):
        dsx = e.values['scale_dsx']
        obj.scale += dsx
        obj.scale = max(min(obj.scale, 2), .5) # Scale bounds
                
    """Check for and process button taps"""
    def handleTap(self, obj, e):
        x = e.values['tap_x']
        y = self.root.height - e.values['tap_y']
        local_x, local_y = obj.to_widget(x, y)
        for child in obj.children:
            try:
                child.action # only care about objects with action functions
            except AttributeError:
                continue
            
            w = child.width * obj.scale
            h = child.height * obj.scale
            if abs(local_x - child.center_x) <= w/2 and abs(local_y - child.center_y) <= h/2:
                child.action(e)
                return
            
                
    """Get point and gesture event data from GestureWorks"""
    def updateGW(self, *args):
        self.gw.processFrame() # tell GW to process a frame of data
        point_events = gw.consumePointEvents() # get the point events from that frame
        gesture_events = gw.consumeGestureEvents() # get the gesture events from that frame
        self.processTouchEvents(point_events)

        # Call event handlers
        for event in gesture_events:
            obj = self.objects[event.target]['obj']
            {'n-drag': self.handleDrag,
             'n-rotate': self.handleRotate,
             'n-scale': self.handleScale,
             'n-tap': self.handleTap}[event.gesture_id](obj, event)

    
    """Remove a widget when its "close" button is tapped."""
    def handleCloseWidget(self, evt):
        obj = self.objects[evt.target]['obj']
        self.root.remove_widget(obj)
     
    
    """Switch a container's image with its alternate and vice versa."""
    def handleMetaSwitch(self, evt):
        obj = self.objects[evt.target]['obj']

        container = obj
        alt = container.alt
        pic = container.main
        
        # shuffle images to appear to "switch"
        self._draw_on_top(alt, container)
        self._draw_on_top(container.alt_info, container)
        container.remove_widget(container.info)
        
        info = container.info
        container.info = container.alt_info
        container.alt_info = info
        
        container.alt = pic
        container.remove_widget(pic)
        container.main = alt

    
    """Toggle a video between play and pause modes."""
    def handlePause(self, evt):  
        obj = self.objects[evt.target]['obj']
        
        if not isinstance(obj.main, Video): return # Disable control if meta image is displayed
        obj.main.play = not obj.main.play
        playing = obj.main.play
        parent = obj
        play = parent.play
        pause = parent.pause
        if playing:
            parent.remove_widget(play)
            parent.add_widget(pause)
        else:
            parent.remove_widget(pause)
            parent.add_widget(play)

       
    """Stop a video by setting its current position to the beginning and pausing it.""" 
    def handleStop(self, evt):
        obj = self.objects[evt.target]['obj']
        video = obj.main
        if not isinstance(video, Video): return # Disable control if meta image is displayed
        video.play = False
        video.seek(0)
        obj.remove_widget(obj.pause)
        obj.add_widget(obj.play)
    
    
    
    """Add a button to a parent widget"""
    def _addButton(self, parent, name, source, size, pos, action_func, on_top=False):
        button = Image(source=source)
        button.size = size
        button.on_top = on_top
        button.pos = pos
        button.scale = 1.
        button.rotation = 0.
        button.action = action_func
        button.name = name
        self.objects.update({name: {'obj': button}})
        parent.add_widget(button)
        
        return button
    
    
    """Create a container widget extending the Scatter class that will respond to touch events and display
    an image or video. Will automatically add container and buttons to internal structures and register
    callbacks for event listeners.
    
    """
    def _create_collection_item(self, source, alt_source, size, position, frame_size, name, is_video=False):
        container = TouchObject(frame_pad=frame_size)
        
        container.main = {True: Video, False: Image}[is_video]
        container.on_top = is_video
        container.main = container.main(source=source)
        container.pos = position
    
        container.main.size = size[0], size[1] / container.main.image_ratio
        container.size = container.main.size
        
        container.alt = Image(source=alt_source)
        container.alt.size = container.main.size

        container.add_widget(container.main)

        button_y = [0, -60][is_video]
        
        info_pos = (container.main.width-100, button_y)
    
        container.name = name
        container.alt_info = self._addButton(container, name + '_alt_info_button', 'media/images/GW_return.png', (40,70), info_pos, self.handleMetaSwitch)
        container.remove_widget(container.alt_info)
        container.info = self._addButton(container, name + '_info_button', 'media/images/GW_info.png', (40,70), info_pos, self.handleMetaSwitch, True)
        container.close = self._addButton(container, name + '_close_button', 'media/images/GW_close.png', (40,70), (container.main.width-55, button_y), self.handleCloseWidget, True)
        self.objects.update({name: {'obj': container}})
           
        if is_video:
            container.pause = self._addButton(container, name + '_pause_button', 'media/images/GW_pause.png', (70,70), (-10, button_y), self.handlePause)
            container.play = self._addButton(container, name + '_play_button', 'media/images/GW_play.png', (70,70), (-10, button_y), self.handlePause)
            container.remove_widget(container.play)
            container.stop = self._addButton(container, name + '_stop_button', 'media/images/GW_stop.png', (70,70), (35, button_y), self.handleStop)       
        
        return container
        

    """Setup and register the touch objects"""
    def build(self):
        if not self.gw.registerWindow('Kivy'): exit()
        
        curdir = getcwd()
        image_file = join(curdir, 'media/images/pics/boccioni_600.png')
        alt_image_file = join(curdir, 'media/images/pics/meta/boccioni_600.png')
        image_name = 'image'
        img = self._create_collection_item(image_file, alt_image_file, (400, 400), (400, 400), 0, image_name, False)
        img.rotation = randint(-45, 45)
        self.root.add_widget(img)
        self.gw.registerTouchObject(image_name)
        self.gw.addGesture(image_name, 'n-drag')
        self.gw.addGesture(image_name, 'n-rotate')
        self.gw.addGesture(image_name, 'n-scale')
        self.gw.addGesture(image_name, 'n-tap')
        
        video_file = join(curdir, 'media/videos/clock.mp4')
        alt_video_file = join(curdir, 'media/videos/meta/clock.png')
        video_name = 'clock'
        vid = self._create_collection_item(video_file, alt_video_file, (400,225), (400,400), 0, video_name, True)
        self.root.add_widget(vid)
        vid.main.play = True
        self.gw.registerTouchObject(video_name)
        self.gw.addGesture(video_name, 'n-drag')
        self.gw.addGesture(video_name, 'n-rotate')
        self.gw.addGesture(video_name, 'n-scale')
        self.gw.addGesture(video_name, 'n-tap')
        
Factory.register('TouchObject', TouchObject)
    
if __name__ == '__main__':
    bin_path = "C:\\gwdistro\\dev\\GestureworksCore\\"
    gw = GestureWorksCore(dll_path=bin_path + 'GestureworksCore32.dll')
    gw.initializeGestureWorks()
    try: 
        gw.loadGML(bin_path + 'basic_manipulation.gml')
    except WindowsError, e:
        print 'Unable to load GML from', bin_path 
    
    collection = CollectionApp(gw)
    
    collection.run()
    