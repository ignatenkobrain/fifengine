#!/usr/bin/env python

# -*- coding: utf-8 -*-

# ####################################################################
#  Copyright (C) 2005-2010 by the FIFE team
#  http://www.fifengine.net
#  This file is part of FIFE.
#
#  FIFE is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the
#  Free Software Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
# ####################################################################
# This is the rio de hola client for FIFE.

import sys, os, re, math, random, shutil

from fife import fife

from fife.extensions.soundmanager import SoundManager
from fife.extensions.loaders import loadImportFile

from scripts.scene import Scene
from scripts.guicontroller import GUIController
from scripts.actors.baseactor import TalkAction, PickUpItemAction
from scripts.objects.baseobject import GameObjectTypes


class KeyState(object):
	def __init__(self):
		self._keystate = { }
		
	def updateKey(self, key, state):
		self._keystate[key] = state
		
	def checkKey(self, key):
		if key in self._keystate:
			return self._keystate[key]
		else:
			return False
			
	def reset(self):
		self._keystate.clear()

class GameListener(fife.IKeyListener, fife.IMouseListener):
	def __init__(self, gamecontroller):
		self._engine = gamecontroller.engine
		self._gamecontroller = gamecontroller
		self._settings = gamecontroller.settings
		self._eventmanager = self._engine.getEventManager()
		self._soundmanager = SoundManager(self._engine)
		
		fife.IMouseListener.__init__(self)
		fife.IKeyListener.__init__(self)
		
		self._attached = False
		
		self._lastmousepos = (0.0,0.0)
		
	def attach(self):
		if not self._attached:
			self._gamecontroller.keystate.reset()
			self._eventmanager.addMouseListenerFront(self)
			self._eventmanager.addKeyListenerFront(self)
			self._attached = True
	
	def detach(self):
		if self._attached:
			self._eventmanager.removeMouseListener(self)
			self._eventmanager.removeKeyListener(self)
			self._attached = False
		
	def mousePressed(self, event):
		#mouse press was consumed by some pychan widget
		if event.isConsumedByWidgets():
			return

		clickpoint = fife.ScreenPoint(event.getX(), event.getY())

		if (event.getButton() == fife.MouseEvent.LEFT):
			self._lastmousepos = (clickpoint.x, clickpoint.y)
			self._gamecontroller.scene.player.walk( self._gamecontroller.scene.getLocationAt(clickpoint) )
			actor_instances = self._gamecontroller.scene.getInstancesAt(clickpoint, self._gamecontroller.scene.actorlayer)
			item_instances = self._gamecontroller.scene.getInstancesAt(clickpoint, self._gamecontroller.scene.itemlayer)
			if actor_instances:
				obj = self._gamecontroller.scene.objectlist[actor_instances[0].getId()]
				if obj.type == GameObjectTypes["QUESTGIVER"]:
					action = TalkAction(self._gamecontroller.scene.player, obj)
					self._gamecontroller.scene.player.nextaction = action
			
			if item_instances:
				obj = self._gamecontroller.scene.objectlist[item_instances[0].getId()]
				if obj.type == GameObjectTypes["ITEM"]:
					action = PickUpItemAction(self._gamecontroller.scene.player, obj)
					self._gamecontroller.scene.player.nextaction = action
					

		if (event.getButton() == fife.MouseEvent.RIGHT):
			instances = self._gamecontroller.scene.getInstancesAt(clickpoint)
			print "selected instances on actor layer: ", [i.getId() for i in instances]
			if instances:
				#do something
				pass
				
	def mouseReleased(self, event):
		pass

	def mouseMoved(self, event):
		renderer = self._gamecontroller.instancerenderer
		if renderer:
			renderer.removeAllOutlines()
		else:
			return

		pt = fife.ScreenPoint(event.getX(), event.getY())
		actor_instances = self._gamecontroller.scene.getInstancesAt(pt, self._gamecontroller.scene.actorlayer)
		item_instances = self._gamecontroller.scene.getInstancesAt(pt, self._gamecontroller.scene.itemlayer)
		for i in actor_instances:
			renderer.addOutlined(i, 173, 255, 47, 2)
			
		for j in item_instances:
			renderer.addOutlined(j, 173, 255, 47, 2)

	def mouseEntered(self, event):
		pass
		
	def mouseExited(self, event):
		pass
		
	def mouseClicked(self, event):
		pass
		
	def mouseWheelMovedUp(self, event):
		pass	
		
	def mouseWheelMovedDown(self, event):
		pass
		
	def mouseDragged(self, event):
		if event.isConsumedByWidgets():
			return

		clickpoint = fife.ScreenPoint(event.getX(), event.getY())
		if (event.getButton() == fife.MouseEvent.LEFT):
			if clickpoint.x > self._lastmousepos[0] + 5 or  clickpoint.x < self._lastmousepos[0] - 5 or clickpoint.y > self._lastmousepos[1] + 5 or clickpoint.y < self._lastmousepos[1] - 5:
				self._gamecontroller.scene.player.walk( self._gamecontroller.scene.getLocationAt(clickpoint) )

			self._lastmousepos = (clickpoint.x, clickpoint.y)
		
	def keyPressed(self, event):
		keyval = event.getKey().getValue()
		keystr = event.getKey().getAsString().lower()
		if keyval == fife.Key.ESCAPE:
			self.detach()
			self._gamecontroller.guicontroller.showMainMenu()
			event.consume()
			
		self._gamecontroller.keystate.updateKey(keyval, True)
		
	def keyReleased(self, event):
		keyval = event.getKey().getValue()
		keystr = event.getKey().getAsString().lower()
		
		self._gamecontroller.keystate.updateKey(keyval, False)
		
