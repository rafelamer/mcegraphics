#!/usr/bin/python
# -*- coding: utf-8 -*-
#

import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk
from HaarLevels import HaarLevels


win = HaarLevels()
win.connect('delete-event', Gtk.main_quit)
win.show_all()
Gtk.main()



