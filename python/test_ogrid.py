# -*- coding: utf-8 -*-
'''
test_ogrid.py

sudo apt-get install python-dev libxtst-dev scons python-vtk pyqt4-dev-tools python2.7-wxgtk2.8 python-configobj
sudo pip install mayavi

'''
import numpy as np
from mayavi import mlab

x, y = np.ogrid[-2:2:20j, -2:2:20j]
z = x * np.exp( - x**2 - y**2)

pl = mlab.surf(x, y, z, warp_scale="auto")
mlab.axes(xlabel='x', ylabel='y', zlabel='z')
mlab.outline(pl)