import sys, time, math
#import shelve # persistent storage is way too slow
from util import *

import cProfile

LabelLen = 16
def label(j, x):
	j = j.to_bytes(LabelLen, 'big')
	h = OracleH(j.join(x))
	return h[:LabelLen]

External = True	
	
def PebbleExpanderStack(logn, logd, k, nonce=0):
	n = 1 << logn
	d = 1 << logd	
	Vs = [bytearray(LabelLen)] * n	# sources
	Vt = [bytearray(LabelLen)] * n	# sinks
		
	for j in range(n):
		Vt[j] = label((nonce << logn) + j, [])	# start from Vt because of the swap in the loop
		
	Pi = FeistelPerm(logn+logd)	
	Pi = SimplePerm(n*d)
	Parents = [[0] * d] * n
	for j in range(n):
		Parents[j] = [p % n for p in Pi.permRange(d*j, d)]
	
	start = time.clock()	
	for i in range(k):	
		Vs, Vt = Vt, Vs
		for j in range(n):
			Vt[j] = label(j, [Vs[p] for p in Parents[j]])
			#Vt[j] = label(j, [Vs[p%n] for p in Pi.permRange(d*j, d)])

	end = time.clock()
	print(end-start)	
	

def PebbleButterflyStack(logn, k, nonce=0):
	n = 1 << logn	
	Vs = [bytearray(LabelLen)] * n	# sources
	Vt = [bytearray(LabelLen)] * n	# sinks
	for j in range(n):
		Vt[j] = label((nonce << logn) + j, [])	# start from Vt because of the swap in the loop
	
	ButLayer = list(range(logn-1, 1, -1)) + list(range(logn))
	Parents = [[[0]*2]*n]*logn
	for l in range(logn):
		for j in range(n):
			Parents[l][j][0] = j
			Parents[l][j][1] = j - int(math.copysign(1<<l, (j%(2<<l))- (1<<l)))
	
	start = time.clock()
	for i in range(k):
		for l in ButLayer:
			Vs, Vt = Vt, Vs
			for j in range(n):
				Vt[j] = label(j, [Vs[p] for p in Parents[l][j]])
	end = time.clock()
	print(end-start)					
			
#cProfile.run('PebbleButterflyStack(16, 10)')			
cProfile.run('PebbleExpanderStack(16, 4, 10)')	