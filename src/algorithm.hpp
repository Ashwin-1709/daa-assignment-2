/** \file */
#ifndef ALGORITHM_H
#define ALGORITHM_H
#include "dbg.hpp"
#include "typedefs.hpp"
#include <array>
#include <set>
#include <vector>

const i64 MAX_FLOW = INT64_MAX;

/// @brief Finds the maximum flow along the current path
/// @param source Source node
/// @param sink Destination node
/// @param residualCapacity Matrix storing the residual capacities between nodes
/// @param adj Graph represented by adjacency list
/// @return A pair containing a vector of parents and the current maximum flow
auto find_simple_path(i64 source, i64 sink,
                      std::vector<std::vector<i64>> &residualCapacity,
                      std::vector<std::vector<i64>> &adj)
    -> std::pair<std::vector<i64>, i64>;

/// @brief Updates the residual capacty matrix using best_flow
/// @param parent std::vector of parent nodes
/// @param best_flow Maximum Flow along a path
/// @param s Source Node
/// @param t Destination Node
/// @param residualCapacity Matrix storing the residual capacities between nodes
/// @param forwardEdges Set of forward edges in the graph
auto augment_path(std::vector<i64> parent, i64 best_flow, i64 s, i64 t,
                  std::vector<std::vector<i64>> &residualCapacity,
                  std::set<std::array<i64, 2>> &forwardEdges) -> void;

/// @brief Finds the maximum flow in a flow network
/// @param s Source Node
/// @param t Destination Node
/// @param residualCapacity Matrix storing the residual capacities between nodes
/// @param adj Graph represented by adjacency list
/// @param forwardEdges Set of forward edges in the graph
/// @return Maximum flow from source node to destination node
auto findFlow(i64 s, i64 t, std::vector<std::vector<i64>> &residualCapacity,
              std::vector<std::vector<i64>> &adj,
              std::set<std::array<i64, 2>> &forwardEdges) -> i64;

/// @brief Finds the min cut of given graph
/// @param s Source Node
/// @param t Destination Node
/// @param residualCapacity Matrix storing the residual capacities between nodes
/// @param adj Graph represented by adjacency list
/// @param forwardEdges Set of forward edges in the graph
/// @return Min Cut of the graph
auto minCut(i64 s, i64 t, std::vector<std::vector<i64>> &residualCapacity,
            std::vector<std::vector<i64>> &adj,
            std::set<std::array<i64, 2>> &forwardEdges)
    -> std::set<std::array<i64, 2>>;

#endif
