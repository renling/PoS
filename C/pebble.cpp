#include <iostream>
#include <util.h>
#include <ctime>
#include <memory>
#include <omp.h>
using namespace std;

inline void int2bytes(byte* dst, uint64_t src, int nbytes)
{
	memset(dst, 0, sizeof(byte)*nbytes);
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
	input[0] = new byte [H.DigestSize];
		
	SimplePerm Pi(n*d); // Precomputing the predecessors is unnecessary for C
	
	
	// pebble the input layer
	cout << "pebbling layer 0" << endl;
	for (uint64_t j = 0; j < n; j++)
	{
		int2bytes(input[0], j, H.DigestSize);
		H.Digest(Vt[j], input, 1);	// the only input is j		
	}
	//cout << '\t' << H.HexDigest(Vt[n-1]) << endl;
	
	double tstart = omp_get_wtime();
	// pebble layer by layer
	for (int i = 0; i < k; i++)
	{
		cout << "pebbling layer " << i+1 << endl;
		swap(Vs, Vt);
				
		#pragma omp parallel
	  	{
	  		int thread_id = omp_get_thread_num(), nloops = 0;
			RandomOracle Hp;		  			
			byte** inputp = new byte* [d+1];
			inputp[0] = new byte [Hp.DigestSize];	
			
			#pragma omp for 
			for (uint64_t j = 0; j < n; j++)
			{
				nloops++;
				int2bytes(inputp[0], j, Hp.DigestSize);			
				for (int l = 0; l < d; l++)
				{
					uint64_t parent = Pi.perm(j*d+l) % n;
					inputp[l+1] = Vs[parent];
				}
				Hp.Digest(Vt[j], inputp, d+1);				
			}
			delete[] inputp[0];
			delete inputp; 
			//cout << "Thread " << thread_id << " performed " << nloops << " iterations\n";
		}		
	}
	cout << '\t' << H.HexDigest(Vt[n-1]) << endl;
	double tend = omp_get_wtime();
	cout << "total time: " << ((double)tend-tstart) << endl;
}

void PebbleButterflyStack(int logn, int k)
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
	byte** input = new byte* [3];
	input[0] = new byte [H.DigestSize];
	
	int ButDepth = 2*logn-1;
	int* ButLayer = new int [ButDepth];
	for (int i = 0; i < logn; i++)
		ButLayer[i] = ButLayer[ButDepth-1-i] = logn-1-i;
						
	
	// pebble the input layer
	cout << "pebbling layer 0" << endl;
	for (uint64_t j = 0; j < n; j++)
	{
		int2bytes(input[0], j, H.DigestSize);
		H.Digest(Vt[j], input, 1);	// the only input is j
	}
	
	double tstart = omp_get_wtime();
	// pebble layer by layer
	for (int i = 0; i < k; i++)
	{
		cout << "pebbling layer " << i+1 << endl;		
		for (int l = 0; l < ButDepth; l++)
		{
			int d = ButLayer[l];
			swap(Vs, Vt);
			
			#pragma omp parallel
	  		{
				RandomOracle Hp;		  			
				byte** inputp = new byte* [d+1];
				inputp[0] = new byte [Hp.DigestSize];	
			
				#pragma omp for 	
				for (uint64_t j = 0; j < n; j++)
				{
					int2bytes(inputp[0], j, H.DigestSize);	
					inputp[1] = Vs[j];
					uint64_t j2 = j;
					j2 -= (1<<d) * ((j%(2<<d)) >= (1<<d) ? 1 : -1);			
					inputp[2] = Vs[j2];					
					Hp.Digest(Vt[j], inputp, 3);
				}
				delete[] inputp[0];
				delete inputp; 
			}	
		}				
	}
	cout << '\t' << H.HexDigest(Vt[n-1]) << endl;
	double tend = omp_get_wtime();
	cout << "total time: " << ((double)tend-tstart) << endl;
}


int main()
{
	PebbleExpanderStack(20, 100, 10);
	PebbleButterflyStack(20, 10);
	return 0;
}
