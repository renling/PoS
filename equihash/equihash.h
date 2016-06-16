#include <iostream>
#include <vector>
#include <algorithm>
#include <memory.h>
#include "util.h"

using namespace std;

typedef vector<int> Indices; 

class TableEntry
{
public:
	TableEntry() {}
	TableEntry(byte* val, vector<int> &ind)
	{
		value = val;
		indices.insert(indices.begin(), ind.begin(), ind.end());
	}

	TableEntry(TableEntry &x1, TableEntry &x2, Indices indy)	// the direct merge constructor
	{
		value = new byte [x1.size];
		size = x1.size;
		CryptoPP::xorbuf(value, x1.value, x2.value, size);
		indices.insert(indices.end(), indy.begin(), indy.end());				
	}

	byte* value;
	int size;
	Indices indices;
};

typedef vector<TableEntry> Table;


class SortBySubBytes
{
public:
	SortBySubBytes(char left, char right)
	{
		if (right < left)
		{
			cout << "[SortBySubBytes]: invalid byte range!" << endl;
			exit(1);
		}
		this->left = left;	
		this->right = right;
	}

	bool operator() (TableEntry x, TableEntry y) { return bytecmp(x.value, y.value) < 0; }	// cannot use <=, need strict ordering

	int bytecmp(byte* x, byte* y) { return memcmp(x + left, y + left, right - left); }

private:
	char left, right;
};
 

