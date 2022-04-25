#include <iostream>
#include <stdint.h>
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <cstring>
#include <string>
#include "Cache.h"
#define nullptr NULL

using namespace G;
using namespace std;

int main(int argc, char **argv)
{
	// sequence of arguments -- "trace filename" "__cacheSize__"  "__blockSize__"  "__associativity__" "__replacementPolicy__"
	string trace_filename = argv[1];

	int capacity, block_size, associativity, replacement_policy;
	capacity = atoi(argv[2]);
	block_size = atoi(argv[3]);
	associativity = atoi(argv[4]);
	replacement_policy = atoi(argv[5]);

	// instantiate the cache according to the input config file
	//Memory sizes are in bytes
	Cache cache1(capacity, block_size, associativity, replacement_policy); //Cache(capacity, block_size, associativity, replacement_policy);
	cache1.readMemory(trace_filename);
	cache1.testMem();

	return 0;
}