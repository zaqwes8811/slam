#!/usr/bin/env python
# coding: utf-8

""" 
	Редко используют симулинк - кажется не так, для чего-то редко

	Проблема с нелинейными элементами в python-control
		нет элементов с насыщениями.
"""

from numpy import convolve
import numpy
from numpy import ndarray
from sympy import Symbol, Integer, expand, cancel
from sympy import fraction, Poly, poly, simplify, collect

from control.matlab import zpk2tf
from control.matlab import tf2zpk
from control.matlab import tf
from control.matlab import step
from control.matlab import nyquist
from control_ext.matlab import *
from control_ext import syms2tf, disp

def t9():
	b = [20, 30, 40]
	a = [0, 2, 10, 100, 0, 900]
	return tf2zpk(b, a)

def t10():
	# fixme: add PlotStyle
	# http://sTckoverflow.com/questions/11481644/how-do-i-assign-multiple-labels-at-once-in-matplotlib
	syss = [tf([10], [1, 10]), 
			tf([100], convolve([1, 10], [1, 10])),
			tf([1000], convolve([1, 10], convolve([1, 10], [1, 10])))]
	labels = ['T', 'F', 'M']

	w = logspace(-1, 3, 1e3)  # better then linspace
	for l, sys in zip(labels, syss):
		bode(sys, w, label=l)

	legend()
	show()

def t11():
	# fixme: add labels to everything
	
	def plot_sys(sys):
		ys, ts = step(sys)
		plot(ts, ys)

	syss = [tf([100], [1, 4, 100]), 
			tf([10], [1, 10]), 
			tf([100], [1, 2, 100]), 
			tf([100], [1, 1, 100])]
	for sys in syss:
		plot_sys(sys)
	legend()
	figure()

	for sys in syss:
		bode(sys)
	legend()

	show()

def t12():
	# fixme: F = 1 - any... wild - STRANGE
	s = Symbol('s')
	
	T = 50.0*(s+3)*(s+12.0)/((s+30.0)*(s+55.0)*(s+100.0)*(s+1000.0))
	# T = 60*(s + 3)*(s + 16)/((s + 33)*(s + 75)*(s + 200)*(s + 2000))
	# # T = Add(5000, s) / (s*(s+5)*(s+50))
	# T = 10*(s + 2)*(s + 22)/((s + 40)*(s + 65)*(s + 150))
	# T = -20*(s + 2)*(s + 26)/((s + 43)*(s + 85)*(s + 250)*(s + 2500))
	# T = 2.72 * (s + 7)*(s + 20)/((s + 10)*(s + 100)*(s + 1000))
	T = -25*(s + 2)*(s + 44)/((s + 55)*(s + 66)*(s + 77)*(s + 8800))
	# T = 50*( s + 0.05 )*( s + 0.5 ) / (s *( s + 5 )) * 5000/(s+10) * 1/5000/s**2
	# loops
	F = T+1
	M = T/F
	# T = F

	syss = [syms2tf(T), syms2tf(F), syms2tf(M)]
	labels = ['T', 'F', 'M']
	w = logspace(-1, 2, 1e3)  # need it
	for l, sys in zip(labels, syss):
		bode(sys, w, label=l)

	legend()

	# nyquist([syms2tf(T)])
	show()

def t18():
	# fixme: make lplane()
	T = ([20, 30, 40], [2, 1, 1, 0, 3])
	sys = tf(*T)
	mag, phase, omega = bode(sys)
	# clear()
	# plot(phase, mag)
	# grid()
	show()

def t19():
	pass

def t39():
	s = Symbol('s')
	k = 10
	T = 1000 * (s+20) / ((s+1)*s)
	Effect = D / F

def analyse_ex():
	s = Symbol('s')
	
	# Loop
	C = 50*(s+0.05)*(s+0.5) / (s*(s+5))
	C = (50*s**2+ 27.5*s+0.25) / (s*(s+5))
	A = Poly(5000, s) / (s+10)
	B = 1
	P = Poly(1, s)/(5000*s**2)

	# D3_in = Poly(1, s)/((s+0.1) * (s+2))
	# OL_D3_out = expand( P * D3_in )
	# dist = syms2tf( OL_D3_out, s )

	# w = logspace(-2, 1, 1000)
	# bode(dist, w, label="src")
	# bode(syms2tf(D3_in,s), w, label="feedback")
	# legend()
	# show()

	T = C*A*P
	F = T + 1
	M = T/F
	w = logspace( -1, 2, 1e4 )

	# fixme: autocast
	# fixme: не сходится с книжкой
	# fixme: bug: if (<-1, N)... не может правильно определить фазу
	sys = syms2tf( T, s )

	mag, phase, omega = bode(sys, w, label="T(s)")
	bode(syms2tf( F,s ), w, label="F(s)")
	bode(syms2tf( M,s ), w, label="M(s)")
	legend()
	show()

	clear()
	plot( phase, mag )
	# grid()
	# show()


if __name__=='__main__':
	analyse_ex()