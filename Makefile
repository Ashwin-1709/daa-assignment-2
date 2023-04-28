CXX = g++
CXXFLAGS = -Wall -Wshadow -Wconversion -Wpedantic -Wno-float-equal -Wno-variadic-macros

all: ford segmented

ford: src/algorithm.hpp src/algorithm.cpp src/ford-fulkerson-bipartite.cpp src/ford-fulkerson-flow.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) src/algorithm.cpp src/ford-fulkerson-bipartite.cpp -o bin/ford-fulkerson-bipartite
	$(CXX) $(CXXFLAGS) src/algorithm.cpp src/ford-fulkerson-flow.cpp -o bin/ford-fulkerson-flow

segmented: src/segmented-least-squares.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) src/segmented-least-squares.cpp -o bin/segmented-least-squares

clean:
	rm -rf bin
