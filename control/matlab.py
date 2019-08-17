from numpy import linspace
from numpy import logspace
from matplotlib.pyplot import show
from matplotlib.pyplot import grid
from matplotlib.pyplot import clf as clear
from matplotlib.pyplot import legend
from matplotlib.pyplot import plot
from matplotlib.pyplot import figure

import control.matlab

def bode(sys, *arg, **args):
	# fixme: check in args dB and deg
	return control.matlab.bode(sys, *arg, dB=True, deg=True, **args)
