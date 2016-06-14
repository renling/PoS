#include <iostream>
#include <vector>
#include <memory.h>
#include "util.h"
using namespace std;

class SortBySubBytes
{
public:
	SortBySubBytes(char left, char right)
	{
		if (right < left)
		{
			cout << "[SortByBits]: invalid bit range!" << endl;
			exit(1);
		}
		this->left = left;	
		this->right = right;
	}

	bool operator() (byte* x, byte* y) 
	{ return memcmp(x + left, y + left, right - left) < 0; }

private:
	char left, right;
};

void Wagner()
{
	int nBytes = 4;
	int kSum = 4;
	int N = 1 << 9;
	RandomOracle H(nBytes);	

	byte** input = new byte* [1];
	input[0] = new byte [H.GetDigestSize()];

	vector<byte*> X;
	X.resize(N);
	for (uint64_t j = 0; j < N; j++)
	{
		X[j] = new byte [H.GetDigestSize()];
		int2bytes(input[0], j, H.GetDigestSize());
		H.Digest(X[j], input, 1);
	}
	H.PrintHexDigest(X);
	
	sort(X.begin(), X.end(), SortBySubBytes(0, 1));
	H.PrintHexDigest(X);
	
	vector<byte*> Y;
}


int main()
{
	Wagner();
	return 0;
}


