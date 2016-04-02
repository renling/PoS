import sys, time
from util import *

LabelLen = 128
def label(j, x):
	j = j.to_bytes(LabelLen, 'big')
	h = hash(j.join(x))
	return h[:LabelLen]

logn = 12
logd = 6
n = 1 << logn
d = 1 << logd
k = 10

nonce = 0

Vs = [0] * n	# sources
Vt = [0] * n	# sinks
for j in range(n):
	Vt[j] = label((nonce << logn) + j, [])	# initial Vt because of the swap in the loop

Pi = FeistelPerm(logn+logd)	
Pi = SimplePerm(n*d)

start = time.clock()
for i in range(k):	
	Vs, Vt = Vt, Vs
	for j in range(n):
		parents = [Pi.perm(j+n*t) % n for t in range(d)]
		#parents = [0 for t in range(d)]
		Vt[j] = label(j, [Vs[p] for p in parents])			
end = time.clock()
print(end-start)