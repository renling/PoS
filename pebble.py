import sys, time, math
from util import *

import cProfile

LabelLen = 16
def label(j, x):
	j = j.to_bytes(LabelLen, 'big')
	h = OracleH(j.join(x))
	return h[:LabelLen]

def pebble():

	logn = 12
	logd = 4
	n = 1 << logn
	d = 1 << logd
	k = 50

	nonce = 0
	
	Vs = [bytearray(LabelLen)] * n	# sources
	Vt = [bytearray(LabelLen)] * n	# sinks
	for j in range(n):
		Vt[j] = label((nonce << logn) + j, [])	# initial Vt because of the swap in the loop

	Pi = FeistelPerm(logn+logd)	
	Pi = SimplePerm(n*d)

	Parents = [[0] * d] * n
	for j in range(n):
		Parents[j] = [p % n for p in Pi.permRange(d*j, d)]
		
	start = time.clock()
	for i in range(k):	
		Vs, Vt = Vt, Vs
		for j in range(n):
			Vt[j] = label(j, [Vs[p%n] for p in Parents[j]])
			#Vt[j] = label(j, [Vs[p%n] for p in Pi.permRange(d*j, d)])
	end = time.clock()
	print(end-start)
	
cProfile.run('pebble()')	