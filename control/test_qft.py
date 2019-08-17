#!/usr/bin/env python

from control import tf
from control import bode
from control import nyquist_plot

from control import gangof4
from control import nichols_plot

import matplotlib.pyplot as plt

if __name__=="__main__":

	# fixme: adding one by one buggy

	sys_tf = tf([2.], [1., 3])
	sys_tf1 = tf([20.], [1., 3])
	sys_tf2 = tf([200.], [10., 3])
	#real, imag, freq = 
	nichols_plot([sys_tf, sys_tf1])
	#nichols_plot(sys_tf2)
	# mag, phase, omega = bode(sys_tf)

	# fixme: plot spec bounds
	# fixme: gui?
	
	#plt.clf()

	# plt.plot(mag, omega)
	plt.show()