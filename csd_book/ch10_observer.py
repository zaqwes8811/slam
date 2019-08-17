# -*- coding: utf-8 -*-
"""
Created on Sat Apr 23 12:57:04 2016

@author: zaqwes
"""

import copy

from control import bode_plot
from control import tf, series, parallel, feedback
from control import impulse_response, step_response
from control.matlab import ss, c2d

from matlab_ext.plotter import *
from matlab_ext.r_ext import *


def synthesize_1pole_filter(f):
    K = f*(2*np.pi)
    return tf([K],[1, K])
    
    
def synthesize_2pole_filter(f, dr):
    wn = f*2*np.pi
    return tf([wn**2], [1, 2*dr*wn, wn**2])


def classic():
    # PI controller
    Kp = 1.5
    Ki = 30.
    
    P = tf([Kp], [1])
    I = tf([Ki], [1, 0])
    
    Gc = parallel(P, I)
    
    # Power convertor
    Gpc = synthesize_2pole_filter(50, 0.707)
    
    # Plant
    Gp = tf([50], [1, 0])
    
    # Sensor
    # если выбросить из петли становится лучше
    # "remove sensor lag"
    Gs = synthesize_1pole_filter(20)
    
    # Openloop
    Gol = series(series(Gc, Gpc), Gp)
    
    # Closed loop
    Gcl = feedback(Gol)
    
    ts, xs = step_response( Gcl )
    
    grid()
    plot(ts, xs)

    
if __name__ == '__main__':
    cls()
    

    # fixme: как так соединить?
    # "Mason's signal fow graphs"
    # 
    # Opened loop -> closed loop
    #
    # fixme: нужны разные операции над блоками
    # parallel - +, series - *, negate - -, feedback - ?
    # как деление?






















