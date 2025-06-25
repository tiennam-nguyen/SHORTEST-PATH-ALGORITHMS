#ifndef TSM_H
#define TSM_H
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;
#define INF 1e18
#define THRESHOLD 1e18-1

// ACO parameters
const int NUM_ANTS = 60;    // Increased from 30
const int MAX_ITERATIONS = 500; // Increased from 120
const double ALPHA = 1.1;  // pheromone importance
const double BETA = 2.0;   // heuristic importance
const double EVAPORATION = 0.45;
const double Q = 100.0;    // pheromone deposit factor
const int NO_IMPROVEMENT_LIMIT = 100; // Increased from 30

string Traveling(const int edges[][3], int numEdges, char start);

#endif