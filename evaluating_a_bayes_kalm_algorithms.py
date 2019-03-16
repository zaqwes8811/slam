# -*- coding: utf-8 -*-
"""
Created on Sun Mar 13 18:59:02 2016

@author: zaqwes
"""

 '''"Do not trust the filter’s covariance matrix to tell you 
 if the filter is performing well!"'''

'''
"Again, the residual plot tells the story."'''

'''
 "For best performance you need a filter whose order matches the system’s
 order.. In many cases that will be easy to do - if you are designing a Kalman
 filter to read the thermometer of a freezer it seems clear that a zero order
 filter is the right choice. But what order should we use if we are tracking
 a car? Order one will work well while the car is moving in a straight line
 at a constant speed, but cars turn, speed up, and slow down, in which case
 a second order filter will perform better. That is the problem addressed
 in the Adaptive Filters chapter. There we will learn how to design a filter
 that adapts to changing order in the tracked object’s behavior."'''

'''
"With that said, a lower order filter can track a higher order process so long
 as you add enough process noise and you keep the discretization period
 small (100 samples a second are usually locally linear). The results will
 not be optimal, but they can still be very good, and I always reach for this
 tool first before trying an adaptive filter." '''
 
'''
"Modelling an automobile is very difficult. The
steering causes nonlinear behavior, tires slip, people brake and accelerate
hard enough to cause tire slips, winds push the car off course. The end result
is the Kalman filter is an inexact model of the system. This inexactness
causes suboptimal behavior, which in the worst case causes the filter to
diverge completely."
'''

'''
If you are a hobbiest my coverage
may get you started. A commercial grade filter requires very careful design
of the fusion process. That is the topic of several books, and you will have
to further your education by finding one that covers your domain.
'''