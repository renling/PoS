#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <memory.h>
#include <ctime>
#include "util.h"

using namespace std;

typedef vector<int> Indices; 

struct TableEntry
{
	byte* value;
	Indices* indices;
	char size;
};

TableEntry MergeEntry(TableEntry &x1, TableEntry &x2, Indices indy, char step)
{
	TableEntry y;
	if ((y.size = x1.size-step) > 0)
	{
		y.value = new byte [x1.size];			
		CryptoPP::xorbuf(y.value, x1.value+step, x2.value+step, y.size);			
	}
	y.indices = new Indices(indy);		
	return y;
}

typedef vector<TableEntry> Table;


class SortBySubBytes
{
public:
	SortBySubBytes(char step) {  this->step = step;	}
	
	bool operator() (TableEntry x, TableEntry y) { return bytecmp(x.value, y.value) < 0; }	// cannot use <=, need strict ordering

	int bytecmp(byte* x, byte* y) { return memcmp(x, y, step); }

private:
	char step;
};
 

