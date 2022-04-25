#include "iomanip"
#include "Cache.h"
#define nullptr NULL

namespace G
{
	typedef bitset<32> bits;

	Cache::Cache(int &C, int &b, int &N, int &R)
	{
		capacity = C;
		block_size = b;
		associativity = N;
		replacement_policy = R;
		compulsory = 0;
		conflict = 0;
		capacity_miss = 0;
		write_miss = 0;
		write_hit = 0;
		read_miss = 0;
		read_hit = 0;
		write_conflict = 0;
		read_conflict = 0;
		write_capacity = 0;
		read_capacity = 0;
		write_compulsory = 0;
		read_compulsory = 0;

		createCache();

		cout << "Capacity: " << C << endl;
		cout << "Block Size: " << b << endl;
		cout << "Associativity: " << N << endl;
		cout << "Total blocks: " << blocks << endl;
		cout << "Total sets: " << sets << endl;
		cout << "Offset bits: " << offset << endl;
		cout << "Index bits: " << index << endl;
		cout << "Tag bits: " << tag << endl;
		cout << "Replacement policy: " << replacement_policy << endl;
	}

	Cache::~Cache()
	{
		free(memory_splited);
	}

	void Cache::testMem()
	{
		memory_splited = (memory_address *)malloc(sizeof(memory_address) * memory_length);

		for (int i = 0; i < memory_length; i++)
		{
			memory_splited[i].mem = memory.at(i);
			memory_splited[i].tag = getTag(memory.at(i));
			memory_splited[i].index = getIndex(memory.at(i));
			memory_splited[i].offset = getOffset(memory.at(i));
		}

		// cache operation begins here
		int i = 0;
		for (; i < memory_length; i++)
		{
			if (replacement_policy == 0)
			{
				LRU(memory_splited[i], readorwrite[i]);
			}
			else if (replacement_policy == 1)
			{
				FIFO(memory_splited[i], readorwrite[i]);
			}
			else
			{
				LIFO(memory_splited[i], readorwrite[i]);
			}
		}

		// displaying the simulated values
		cout << "" << endl;
		cout << "NUM ACCESSES: " << i << endl;
		cout << "" << endl;

		cout << "HIT RATE: " << (double)(hits) / (double)(i) << endl;

		cout << "-----"
			 << "HIT"
			 << "-----" << endl;
		cout << left << setw(15) << "READ" << left << setw(15) << read_hit << endl;
		cout << left << setw(15) << "WRITE" << left << setw(15) << write_hit << endl;
		cout << left << setw(15) << "TOTAL" << left << setw(15) << hits << endl;

		cout << "" << endl;
		cout << "-----"
			 << "MISSES"
			 << "-----" << endl;
		cout << left << setw(15) << "" << left << setw(15) << "COMPULSORY" << left << setw(15) << "CONFLICT" << left << setw(15) << "CAPACITY" << left << setw(15) << "TOTAL" << endl;
		cout << left << setw(15) << "READ" << left << setw(15) << read_compulsory << left << setw(15) << read_conflict << left << setw(15) << read_capacity << left << setw(15) << read_miss << endl;
		cout << left << setw(15) << "WRITE" << left << setw(15) << write_compulsory << left << setw(15) << write_conflict << left << setw(15) << write_capacity << left << setw(15) << write_miss << endl;
		cout << left << setw(15) << "TOTAL" << left << setw(15) << compulsory << left << setw(15) << conflict << left << setw(15) << capacity_miss << left << setw(15) << misses << endl;
	}

	void Cache::LRU(memory_address &adr, bool wr)

