# Graph Library (C++)

A header-only C++ template library for working with graphs. It provides undirected, directed, weighted, and directed weighted graphs, along with common graph algorithms. Nodes can be any hashable type (`int`, `std::string`, ...).

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)

## Classes

| Class | Description |
|-------|-------------|
| `Graph<T>` | Undirected, unweighted graph |
| `DirectedGraph<T>` | Directed, unweighted graph (inherits `Graph<T>`) |
| `WeightedGraph<T, U>` | Undirected graph with edge weights (`U` defaults to `int`) |
| `DirectedWeightedGraph<T, U>` | Directed graph with edge weights (inherits `WeightedGraph<T, U>`) |

## Features

**Graph management**
- Add and remove nodes and edges
- Copy and move constructors and assignment operators
- Construction from an `unordered_map` or an initializer list
- `operator==` for comparing graphs
- Print the graph, clear it, get the number of nodes

**Algorithms**
- BFS and DFS path search
- Bidirectional BFS
- Cycle (loop) detection
- Counting connected components
- Euler cycle detection and construction
- Hamiltonian cycle search (undirected graphs)
- Strongly connected components (directed graphs, Kosaraju's algorithm)
- Reverse (transpose) of a directed graph
- Dijkstra's shortest path (weighted graphs)
- Bellman-Ford shortest path (weighted graphs)

## Requirements

- A C++17 compatible compiler (GCC, Clang, or MSVC)

## Project Structure

```
.
├── Graph.h                      # Class declarations (include this one)
├── Graph.cpp                    # Graph<T>
├── Directed_Graph.cpp           # DirectedGraph<T>
├── Weighted_Graph.cpp           # WeightedGraph<T, U>
├── Directed_Weighted_Graph.cpp  # DirectedWeightedGraph<T, U>
└── main.cpp                     # Your program
```

> This is a template library. `Graph.h` includes all the `.cpp` files at the bottom, so **do not compile them separately**. Keep all files in the same folder and include only `Graph.h`.

## Building

```bash
g++ -std=c++17 main.cpp -o app
./app
```

## Usage

### Undirected graph

```cpp
#include "Graph.h"

int main() {
    Graph<int> g;
    g.addedge(1, 2);
    g.addedge(2, 3);
    g.addedge(3, 4);
    g.addedge(1, 4);

    g.printall();

    std::vector<int> path = g.BFS(1, 3);          // shortest path by edges
    bool hasCycle = g.loop_check();               // true
    size_t components = g.countComponents();      // 1
    std::vector<int> cycle = g.HamiltonCycle();   // Hamiltonian cycle if one exists
    return 0;
}
```

### Directed graph

```cpp
DirectedGraph<std::string> dg;
dg.addedge("A", "B");
dg.addedge("B", "C");
dg.addedge("C", "A");

auto sccs = dg.stronglyConnectedComponents();     // vector of components
auto rev  = dg.getReverse();                      // graph with all edges reversed
auto path = dg.bidirectionalBFS("A", "C");
```

### Weighted graph

```cpp
WeightedGraph<std::string, int> wg;
wg.addedge("A", "B", 4);
wg.addedge("B", "C", 3);
wg.addedge("A", "C", 10);

auto result = wg.Dijkstra_algorithm("A", "C");    // pair<distance, path>
std::cout << "Distance: " << result.first << "\n";
for (const auto& node : result.second) std::cout << node << " ";
```

### Directed weighted graph

```cpp
DirectedWeightedGraph<int, double> dwg;
dwg.addedge(1, 2, 2.5);
dwg.addedge(2, 3, -1.0);

auto result = dwg.Bellman_Ford(1, 3);             // supports negative weights
```

## API Overview

| Method | Description |
|--------|-------------|
| `addnode(node)` / `removenode(node)` | Add or remove a node |
| `addedge(a, b)` / `addedge(a, b, w)` | Add an edge (with weight for weighted graphs) |
| `removeedge(a, b)` | Remove an edge |
| `BFS(a, b)` / `DFS(a, b)` | Find a path from `a` to `b` (empty vector if none) |
| `bidirectionalBFS(a, b)` | Path search from both ends |
| `loop_check()` | Returns `true` if the graph contains a cycle |
| `countComponents()` | Number of connected components |
| `hasEulerCycle()` / `EulerCycle()` | Check for or build an Euler cycle |
| `HamiltonCycle()` | Find a Hamiltonian cycle |
| `stronglyConnectedComponents()` | SCCs of a `DirectedGraph` |
| `getReverse()` | Reverse all edges of a directed graph |
| `Dijkstra_algorithm(a, b)` | Shortest weighted path (non-negative weights) |
| `Bellman_Ford(a, b)` | Shortest weighted path (allows negative weights) |
| `getSize()` | Number of nodes |
| `printall()` / `clear()` | Print or clear the graph |

## Notes

- Self-loops (`addedge(a, a)`) are ignored in unweighted graphs.
- Path-finding methods return an empty vector when the nodes don't exist or no path is found.
- Constructing a graph from an integer (e.g. `Graph<int> g(5)`) is deleted on purpose.

## License

[Add your license here]
