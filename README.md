To run the programme all the 4 files Cache.cpp, Cache.h and Main.cpp should be in one directory. To compile these file run the below command in that directory terminal

$ g++ -std=c++11 Cache.cpp main.cpp  -o hw

Now we have an executable file hw. To run hw we need to give trace file name, capacity in bits, block size in bits, associativity, replacement policy input data as command line arguments in the terminal.
For replacement policy if you want to test LRU input 0, for FIFO input 1, and for LIFO input 2.
Memory sizes are in bytes.
For example

$ ./hw "example.txt" 2048 64 4 2