	{
		int currentIndex = bitsToInt(adr.index); // current set index
		list<tag_entry> *currentSet = &tagCache.at(currentIndex);
		list<tag_entry>::iterator *currentIt = &tagIter.at(currentIndex);

		*currentIt = find(currentSet->begin(), currentSet->end(), adr.tag); // finding the tag in the current set

		// CACHE FULL CASE
		if (currentSet->size() == associativity)
		{
			// CACHE MISS
			if ((*currentIt == currentSet->end()))
			{
				misses++;
				if (wr)
				{
					write_miss += 1;
				}
				else
				{
					read_miss += 1;
				}

				*currentIt = currentSet->begin(); // set iterator pointer to LRU
				currentSet->erase(*currentIt);	  // erase LRU
				currentSet->push_back(adr.tag);	  // push back the tag as the new MRU

				conflict += 1;
				if (wr)
				{
					write_conflict += 1;
				}
				else
				{
					read_conflict += 1;
				}

				int totsize = tagCache.size();
				bool che = true;
				// checks if any of the block is empty
				for (int a1 = 0; a1 < totsize; a1++)
				{
					if (tagCache[a1].size() != associativity)
					{
						che = false;
					}
				}
				if (che)
				{
					capacity_miss += 1;
					if (wr)
					{
						write_capacity += 1;
					}
					else
					{
						read_capacity += 1;
					}
				}
			}

			// CACHE HIT
			else
			{
				hits++;
				if (wr)
				{
					write_hit += 1;
				}
				else
				{
					read_hit += 1;
				}

				currentSet->erase(*currentIt);
				currentSet->push_back(adr.tag);
			}
		}
		// CACHE NOT FULL CASE
		else if ((currentSet->size() != 0) && (currentSet->size() < associativity))
		{
			// CACHE MISS
			if ((*currentIt == currentSet->end()))
			{
				misses++;
				if (wr)
				{
					write_miss += 1;
				}
				else
				{
					read_miss += 1;
				}

				currentSet->push_back(adr.tag);

				compulsory += 1;
				if (wr)
				{
					write_compulsory += 1;
				}
				else
				{
					read_compulsory += 1;
				}
			}
			// CACHE HIT
			else
			{
				hits++;

				if (wr)
				{
					write_hit += 1;
				}
				else
				{
					read_hit += 1;
				}

				currentSet->erase(*currentIt);
				currentSet->push_back(adr.tag);
			}
		}
		// CACHE NEW CASE
		else if ((currentSet->size() == 0) && (*currentIt == currentSet->end()))
		{
			misses++;
			if (wr)
			{
				write_miss += 1;
			}
			else
			{
				read_miss += 1;
			}

			currentSet->push_back(adr.tag);

			compulsory += 1;
			if (wr)
			{
				write_compulsory += 1;
			}
			else
			{
				read_compulsory += 1;
			}
		}

		currentSet = nullptr;
		currentIt = nullptr;
		delete currentSet;
		delete currentIt;
	}

	void Cache::FIFO(memory_address &adr, bool wr)

	{
		int currentIndex = bitsToInt(adr.index); // current set index
		list<tag_entry> *currentSet = &tagCache.at(currentIndex);
		list<tag_entry>::iterator *currentIt = &tagIter.at(currentIndex);

		*currentIt = find(currentSet->begin(), currentSet->end(), adr.tag);

		// CACHE FULL CASE
		if (currentSet->size() == associativity)
		{
			// CACHE MISS
			if ((*currentIt == currentSet->end()))
			{
				misses++;
				if (wr)
				{
					write_miss += 1;
				}
				else
				{
					read_miss += 1;
				}

				*currentIt = currentSet->begin();
				currentSet->erase(*currentIt);
				currentSet->push_back(adr.tag);

				conflict += 1;
				if (wr)
				{
					write_conflict += 1;
				}
				else
				{
					read_conflict += 1;
				}
				int totsize = tagCache.size();
				bool che = true;
				for (int a1 = 0; a1 < totsize; a1++)
				{
					if (tagCache[a1].size() != associativity)
					{
						che = false;
					}
				}
				if (che)
				{
					capacity_miss += 1;
					if (wr)
					{
						write_capacity += 1;
					}
					else
					{
						read_capacity += 1;
					}
				}
			}

			// CACHE HIT
			else
			{
				hits++;
				if (wr)
				{
					write_hit += 1;
				}
				else
				{
					read_hit += 1;
				}
			}
		}
		// CACHE NOT FULL CASE
		else if ((currentSet->size() != 0) && (currentSet->size() < associativity))
		{
			// CACHE MISS
			if ((*currentIt == currentSet->end()))
			{
				misses++;
				if (wr)
				{
					write_miss += 1;
				}
				else
				{
					read_miss += 1;
				}

				currentSet->push_back(adr.tag);

				compulsory += 1;
				if (wr)
				{
					write_compulsory += 1;
				}
				else
				{
					read_compulsory += 1;
				}
			}
			// CACHE HIT
			else
			{
				hits++;
				if (wr)
				{
					write_hit += 1;
				}
				else
				{
					read_hit += 1;
				}
			}
		}
		// CACHE NEW CASE
		else if ((currentSet->size() == 0) && (*currentIt == currentSet->end()))
		{
			misses++;
			if (wr)
			{
				write_miss += 1;
			}
			else
			{
				read_miss += 1;
			}

			currentSet->push_back(adr.tag);

			compulsory += 1;
			if (wr)
			{
				write_compulsory += 1;
			}
			else
			{
				read_compulsory += 1;
			}
		}

		currentSet = nullptr;
		currentIt = nullptr;
		delete currentSet;
		delete currentIt;
	}

	void Cache::LIFO(memory_address &adr, bool wr)

