#!/usr/bin/env python
# coding : utf8

# fixme: try scipy.signal
# fixme: extract or use exist bode/hod

# fixme: integr into signal sys
#    http://stackoverflow.com/questions/12451767/how-to-define-lti-systems-with-time-delay-in-scipy

import numpy as np
import matplotlib.pyplot as plt

def bode_analog(h, ws, param):
	mags = []
	phs = []
	for w in ws:
		point = h(w * 1.0j)
		mag, ph = np.absolute(point), np.angle(point)
		mags.append(mag)
		phs.append(ph)

	phs = np.unwrap(np.array(phs))

	plt.subplot(2, 1, 1)
	plt.plot(10*np.log10(ws), 20 * np.log10(mags), param)
	plt.title('Bode')
	plt.xlabel('W, rad/s')
	plt.ylabel('Ampl, dB')
	plt.grid(True, which='both')

	plt.subplot(2, 1, 2)
	plt.grid(True, which='both')
	plt.plot(10*np.log10(ws), phs * 180 / np.pi, param)
	plt.xlabel('W, rad/s')
	plt.ylabel('Phase, deg')

def godo(h, ws, param):
	mags = []
	phs = []
	for w in ws:
		point = h(w*1.0j)
		mag, ph = np.real(point), np.imag(point)
		mags.append(mag)
		phs.append(ph)

	#phs = np.unwrap(np.array(phs))
	plt.plot(-1, 0, "x")
	plt.plot(0, 0, "v")
	plt.grid(True, which='both')
	plt.plot(mags, phs, param)

def pade(w, h=1.0):
	#   http://www.mathworks.com/help/control/ref/pade.html
	#http://lpsa.swarthmore.edu/BackGround/TimeDelay/TimeDelay.html

	a = h**1 / 2.0
	b = h**2 / 12.0
	c = h**3 / 120.0
	return \
		(1 - w * a + w**2 * b - w**3 * c ) / \
		(1 + w * a + w**2 * b + w**3 * c )

if __name__ == '__main__':
	# TROUBLE: delay 
	#   http://www.control.lth.se/media/Education/DoctorateProgram/2012/Delays/Lectures/lect01.pdf
	#   http://www.mathworks.com/help/control/examples/analyzing-control-systems-with-delays.html

	xs = np.linspace(0, 20, 100)
	ws = np.linspace(0.1, 1e2, 500)

	# Gen signal
	# fixme: add const lag
	# fixme: high bandwidth sensor and big delay
	def h(w):
		a = 10 / (w + 1)
		a *= np.exp(-0.1 * w)
		return a

	def h2(w):
		a = 100 / (w + 10)
		a *= np.exp(-0.1 * w)
		return a

	def h1(w):
		return h(w) / (1 + h(w))

	# Bode
	# bode_analog(h1, ws, "-b")
	# bode_analog(h, ws, "-r")
	# bode_analog(h2, ws, "-g")

	# Hodog.
	godo(h, ws, "-b")
	godo(h2, ws, "-g")

	# FFT

	plt.show()

