#include "equihash.h"

void PrintHexDigest(Table &X, int digestSize, string header="")
{
	cout << header << ": len = " << X.size() << endl;
	return;
	for (uint64_t i = 0; i < X.size(); i++)
	{
		cout << "  " << HexDigest(X[i].value, digestSize) << "  (" << (int) X[i].indices[0];
		for (int j = 1; j < X[i].indices.size(); j++)
			cout << ", " << (int) X[i].indices[j];
		cout << ")" << endl;
	}
	cout << endl;
}

bool Duplicated(Indices &y, Indices &x1, Indices &x2)
{
	y.reserve(x1.size() + x2.size());
	y.insert(y.end(), x1.begin(), x1.end());
	y.insert(y.end(), x2.begin(), x2.end());
	sort(y.begin(), y.end());				
	Indices::iterator it = unique(y.begin(), y.end());
	y.resize( distance(y.begin(), it) );
	return y.size() < (x1.size() + x2.size());
}

void FindCollision(Table &Y, Table &X, char left, char right, int digestSize)
{
	Y.clear();
	Y.reserve(X.size());
	SortBySubBytes ByteRange(left, right);
	sort(X.begin(), X.end(), ByteRange);
	PrintHexDigest(X, digestSize, "sorted");

	uint64_t i = 0, j = 0;
	while (i < X.size())
	{
		j = i+1;
		while ( j < X.size() && !ByteRange.bytecmp(X[i].value, X[j].value) )
			j++;
		
		// collision from [i, j)
		for (uint64_t u = i; u < j; u++)
			for (uint64_t v = u+1; v < j; v++)	// insert X[k], X[l]
			{	
				Indices indy;
				if (!Duplicated(indy, X[u].indices, X[v].indices))
					Y.emplace_back(X[u], X[v], indy);				
			}
		i = j;
	}
	PrintHexDigest(Y, digestSize, "collide");
}

void Wagner(int nBytes, int kStep, int N)
{
	RandomOracle H(nBytes);	

	byte** input = new byte* [1];
	input[0] = new byte [nBytes];

	Table X, Y;

	X.resize(N);
	for (uint64_t j = 0; j < X.size(); j++)
	{
		X[j].size = nBytes;
		X[j].value = new byte [nBytes];
		int2bytes(input[0], j, nBytes);
		H.Digest(X[j].value, input, 1);
		X[j].indices.push_back(j);
	}
	PrintHexDigest(X, nBytes, "initial");
	
	for (int k = 0; k < nBytes-2*kStep; k += kStep)
	{
		FindCollision(Y, X, k, k+kStep, nBytes);
		swap(X, Y);
		for (uint64_t j = 0; j < Y.size(); j++)
			delete[] Y[j].value;
		//for_each(Y.begin(), Y.end(), default_delete<TableEntry>());
	}
	FindCollision(Y, X, nBytes-2*kStep, nBytes, nBytes);
	return;
}


int main()
{
	int nBytes = 24;
	int kStep = 2;
	int N = 2 << (kStep * 8);
	Wagner(nBytes, kStep, N);
	return 0;
}