	{
		int currentIndex = bitsToInt(adr.index); // current set index
		list<tag_entry> *currentSet = &tagCache.at(currentIndex);
		list<tag_entry>::iterator *currentIt = &tagIter.at(currentIndex);

		*currentIt = find(currentSet->begin(), currentSet->end(), adr.tag);

		// CACHE FULL CASE
		if (currentSet->size() == associativity)
		{
			// CACHE MISS
			if ((*currentIt == currentSet->end()))
			{
				misses++;
				if (wr)
				{
					write_miss += 1;
				}
				else
				{
					read_miss += 1;
				}

				advance(*currentIt, distance(*currentIt, currentSet->end()) - 1);
				currentSet->erase(*currentIt);
				currentSet->push_back(adr.tag);

				conflict += 1;
				if (wr)
				{
					write_conflict += 1;
				}
				else
				{
					read_conflict += 1;
				}
				int totsize = tagCache.size();
				bool che = true;
				for (int a1 = 0; a1 < totsize; a1++)
				{
					if (tagCache[a1].size() != associativity)
					{
						che = false;
					}
				}
				if (che)
				{
					capacity_miss += 1;
					if (wr)
					{
						write_capacity += 1;
					}
					else
					{
						read_capacity += 1;
					}
				}
			}

			// CACHE HIT
			else
			{
				hits++;
				if (wr)
				{
					write_hit += 1;
				}
				else
				{
					read_hit += 1;
				}
			}
		}
		// CACHE NOT FULL CASE
		else if ((currentSet->size() != 0) && (currentSet->size() < associativity))
		{
			// CACHE MISS
			if ((*currentIt == currentSet->end()))
			{
				misses++;
				if (wr)
				{
					write_miss += 1;
				}
				else
				{
					read_miss += 1;
				}

				currentSet->push_back(adr.tag);

				compulsory += 1;
				if (wr)
				{
					write_compulsory += 1;
				}
				else
				{
					read_compulsory += 1;
				}
			}
			// CACHE HIT
			else
			{
				hits++;
				if (wr)
				{
					write_hit += 1;
				}
				else
				{
					read_hit += 1;
				}
			}
		}
		// CACHE NEW CASE
		else if ((currentSet->size() == 0) && (*currentIt == currentSet->end()))
		{
			misses++;
			if (wr)
			{
				write_miss += 1;
			}
			else
			{
				read_miss += 1;
			}

			currentSet->push_back(adr.tag);


			compulsory += 1;
			if (wr)
			{
				write_compulsory += 1;
			}
			else
			{
				read_compulsory += 1;
			}
		}

		currentSet = nullptr;
		currentIt = nullptr;
		delete currentSet;
		delete currentIt;
	}

	void Cache::createCache()
	{
		blocks = capacity / block_size;
		sets = blocks / associativity;
		offset = log(block_size) / log(2);
		index = log(sets) / log(2);
		tag = 32 - offset - index;

		hits = 0;
		misses = 0;

		tagCache.resize(sets);
		tagIter.resize(sets);

		for (int i = 0; i < sets; i++)
		{
			tagIter.at(i) = tagCache.at(i).begin();
		}
	}

	void Cache::readMemory(string trace_filename)
	{
		ifstream file;
		string line;

		file.open(trace_filename, ios::in);

		int j = 0;
		while (!file.eof())
		{
			getline(file, line);

			// breaking the loop when only /n is left in the line
			if (line.length() < 2)
			{
				break;
			}
			if (line[0] == 'W')
			{
				readorwrite.push_back(true);
			}
			else
			{
				readorwrite.push_back(false);
			}

			string line2 = line.substr(2, 11);
			memory.push_back(hexToBin(line2));

			j++;
		}

		cout << "Total " << j << " memory addresses are read from " << trace_filename << endl;
		memory_length = j;
		file.close();
	}

	// convert hex address to bitset<32>
	bits Cache::hexToBin(string &s)
	{
		stringstream ss;
		unsigned int n;

		ss << hex << s;
		ss >> n;
		bitset<32> b(n);
		return b;
	}

	// returns the tag from memory address
	bits Cache::getTag(bits &h)
	{
		bitset<32> b(h.to_string(), 0, tag);
		return b;
	}

	bits Cache::getIndex(bits &h)
	{
		bitset<32> b(h.to_string(), tag, index);
		return b;
	}

	bits Cache::getOffset(bits &h)
	{
		bitset<32> b(h.to_string(), index + tag, 31);
		return b;
	}

	// returns the integer value
	int Cache::bitsToInt(bits &h)
	{
		int b = stoi(h.to_string(), nullptr, 2);
		return b;
	}
}
