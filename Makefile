all : src/test.cpp src/json.hpp
	g++ --std=c++0x -Wall -pedantic -Isrc/ -o bin/test src/test.cpp

clean :
