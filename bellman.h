#ifndef BELLMAN_H
#define BELLMAN_H

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;
#define MAX 1e9

void mapVertices(int edges[][3], int numEdges, vector<string>& vertexMap);

void BF(int edgeList[][3],int numEdges,char start_vertices,int BFValue[],int BFPrev[]);

void oneStepBF(int edgeList[][3],int numEdges,char start_vertices, vector<int>& BFValue, vector<int>& BFPrev);

string BF_Path(int edges[][3], int numEdges, char start, char goal);

#endif