#include <iostream>
#include <util.h>
#include <ctime>
using namespace std;

inline void int2bytes(byte* dst, uint64_t src)
{
	*((uint64_t*) dst) = src;
}

void PebbleExpanderStack(int logn, int d, int k)
{
	uint64_t n = (uint64_t) 1 << logn;
	
	RandomOracle H;	
	byte** Vs = new byte* [n];
	byte** Vt = new byte* [n];
	for (uint64_t j = 0; j < n; j++)
	{
		Vs[j] = new byte [H.DigestSize];
		Vt[j] = new byte [H.DigestSize];
	}		
	byte** input = new byte* [d+1];
	for (int i = 0; i < d+1; i++)
		input[i] = new byte [H.DigestSize];
	
	
	SimplePerm Pi(n*d); // Precomputing the predecessors is unnecessary for C
	
	clock_t start = clock();
	// pebble the input layer
	cout << "pebbling layer 0" << endl;
	for (uint64_t j = 0; j < n; j++)
	{
		int2bytes(input[0], j);
		H.Digest(Vt[j], input, 1);	// the only input is j
		//cout << '\t' << H.HexDigest(Vt[j]) << endl;
	}
	
	// pebble layer by layer
	for (int i = 0; i < k; i++)
	{
		cout << "pebbling layer " << i+1 << endl;
		swap(Vs, Vt);
		for (uint64_t j = 0; j < n; j++)
		{
			int2bytes(input[0], j);			
			for (int l = 0; l < d; l++)
			{
				uint64_t parent = Pi.perm(j*d+l) % n;
				input[l+1] = Vs[parent];
			}
			H.Digest(Vt[j], input, d+1);
			//cout << '\t' << H.HexDigest(Vt[j]) << endl;
		}
	}
	clock_t end = clock();
	cout << "total time: " << (end-start) / (double) CLOCKS_PER_SEC << endl;
}


int main()
{
	PebbleExpanderStack(22, 16, 10);	
	return 0;
}
