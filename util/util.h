#ifndef PEBBLE_UTIL
#define PEBBLE_UTIL

#include <iostream>
#include <stdint.h>
#include <sha.h>
#include <sha3.h>
#include <hex.h>
#include <vector>
#include <memory>
#include <omp.h>
using namespace std;


class RandomOracle
{
public:
	const static int DigestSize = 32;
	RandomOracle() { memset(nonce, 0, sizeof(byte)*DigestSize); }

	void Digest(byte output[DigestSize], const byte* const input [DigestSize], int nInput)
	{
		//for (int i = 0; i < nInput; i++)
		//	memcpy(output, input[i], DigestSize);
		//return;	
		hash.Update(nonce, DigestSize);
		for (int i = 0; i < nInput; i++)
			hash.Update(input[i], DigestSize);
		hash.TruncatedFinal(output, DigestSize);
		return;
	}
	
	string HexDigest(const byte digest[DigestSize])
	{
		string encoded;
		
		encoder.Initialize();
		encoder.Put(digest, DigestSize);
		encoder.MessageEnd();		
		int size = encoder.MaxRetrievable();
		if(size)
		{
			encoded.resize(size);		
			encoder.Get((byte*)encoded.data(), encoded.size());
		}		
		return encoded;
	}
	
private:	
	CryptoPP::SHA3_256 hash;
	CryptoPP::HexEncoder encoder;
	byte nonce[DigestSize];
};

class SimplePerm
{
public:
	SimplePerm(uint64_t N) 
	{
		if (N >= (1 << 31))
		{
			std::cout << "[SimplePerm] Cannot handle more than 2^31 elements!\n";
			exit(1);
		}
		Pi.resize(N);
		for (uint64_t i = 0; i < N; i++)
			Pi[i] = i;
		srand(0);	
		for (uint64_t i = N-1; i > 0; i--)
		{
			uint64_t j = rand() % (i+1);
			swap(Pi[i], Pi[j]);
		}			
	}

	uint64_t perm(uint64_t x) { return Pi[x]; }
	vector<uint64_t>::iterator permIdx(uint64_t x) { return Pi.begin() + x; }	// so that the caller can get the next a few values, proabably unnecessary

private:
	vector<uint64_t> Pi;
};
#endif
