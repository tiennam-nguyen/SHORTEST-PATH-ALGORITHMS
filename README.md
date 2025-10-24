# Discrete Structure - Assignment: Shortest Path Algorithms

This repository contains the C++ implementation for the assignment in the Discrete Structure course (CO1007) at Ho Chi Minh City University of Technology (HCMUT).

The project focuses on implementing and applying fundamental shortest path algorithms in graph theory, specifically the **Bellman-Ford algorithm** and a solution for the **Travelling Salesman Problem (TSP)**.

## Implemented Algorithms

### 1. Bellman-Ford Algorithm

This algorithm is used to find the shortest paths from a single source vertex to all other vertices in a weighted, directed graph, even with negative edge weights. The implementation is split into two parts:

* **`BF()` (1-Step Calculation)**
    * **Purpose:** Executes a single iteration (relaxation step) of the Bellman-Ford algorithm.
    * **Inputs:** The graph, number of edges, start vertex, and the current 'Value' (distance) and 'Previous' (predecessor) arrays.
    * **Outputs:** The updated 'Value' and 'Previous' arrays after one relaxation pass.

* **`BF_Path()` (Full Path Finding)**
    * **Purpose:** Computes the complete shortest path from a start vertex to a goal vertex by running the Bellman-Ford algorithm until completion.
    * **Inputs:** The graph, number of edges, start vertex, and goal vertex.
    * **Output:** A string representing the shortest path (e.g., "ADCB").
    * **Note:** The assignment guarantees all input weights are positive, so negative cycles are not a concern.

### 2. Travelling Salesman Problem (TSP)

This is a classic NP-hard problem in combinatorial optimization.

* **`Traveling()`**
    * **Purpose:** Calculates the shortest possible route that visits every vertex in the graph exactly once and returns to the starting vertex.
    * **Inputs:** The graph, number of edges, and the start vertex.
    * **Output:** A string representing the optimal path.
    * **Approach:** A detailed explanation of the algorithm used to solve the TSP is included in the `tsm.pdf` report.

## File Structure

The project is organized into the following files as required by the assignment specification:

* `main.cpp`: The main driver file used to set up and run the functions.
* `bellman.h`: Header file for Bellman-Ford related functions.
* `bellman.cpp`: Implementation file for `BF()` and `BF_Path()`.
* `tsm.h`: Header file for the Travelling Salesman Problem function.
* `tsm.cpp`: Implementation file for `Traveling()`.
* `tsm.pdf`: A report detailing the approach and solution for the TSP.

## Compilation & Environment

* **Language:** C++
* **Allowed Libraries:** As per the assignment constraints, only the following standard libraries are used:
    * `<iostream>`
    * `<fstream>`
    * `<string>`
    * `<cmath>`
    * `<vector>`
    * `<algorithm>`
    * `<random>`
