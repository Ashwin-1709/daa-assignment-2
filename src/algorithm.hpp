/** \file */
#ifndef ALGORITHM_H
#define ALGORITHM_H
#include "dbg.hpp"
#include "typedefs.hpp"
#include <array>
#include <optional>
#include <set>
#include <vector>

const u64 MAX_FLOW = UINT64_MAX;

/// @brief Finds the maximum flow along the current path
/// @param source Source node
/// @param sink Destination node
/// @param residualCapacity Matrix storing the residual capacities between nodes
/// @param adj Graph represented by adjacency list
/// @return A pair containing a vector of parents and the current maximum flow
auto find_simple_path(u64 source, u64 sink,
                      std::vector<std::vector<u64>> &residualCapacity,
                      std::vector<std::vector<u64>> &adj)
    -> std::pair<std::vector<std::optional<u64>>, u64>;

/// @brief Updates the residual capacty matrix using best_flow
/// @param parent std::vector of parent nodes
/// @param best_flow Maximum Flow along a path
/// @param s Source Node
/// @param t Destination Node
/// @param residualCapacity Matrix storing the residual capacities between nodes
/// @param forwardEdges Set of forward edges in the graph
auto augment_path(std::vector<std::optional<u64>> parent, u64 best_flow, u64 s,
                  u64 t, std::vector<std::vector<u64>> &residualCapacity,
                  std::set<std::array<u64, 2>> &forwardEdges) -> void;

/// @brief Finds the maximum flow in a flow network
/// @param s Source Node
/// @param t Destination Node
/// @param residualCapacity Matrix storing the residual capacities between nodes
/// @param adj Graph represented by adjacency list
/// @param forwardEdges Set of forward edges in the graph
/// @return Maximum flow from source node to destination node
auto findFlow(u64 s, u64 t, std::vector<std::vector<u64>> &residualCapacity,
              std::vector<std::vector<u64>> &adj,
              std::set<std::array<u64, 2>> &forwardEdges) -> u64;

/// @brief Finds the min cut of given graph
/// @param s Source Node
/// @param residualCapacity Matrix storing the residual capacities between nodes
/// @param adj Graph represented by adjacency list
/// @param forwardEdges Set of forward edges in the graph
/// @return Min Cut of the graph
auto minCut(u64 s, std::vector<std::vector<u64>> &residualCapacity,
            std::vector<std::vector<u64>> &adj,
            std::set<std::array<u64, 2>> &forwardEdges)
    -> std::set<std::array<u64, 2>>;

#endif
