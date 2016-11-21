import sys, hashlib, random, math
from Crypto.Cipher import AES
Encryptor = AES.new(b'1234567812345678', AES.MODE_ECB)

def OracleH(x):
	#return Encryptor.encrypt(x)	# encryption is slower than hashing. Given AES instructions, why?
	return hashlib.sha224(x).digest()

class FeistelPerm:
	'a 3-round Feistel permutation on 2^{n}, n must be even, k is an optional seed'
	def __init__(self, n, s=0):
		if n % 2:
			print("Feistel permutation requires even bit length")
			sys.exit(1)
		random.seed(s)	
		self.n2 = math.ceil(n/2)
		self.n4 = math.ceil(n/4)	
		self.key = [random.getrandbits(self.n2) for i in range(3)]
	
	# PRF_k(x): n -> n bits
	def _PRF(self, x, k):
		x += k << self.n2		
		x = x.to_bytes(self.n4, 'big')
		h = hashlib.md5(x).digest()
		return int.from_bytes(h[:self.n4], 'big')
	
	def perm(self, x):
		n2 = self.n2
		for i in range(3):
			L = x >> n2
			R = x & ((1 << n2)-1)
			x = (R << n2) + self._PRF(R, self.key[i]) ^ L
		return x
		
class SimplePerm:
	'A trivial permutation'
	def __init__(self, N):		
		self.pi = [i for i in range(N)]
		random.shuffle(self.pi)
	
	def perm(self, x):
		return self.pi[x]
		
	def permRange(self, x, d):
		return self.pi[x:x+d]