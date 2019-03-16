# encoding: utf-8

# fixme: outliners - Kalman must be robust 
#   https://www.itwm.fraunhofer.de/fileadmin/ITWM-Media/Zentral/Pdf/Berichte_ITWM/2010/bericht_185.pdf
#
# fixme: робастный и online
#
# fixme: моневрирование - MMAE(залипает!), fading mem
#
# Трекинг вообще - non-linear
# http://gandalf.psych.umn.edu/users/schrater/Papers/VeerPapaSch06.pdf
#
# моневрирующие цели
# http://arxiv.org/pdf/1503.07828.pdf

import numpy as np
from numpy import array
from numpy.random import randn  # too weak
from filterpy.kalman import KalmanFilter
from filterpy.common import Q_discrete_white_noise

from matlab_ext.plotter import *

class Car(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.vx = 0.
        self.vy = 10.1
        self.ax = 0.
        self.ay = 0.
        
    def to_kph(self):
        return array([self.vx, self.vy ]) * 3.6

class Mover(object):
    def __init__( self ):
        vmax = 60  # kph
        self.car = Car( 0, 0 ); 
        self.car.vx = vmax / 3.6  # m/s
        self.iter = 0
        
    def think(self):
        if self.iter == 15:
            self.car.ax = 60
        if self.iter == 25:
            self.car.ax = 0
        self.iter += 1
        
    def move(self, dt=1.):
        def rnd():
            std = 2.
            return randn() * std
            
        self.think()
        
        # fixme: сделать через матрицы
        # X
        self.car.x += self.car.vx * dt + self.car.ax * dt**2 / 2.
        self.car.vx += self.car.ax * dt
        
        # Y
        self.car.y += self.car.vy * dt + self.car.ay * dt**2 / 2.
        self.car.vy += self.car.ay * dt
        
        return self.car.x + rnd(), self.car.y+rnd(), self.car.x

# coord sys as in image
#
#   http://www.autonomoustuff.com/uploads/9/6/0/5/9605198/sms_type_29_data_sheet.pdf
class WorldSpace(object):
    def __init__( self, w, h ):
        self.w = w
        self.h = h
  
    def check_accessory( self, car ):
        return car.x < w and car.y < self.h

def make_cv_filter(dt, noise_factor):
    cvfilter = KalmanFilter(dim_x = 2, dim_z=1)
    cvfilter.x = array([0., 0.])
    cvfilter.P *= 3
    cvfilter.R *= noise_factor
    cvfilter.F = array([[1, dt],
                        [0, 1]], dtype=float)
    cvfilter.H = array([[1, 0]], dtype=float)
    var = 2.02
    cvfilter.Q = Q_discrete_white_noise(dim=2, dt=dt, var=var)
    return cvfilter

def initialize_filter(kf, noise_factor=None):
    """ helper function - we will be reinitialing the filter
    many times.
    """
    kf.x.fill(0)
    kf.P = np.eye(kf.dim_x) * .1
    if noise_factor is not None:
        kf.R = np.eye(kf.dim_z) * noise_factor
        
#############################################################

if __name__=='__main__':
    cls()
    close_all()
        
    world_space = WorldSpace( 10, 120 )
    mover = Mover()
    
    N = 100
    fps = 22
    dt = 1./fps
    noise_factor = 2.
    # filter    

    cvfilter = make_cv_filter(dt, noise_factor)
    initialize_filter(cvfilter)
    cvfilter.alpha = 1.08
    
    xs = np.zeros( N )
    ys = np.zeros( N )
    ts = np.zeros( N )
    kxs = np.zeros( N )
    rs_x = np.zeros( N )
    t = 0
    for i in range( N ):
        z_x, ys[i], xs[i] = mover.move( dt )

        cvfilter.predict()
        cvfilter.update(z_x)  

        kxs[i] = cvfilter.x[0]
        rs_x[i] = cvfilter.residual_of(z_x)
        
        t += dt
        ts[i] = t
        
    # draw
    figure()
    plot( ts, xs )
    plot( ts, kxs, '-g' )
    xlabel('t, s')
    ylabel('x, m')
    grid()
    show()
    
    figure()
    plot(ts, rs_x)
    grid()

    
        
         



