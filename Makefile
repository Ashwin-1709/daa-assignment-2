CXX = g++
CXXFLAGS = -Wall -g -std=c++17 #-Wshadow -Wconversion -Wpedantic -Wno-float-equal -Wno-variadic-macros

all: bipartite flow segmented

bipartite: src/algorithm.hpp src/algorithm.cpp src/ford-fulkerson-bipartite.cpp
	$(CXX) $(CXXFLAGS) src/algorithm.cpp src/ford-fulkerson-bipartite.cpp -o $@

flow: src/algorithm.hpp src/algorithm.cpp src/ford-fulkerson-flow.cpp
	$(CXX) $(CXXFLAGS) src/algorithm.cpp src/ford-fulkerson-flow.cpp -o $@

segmented: src/segmented-least-squares.cpp
	$(CXX) $(CXXFLAGS) src/segmented-least-squares.cpp -o $@

clean:
	rm -rf bipartite flow segmented
