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

class HaarBasis(Gtk.Window):
    maxLevel = 6
    def __init__(self):
        Gtk.Window.__init__(self, title = 'Haar Basis')
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
        dimensionLabel = Gtk.Label.new('Basis of space F:  ')
        idx.add(dimensionLabel)
        a1 = Gtk.Adjustment(0, 0, 100, 1, 10, 0)
        self.dimensionButton = Gtk.SpinButton()
        self.dimensionButton.set_adjustment(a1)
        self.dimensionButton.connect('value-changed',self.dimensionButtonChanged)
        idx.add(self.dimensionButton)
        idx.add(Gtk.Label.new('          '))
        functionNumberLabel = Gtk.Label.new('Function number:  ')
        idx.add(functionNumberLabel)
        self.functionNumberButton = Gtk.SpinButton()
        a2 = Gtk.Adjustment(0, 0, 100, 1, 10, 0)
        self.functionNumberButton.set_adjustment(a2)
        self.functionNumberButton.connect('value-changed',self.functionNumberButtonChanged)
        idx.add(self.functionNumberButton)    
        layout.add(canvas)
        self.draw_figure()

    def dimensionButtonChanged(self,_m):
        m = self.dimensionButton.get_value()
        if m > self.maxLevel:
            self.dimensionButton.set_value(self.maxLevel)
            return
        self.functionNumberButton.set_value(0)        
        self.haar.set_dimension(m)
        self.haar.set_function_number(0)
        self.haar.set_level_index()
        self.haar.set_y_values()
        self.l1.set_ydata(self.haar.Y)
        self.fig.canvas.draw_idle()
         
    def functionNumberButtonChanged(self,_m):
        m = self.functionNumberButton.get_value()
        n = self.dimensionButton.get_value()
        if m >= 2**n:
            self.functionNumberButton.set_value(2**n-1)
            return
        self.haar.set_dimension(n)
        self.haar.set_function_number(m)
        self.haar.set_level_index()
        self.haar.set_y_values()
        self.l1.set_ydata(self.haar.Y)
        self.fig.canvas.draw_idle()
         
    def draw_figure(self):
        self.haar = Haar()
        self.haar.set_y_values()
        self.ax = self.fig.add_subplot(111,ylim=(-1.25,1.25))
        self.ax.set_xticks(np.arange(-0.5,1.5, 0.25))
        self.ax.set_yticks(np.arange(-1.25,1.5, 0.25))
        self.ax.grid(True)
        self.l1, = self.ax.plot(self.haar.X,self.haar.Y,lw = 3)
        self.fig.canvas.draw()



