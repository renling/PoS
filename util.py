import sys, hashlib, random

def hash(x):
	return hashlib.sha224(x).digest()

# PRF_k(x): n -> n bits
def PRF(x, k, n):
	x += k << n
	x = x.to_bytes(n, 'big')
	h = hashlib.md5(x).digest()
	return int.from_bytes(h[:n], 'big')

class FeistelPerm:
	'a 3-round Feistel permutation on 2^{n}, n must be even, k is an optional seed'
	def __init__(self, n, s=0):
		if n % 2:
			print("Feistel permutation requires even bit length")
			sys.exit(1)
		random.seed(s)	
		self.n2 = int(n/2)	
		self.key = [random.getrandbits(self.n2) for i in range(3)]
	
	def perm(self, x):
		n2 = self.n2
		for i in range(3):
			L = x >> n2
			R = x & ((1 << n2)-1)
			x = (R << n2) + PRF(R, self.key[i], n2) ^ L
		return x
		
class SimplePerm:
	'A trivial permutation'
	def __init__(self, N):		
		self.pi = [i for i in range(N)]
		random.shuffle(self.pi)
	
	def perm(self, x):
		return self.pi[x]