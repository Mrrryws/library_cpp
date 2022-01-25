all: prog

# executable deps and command to build it:
prog: test-suite.o library.o catch.o
	g++ test-suite.o library.o catch.o -o prog

# library.o deps and command to build it:
library.o: library.cpp library.h
	g++ -c library.cpp -o library.o

# catch.o deps and command to build it:
catch.o: catch.cpp catch.h
	g++ -c catch.cpp -o catch.o

# test-suite.o deps and command to build it:
test-suite.o: test-suite.cpp catch.h library.h
	g++ -c test-suite.cpp -o test-suite.o