class GameController(object):
	def __init__(self, application, engine, settings):
		self._application = application
		self._engine = engine
		self._settings = settings
		
		self._keystate = KeyState()
		
		self._guicontroller = GUIController(self)
		
		self._listener = GameListener(self)
		
		self._guicontroller.showMainMenu()
		
		self._scene = None
		self._instancerenderer = None
		self._floatingtextrenderer = None
		
	def onConsoleCommand(self, command):
		"""
		Might be useful if you want to have the game parse a command.
		Not sure if I am going to keep this or not.
		"""
		
		result = ""
		
		args = command.split(" ")
		cmd = []
		for arg in args:
			arg = arg.strip()
			if arg != "":
				cmd.append(arg)
		
	
		if cmd[0] == "spawn":
			result = "Usage: spawn [item|actor] [id] [posx] [posy]"
			if len(cmd) != 5:
				print len(cmd)
				return result
			else:
				if cmd[1] == "item":
					obj = self._scene.loadItem(cmd[2])
				elif cmd[1] == "actor":
					obj = self._scene.loadActor(cmd[2])
				else:
					return result
				if obj:
					self._scene.addObjectToScene(obj)
					result = "Success!"
				else:
					result = "Error: Not Found!"
			
		return result
		
	def newGame(self):
		self._guicontroller.hideMainMenu()
		
		self._keystate.reset()
		
		if self._scene:
			self._scene.destroyScene()
			self._scene = None
		
		self._scene = Scene(self)
		self._scene.createScene(self._settings.get("RPG", "TownMapFile", "maps/town.xml"))

		self._instancerenderer = fife.InstanceRenderer.getInstance(self._scene.cameras[self._settings.get("RPG", "DefaultCameraName", "camera1")])
		self._floatingtextrenderer = fife.FloatingTextRenderer.getInstance(self._scene.cameras[self._settings.get("RPG", "DefaultCameraName", "camera1")])
		self._floatingtextrenderer.addActiveLayer(self._scene.actorlayer)

		#start listening to events
		self._listener.attach()
		
	def endGame(self):
		if self._scene:
			self._scene.destroyScene()
			self._scene = None
			self._instancerenderer = None
			self._floatingtextrenderer = None
		
	def quit(self):
		self.endGame()
		self._application.requestQuit()

	def pump(self):
		if self._scene:
			self._scene.updateScene()
		
		
	def _getGUIController(self):
		return self._guicontroller
		
	def _getEngine(self):
		return self._engine
		
	def _getSettings(self):
		return self._settings
		
	def _getScene(self):
		return self._scene
	
	def _getKeyState(self):
		return self._keystate
	
	def _getInstanceRenderer(self):
		return self._instancerenderer
	
	guicontroller = property(_getGUIController) 
	engine = property(_getEngine)
	settings = property(_getSettings)
	scene = property(_getScene)
	keystate = property(_getKeyState)
	instancerenderer = property(_getInstanceRenderer)