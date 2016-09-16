#include "equihash.h"

void PrintTable(Table &X, string header="", bool list=false)
{
	cout << header << ": len = " << X.size() << endl;
	if (!list)	return;
	for (uint64_t i = 0; i < X.size(); i++)
	{
		if (X[i].size > 0)
			cout << "  " << HexDigest(X[i].value, X[i].size);
		cout << "  (" << (int) X[i].indices->at(0);
		for (int j = 1; j < X[i].indices->size(); j++)
			cout << ", " << (int) X[i].indices->at(j);
		cout << ")" << endl;
	}
	cout << endl;
}

bool Duplicated(Indices &y, Indices &x1, Indices &x2)
{	
	merge(x1.begin(), x1.end(), x2.begin(), x2.end(), back_inserter(y));
	return false;	// if no duplication check
	return adjacent_find( y.begin(), y.end() ) != y.end();	// check for adjacent duplicate
}

void FindCollision(Table &Y, Table &X, char step)
{
	Y.clear();
	Y.reserve(X.size());
	SortBySubBytes ByteRange(step);
	sort(X.begin(), X.end(), ByteRange);
	PrintTable(X, "sorted");

	uint64_t i = 0, j = 0;
	while (i < X.size())
	{
		j = i+1;
		while ( j < X.size() && !ByteRange.bytecmp(X[i].value, X[j].value) )
			j++;
		
		// collision from [i, j)
		for (uint64_t u = i; u < j; u++)
		{
			for (uint64_t v = u+1; v < j; v++)	// add (X[k], X[l])
			{	
				Indices indy;
				if (!Duplicated(indy, *(X[u].indices), *(X[v].indices) ))
					Y.push_back( MergeEntry(X[u], X[v], indy, step) );				
			}
			delete X[u].value;	// we can free Table X up to row j now
			delete X[u].indices;
		}
		
		i = j;
	}
	PrintTable(Y, "collide");
}

int WrongWagner(char nBytes, char kStep, int N, int64_t seed=0)
{
	RandomOracle H(nBytes, seed);	

	byte** input = new byte* [1];
	input[0] = new byte [nBytes];

	Table X, Y;
	X.resize(N);
	for (int j = 0; j < X.size(); j++)
	{
		X[j].size = nBytes;
		X[j].value = new byte [nBytes];
		int2bytes(input[0], j, nBytes);
		H.Digest(X[j].value, input, 1);
		X[j].indices = new Indices;
		X[j].indices->push_back(j);
	}
	PrintTable(X, "initial");
	
	for (char k = 0; k < nBytes-2*kStep && X.size() > 0; k += kStep)
	{
		FindCollision(Y, X, kStep);
		swap(X, Y);
	}
	FindCollision(Y, X, 2*kStep);
	PrintTable(Y, "final", true);
	cout << "# of sol = " << Y.size() << endl; 
	return Y.size();
}


int main(int argc, char* argv[])
{
	char nBytes = 7;
	char kStep = 1;
	if (argc == 3)
	{
		nBytes = atoi(argv[1]);
		kStep = atoi(argv[2]);
	}
	if (nBytes % kStep)
		cout << "Error: kStep does not divide nBytes!" << endl;
	int N = 2 << (kStep * 8);

	int nTest = 10, nSol = 0;
	clock_t start = clock(), diff;
	for (int i = 0; i < nTest; i++)
	{
		nSol += WrongWagner(nBytes, kStep, N, i);
	}	
	diff = clock() - start;
	cout << "Total # of sol = " << nSol << ", total time = " << diff / 1000000.0 << endl;
	return 0;
}


