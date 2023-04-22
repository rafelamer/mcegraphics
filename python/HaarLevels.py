#!/usr/bin/python
# -*- coding: utf-8 -*-
#

import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk
import numpy as np
from matplotlib.figure import Figure
from matplotlib.backends.backend_gtk3agg import FigureCanvasGTK3Agg
from Haar import Haar

class HaarLevels(Gtk.Window):
    maxLevel = 6
    def __init__(self):
        Gtk.Window.__init__(self, title = 'Haar Levels')
        layout = Gtk.Box.new(Gtk.Orientation.VERTICAL,0)
        self.add(layout)
        self.fig = Figure((10, 10), dpi = 80)
        w, h = self.fig.get_size_inches()
        dpi_res = self.fig.get_dpi()
        w,h = int(np.ceil(w * dpi_res)), int(np.ceil(h * dpi_res))
        canvas = FigureCanvasGTK3Agg(self.fig)
        canvas.set_size_request(w, h)
        idx = Gtk.Box.new(Gtk.Orientation.HORIZONTAL,0)
        layout.add(idx)
        levelLabel = Gtk.Label.new('Level:  ')
        idx.add(levelLabel)
        a1 = Gtk.Adjustment(0, 0, 100, 1, 10, 0)
        self.levelButton = Gtk.SpinButton()
        self.levelButton.set_adjustment(a1)
        self.levelButton.connect('value-changed',self.levelButtonChanged)
        idx.add(self.levelButton)
        idx.add(Gtk.Label.new('          '))
        indexLabel = Gtk.Label.new('Index:  ')
        idx.add(indexLabel)
        self.indexButton = Gtk.SpinButton()
        a2 = Gtk.Adjustment(0, 0, 100, 1, 10, 0)
        self.indexButton.set_adjustment(a2)
        self.indexButton.connect('value-changed',self.indexButtonChanged)
        idx.add(self.indexButton)
        layout.add(canvas)
        self.draw_figure()

    def levelButtonChanged(self,_m):
        m = self.levelButton.get_value()
        if m > self.maxLevel:
            self.levelButton.set_value(self.maxLevel)
            return
        self.haar.set_level(m)
        self.haar.set_index(0)
        self.indexButton.set_value(0)        
        self.haar.set_y_values()
        self.l1.set_ydata(self.haar.phiY)
        self.l2.set_ydata(self.haar.psiY)
        self.fig.canvas.draw_idle()
         
    def indexButtonChanged(self,_m):
        m = self.indexButton.get_value()
        if m >= 2**self.haar.level:
            self.indexButton.set_value(2**self.haar.level-1)
            return
        self.haar.set_index(m)
        self.haar.set_y_values()
        self.l1.set_ydata(self.haar.phiY)
        self.l2.set_ydata(self.haar.psiY)
        self.fig.canvas.draw_idle()
         
    def draw_figure(self):
        self.haar = Haar()
        self.haar.set_y_values()
        self.ax = self.fig.add_subplot(211,ylim=(-1.25,1.25))
        self.bx = self.fig.add_subplot(212,ylim=(-1.25,1.25))
        self.ax.set_xticks(np.arange(-0.5,1.5, 0.25))
        self.ax.set_yticks(np.arange(-1.25,1.5, 0.25))
        self.ax.grid(True)
        self.l1, = self.ax.plot(self.haar.X,self.haar.phiY,lw = 3)
        self.bx.set_xticks(np.arange(-0.5,1.5, 0.25))
        self.bx.set_yticks(np.arange(-1.25,1.5, 0.25))
        self.bx.grid(True)
        self.l2, = self.bx.plot(self.haar.X,self.haar.psiY,lw = 3)
        self.fig.canvas.draw()



