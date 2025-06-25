#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <set>
#include "bellman.h"
#include "tsm.h"
using namespace std;
#define MAXSIZE 1000

// Function to print an edge
void printedge(int edge[]) {
    cout << (char)edge[0] << (char)edge[1] << "," << edge[2] << endl;
}

// Function to generate a random edges list
int edgeListGen(int edgeList[][3], int numEdges, int numVertices, int initlimit = 1);

// main Function
int main() {
    bool randGen = 1;
    int edgeList[MAXSIZE][3];
    int numEdges = 40;
    int numVertices = 10;
    int initlimit = 15;

    if (!randGen) {
        // Read from file
        ifstream fin("EdgeList.txt");
        char v1, v2;
        for (int i = 0; i < numEdges; i++) {
            fin >> v1 >> v2 >> edgeList[i][2];
            edgeList[i][0] = v1;
            edgeList[i][1] = v2;
        }
        fin.close();
    } else {
        // Generate a random edgelist
        if (edgeListGen(edgeList, numEdges, numVertices, initlimit) < 0) return -1;
    }

    // Determine unique vertices and sort them
    set<int> verticesSet;
    for (int i = 0; i < numEdges; i++) {
        verticesSet.insert(edgeList[i][0]);
        verticesSet.insert(edgeList[i][1]);
    }
    vector<int> vertices(verticesSet.begin(), verticesSet.end());
    sort(vertices.begin(), vertices.end());
    int actualNumVertices = vertices.size();

    // Choose start vertex
    char start_vertices = (char)edgeList[0][0];

    // Test BF()
    {
        const int MAX_V = 50;
        if (actualNumVertices > MAX_V) {
            cout << "Too many vertices" << endl;
            return -1;
        }
        int BFValue[MAX_V];
        int BFPrev[MAX_V];
        // Initialize BFValue and BFPrev
        for (int i = 0; i < actualNumVertices; i++) {
            BFValue[i] = (vertices[i] == (int)start_vertices) ? 0 : 1000000000;
            BFPrev[i] = -1;
        }
        BF(edgeList, numEdges, start_vertices, BFValue, BFPrev);
        // Print results
        cout << "After one step of Bellman-Ford:" << endl;
        for (int i = 0; i < actualNumVertices; i++) {
            cout << "Vertex " << (char)vertices[i] << ": BFValue = " << BFValue[i] << ", BFPrev = ";
            if (BFPrev[i] == -1) {
                cout << "None";
            } else {
                cout << (char)vertices[BFPrev[i]];
            }
            cout << endl;
        }
    }

    // Test BF_Path()
    {
        char goal_vertices = (char)vertices.back();
        string path = BF_Path(edgeList, numEdges, start_vertices, goal_vertices);
        cout << "\nShortest path from " << start_vertices << " to " << goal_vertices << ": " << path << endl;
    }

    // Test Traveling()
    {
        string tsp_path = Traveling(edgeList, numEdges, start_vertices);
        cout << "\nTSP path starting from " << start_vertices << ": " << tsp_path << endl;
    }

    return 0;
}


int edgeListGen(int edgeList[][3], int numEdges, int numVertices, int initlimit) {
    random_device rd;
    mt19937 gen(rd());

    // Check graph constraints
    if (numEdges > (numVertices * (numVertices - 1)) / 2) {
        cout << "cannot create simple graph" << endl;
        return -1;
    }
    if (numEdges < numVertices - 1) {
        cout << "cannot create a connected (weak) graph" << endl;
        return -1;
    }

    // Generate random vertex names (ASCII 33 to 126)
    vector<int> verName;
    for (int i = 33; i <= 126; i++) {
        verName.push_back(i);
    }
    shuffle(verName.begin(), verName.end(), gen);
    vector<int> verList(verName.begin(), verName.begin() + numVertices);

    // Generate edges ensuring connectivity
    vector<pair<int, int>> fullList;
    for (int i = 0; i < numVertices; i++) {
        for (int j = i + 1; j < numVertices; j++) {
            fullList.push_back({verList[i], verList[j]});
        }
    }
    shuffle(fullList.begin(), fullList.end(), gen);
    for (int i = 0; i < numEdges; i++) {
        edgeList[i][0] = fullList[i].first;
        edgeList[i][1] = fullList[i].second;
    }

    // Generate weights
    if (initlimit <= 1) {
        for (int i = 0; i < numEdges; i++) {
            edgeList[i][2] = 1;
        }
    } else {
        uniform_int_distribution<int> dist(1, initlimit);
        for (int i = 0; i < numEdges; i++) {
            edgeList[i][2] = dist(gen);
        }
    }
    return 1;
}