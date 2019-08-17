#!/usr/bin/env python
# coding: utf-8

"""
	Goal: не ставить много дополнительный пакетов
	  	а использовать голый математический пакет с
	  	тонкой прослойкой над ним. Плюс это возволяет
	  	расширять модели после

	Goal: как можно меньше Simulink-like вещей

	fixme: численно или символьно?
	
"""

from sympy import oo, Poly, pprint
from sympy.integrals import laplace_transform as laplace
from sympy.integrals import inverse_laplace_transform as ilaplace
from sympy import fraction
from control.matlab import tf
import matplotlib.pyplot as plt


# fixme: l-plane with bounds

# fixme: to export scilab like api
inf = oo

# all get as input H(s), H1(s),... - Laplace domain
def poly( rep, *gens, **args ):
	return Poly.from_list( rep, *gens, **args )

def pfss( f ):
	A = apart(f).as_terms()
	return zip( *A[0] )[0]

def syslin( f ):
	return f

def roots( f ):
	return polys.polytools.nroots( f, n=3 )

def denom( f ):
	return fraction( f )[1]

def plzr( f ):
	# fixme: plot roots and zeros
	pass

def parallel(sys1, sys2):
	pass

def coeffs():
	pass

def numer():
	pass

def det():
	# определитель
	pass

def horner():
	pass
	
# utils
def disp( some ):
	pprint(some)

# from scilab tutorial
def laplace_and_matrix():
	t, s = symbols('t, s')

	if False:	
		y = laplace(exp(t), t, s)
		disp(y)

		y = laplace(4+5*exp(-3*t), t, s)
		disp(y)

		y = laplace(4+5*exp(-3*t), t, s)
		disp(y)

		disp( laplace(2*t-3*exp(-t), t, s) )

		# poly
		p = poly( [9, 1], s )
		q = poly( [3, 7, 1], s )
		f = p/q
		disp( f )

		# final/init value 
		# Не всегда применими, см. видеолекции Classical Control Theory by
		#   Brian Douglas
		#
		# https://en.wikipedia.org/wiki/Final_value_theorem
		final_val = limit( s*f, s, 0 )
		disp( final_val )
		initial_val = limit( s*f, s, inf )
		disp( initial_val )

	f = (s+3)/((s+1)*(s+2)*(s+4))
	A = pfss(f)
	map( lambda v: disp( ilaplace(v, s, t) ), A )

	# fixme: dbl() - ?

	# fixme: construct from roots

def transfer_function():
	t, s = symbols('t, s')
	f = ( 4*(s+2)*((s+Rational(2))**3) )/((s+6)*( (s+4)**2 ) )
	H = syslin( f )
	# disp( ilaplace(apart(H), s, t) )  # can't direct

	# http://docs.sympy.org/0.6.7/modules/simplify.html
	numer, _denom = fraction(H)

	disp(numer)
	print roots( numer )  # fixme: !!! complex roots!
	print roots( denom(H) )

def control_system_components():
	pass

def numeric_parts():
	# http://docs.sympy.org/0.7.0/modules/mpmath/calculus/index.html
	pass

def root_locus_method():
	# fixme: spec functions
	# https://help.scilab.org/docs/5.5.2/en_US/evans.html
	# https://help.scilab.org/doc/5.5.2/en_US/kpure.html
	pass

def frequency_domain_analysis():
	pass

def bode_plot():
	pass

def nyquist_plot():
	pass

	
def state_variable_approach():
	# state-space
	# http://project3001.org/docs/blog/introsympy/

	# state-space to transfer, delay, nonlin?

	# Разные матрица - observ, contr...
	# https://en.wikipedia.org/wiki/State-space_representation
	# see nonlinear
	#
	# With delays
	#   http://www.mathworks.com/help/control/ug/analyzing-control-systems-with-delays.html
	#
	# tf vs ss
	#   http://www.atp.ruhr-uni-bochum.de/rt1/syscontrol/node97.html
	#   https://www.physicsforums.com/threads/control-theory-laplace-versus-state-space-representation.265934/

	# Trouble: lti + nonlinear + feedback? How connect and solve?
	#   http://www.mathworks.com/help/control/ref/series.html
	#   http://www.math.kth.se/optsyst/grundutbildning/kurser/SF2832/Project/P_PBH.pdf
	
	# !! https://github.com/masfaraud/BMSpy/blob/master/bms/blocks.py
	#
	# HOW MODEL ALL TOGETHER?
	#
	pass

def digital_control_systems():
	pass


# fixme: SISO and MIMO systems
#   https://en.wikibooks.org/wiki/Control_Systems/MIMO_Systems

def __zp2tf():
	z = 10*numpy.array([-1, -3, -8])
	p = [-4, -35, -100, -200]
	return tf( *zpk2tf(z, p, 1) )


def syms2tf(f, s):
	n, d = fraction(f.cancel())
	n = map(float, Poly(n, s).all_coeffs())
	d = map(float, Poly(d, s).all_coeffs())
	return tf( n, d )


if __name__=="__main__":
	init_printing(use_unicode=True)

	control_system_components()

