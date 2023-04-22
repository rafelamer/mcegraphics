#!/usr/bin/python
# -*- coding: utf-8 -*-
#
import numpy as np

class Haar:
    X = None
    Y = None
    phiY = None
    psiY = None
    level = None
    index = None
    dimension = None
    number = None
    
    def __init__(self):
        self.level = 0
        self.index = 0
        self.dimension = 0
        self.number = 0
        self.X = np.linspace(-0.5,1.5,400)

    def phi(self,x):
        if x > 1.0 or x < 0:
            return 0.0
        return 1.0

    def psi(self,x):
        if x > 1.0 or x < 0:
            return 0.0
        if x < 0.5:
            return 1.0
        return -1.0
    
    def Phi(self,x,i,j):
        return self.phi(2**j*x-i)

    def Psi(self,x,i,j):
        return self.psi(2**j*x-i)

    def set_level(self,_n):
        try:
            n = int(_n)
        except:
            return
        if n < 0:
            return
        self.level = n

    def set_index(self,_n):
        try:
            n = int(_n)
        except:
            return
        if n < 0:
            return
        if n >= 2**self.level:
            return
        self.index = n

    def set_dimension(self,_n):
        try:
            n = int(_n)
        except:
            return
        if n < 0:
            return
        self.dimension = n

    def set_function_number(self,_n):
        try:
            n = int(_n)
        except:
            return
        if n < 0:
            return
        if n >= 2**self.dimension:
            return
        self.number = n
        
    def set_level_index(self):
        if self.number <= 1:
           self.level = 0
           self.index = 0
           return
        m = 2
        while self.number >= 2**m:
            m += 1
        self.level = m-1
        self.index = self.number-2**self.level

    def set_y_values(self):
        self.phiY = [self.Phi(t,self.index,self.level) for t in self.X]
        self.psiY = [self.Psi(t,self.index,self.level) for t in self.X]
        for k in range(1,len(self.phiY)):
            if np.abs(self.phiY[k]-self.phiY[k-1]) > 0.5:
                self.phiY[k] = np.nan
            if np.abs(self.psiY[k]-self.psiY[k-1]) > 0.5:
                self.psiY[k] = np.nan
        if self.number == 0:
            self.Y = self.phiY
        else:
            self.Y = self.psiY

