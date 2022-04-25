#include <cstdint>
#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <bitset>
#include <sstream>
#include <stdio.h>
#include <unordered_map>
#include <list>
#include <algorithm>
#ifndef CACHE_H
#define CACHE_H

using namespace std;

namespace G
{
	class Cache
	{

		typedef bitset<32> bits;

		struct memory_address
		{
			bits mem;
			bits tag;
			bits index;
			bits offset;
		};

		typedef memory_address *entry;
		typedef bits tag_entry;

	private:
		// input parameters
		int capacity;
		int block_size;
		int associativity;
		int replacement_policy;

		// calculations
		int blocks;
		int sets;
		int offset;
		int index;
		int tag;
		int memory_length;

		// simulated values
		int hits;
		int misses;
		int compulsory;
		int conflict;
		int capacity_miss;
		int write_miss;
		int write_hit;
		int read_miss;
		int read_hit;
		int write_conflict;
		int read_conflict;
		int write_capacity;
		int read_capacity;
		int write_compulsory;
		int read_compulsory;

		vector<bool> readorwrite;
		vector<list<tag_entry>> tagCache;
		vector<list<tag_entry>::iterator> tagIter;

		// memory storage
		vector<bits> memory;			// store the entire input trace file
		memory_address *memory_splited; // array of structures of indexed memory address

		// other functions
		int bitsToInt(bits &h);
		bits getTag(bits &h);
		bits getIndex(bits &h);
		bits getOffset(bits &h);
		bits hexToBin(string &s);

		void createCache();

		// replacement and access functions
		void LRU(memory_address &adr, bool wr);
		void FIFO(memory_address &adr, bool wr);
		void LIFO(memory_address &adr, bool wr);

	public:
		// constructor and deconstructor
		Cache(int &C, int &b, int &N, int &R);
		~Cache();

		void readMemory(string trace_filename);
		void testMem();
	};
}
#endif