CXXFLAGS+=-std=c++11 -Wall -Ilib/

default : bin/test

bin/test : bin lib/json.hpp src/test.cpp
	$(CXX) $(CXXFLAGS) -o bin/test src/test.cpp

bin :
	mkdir -p bin

clean :
	rm -rf bin
