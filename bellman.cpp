#include "bellman.h"

void mapVertices(int edges[][3], int numEdges, vector<string>& vertexMap){
    vector<string> vertices;

    for(size_t i = 0; i<numEdges; i++){
        vertices.push_back(to_string(edges[i][0]));
        vertices.push_back(to_string(edges[i][1]));
    }
    sort(vertices.begin(),vertices.end(), [](const string& a, const string& b){
        return stoi(a) < stoi(b);
    });
    vertices.erase(unique(vertices.begin(),vertices.end()),vertices.end());
    vertexMap = vertices;
}

void BF(int edgeList[][3],int numEdges,char start_vertices,int BFValue[],int BFPrev[]){
    vector<string> vertexMap;
    mapVertices(edgeList, numEdges, vertexMap);
    int V = vertexMap.size();

    // Create a temporary vector to store and sort edges
    vector<vector<int>> sortedEdges(numEdges, vector<int>(3));
    
    // Copy original edges to the temporary vector
    for(int i = 0; i < numEdges; i++){
        sortedEdges[i][0] = edgeList[i][0];  // start vertex
        sortedEdges[i][1] = edgeList[i][1];  // end vertex
        sortedEdges[i][2] = edgeList[i][2];  // weight
    }
    
    // Sort the temporary vector by starting vertex first, then ending vertex
    sort(sortedEdges.begin(), sortedEdges.end(), [](const vector<int>& a, const vector<int>& b){
        if(a[0] != b[0]) return a[0] < b[0];  // Sort by starting vertex
        return a[1] < b[1];                   // Then by ending vertex
    });

    // Find the index of the starting vertex in vertexMap
    int startIdx = -1;
    string startStr = to_string(static_cast<int>(start_vertices));
    for(size_t i=0; i<V; i++){
        if(vertexMap[i] == startStr){
            startIdx = i;
            break;
        }
    }
    
    if (startIdx == -1) {
        // If startVertex doesn't exist, set BFPrev[] to -1 for unreachable vertices,
        // except the last vertex which gets BFPrev = 0
        for (int i = 0; i < V; i++) {
            if (BFValue[i] == -1) {
                BFPrev[i] = -1;
            }
        }
        if (V > 0) {
            BFPrev[V - 1] = 0;
        }
        return;
    }

    // Initialize value and prev if not set correctly
    if(startIdx != -1 && BFValue[startIdx] != 0){
        for(int i=0; i<V; i++){
            BFValue[i] = MAX;
            BFPrev[i] = -1;
        }
        BFValue[startIdx] = 0;
    } else {
        // Convert -1 to MAX for consistency
        for(int i=0; i<V; i++){
            if(BFValue[i] == -1) BFValue[i] = MAX;
        }
    }

    // Temporary copy of value for relaxation
    vector<int> prevVal(V);
    for(int i=0; i<V; i++){
        prevVal[i] = BFValue[i];
    }

    // One step of Bellman-Ford
    for(size_t i=0; i<numEdges; i++){
        string uStr = to_string(sortedEdges[i][0]);
        string vStr = to_string(sortedEdges[i][1]);
        int weight = sortedEdges[i][2];

        int u = -1, v = -1;
        for(size_t j=0; j<V; j++){
            if(vertexMap[j] == uStr) u = j;
            if(vertexMap[j] == vStr) v = j;
        }

        // Relaxation
        if(startIdx != -1 && u != -1 && v != -1 && prevVal[u] != MAX){
            if(prevVal[u] + weight < BFValue[v]){
                BFValue[v] = prevVal[u] + weight;
                BFPrev[v] = u;
            }
        }
    }


    // Convert MAX back to -1 in value
    for(int j=0; j<V; j++){
        if(BFValue[j] == MAX) BFValue[j] = -1;
    }
}


void oneStepBF(int edgeList[][3],int numEdges,char start_vertices, vector<int>& BFValue, vector<int>& BFPrev){
    vector<string> vertexMap;
    mapVertices(edgeList, numEdges, vertexMap);
    int V = vertexMap.size();

    // Create a temporary vector to store and sort edges
    vector<vector<int>> sortedEdges(numEdges, vector<int>(3));
    
    // Copy original edges to the temporary vector
    for(int i = 0; i < numEdges; i++){
        sortedEdges[i][0] = edgeList[i][0];  // start vertex
        sortedEdges[i][1] = edgeList[i][1];  // end vertex
        sortedEdges[i][2] = edgeList[i][2];  // weight
    }
    
    // Sort the temporary vector by starting vertex first, then ending vertex
    sort(sortedEdges.begin(), sortedEdges.end(), [](const vector<int>& a, const vector<int>& b){
        if(a[0] != b[0]) return a[0] < b[0];  // Sort by starting vertex
        return a[1] < b[1];                   // Then by ending vertex
    });

    // Find the index of the starting vertex in vertexMap
    int startIdx = -1;
    string startStr = to_string(static_cast<int>(start_vertices));
    for(size_t i=0; i<V; i++){
        if(vertexMap[i] == startStr){
            startIdx = i;
            break;
        }
    }
    
    if (startIdx == -1) {
        // If startVertex doesn't exist, set BFPrev[] to -1 for unreachable vertices,
        // except the last vertex which gets BFPrev = 0
        for (int i = 0; i < V; i++) {
            if (BFValue[i] == -1) {
                BFPrev[i] = -1;
            }
        }
        if (V > 0) {
            BFPrev[V - 1] = 0;
        }
        return;
    }

    // Initialize value and prev if not set correctly
    if(startIdx != -1 && BFValue[startIdx] != 0){
        for(int i=0; i<V; i++){
            BFValue[i] = MAX;
            BFPrev[i] = -1;
        }
        BFValue[startIdx] = 0;
    } else {
        // Convert -1 to MAX for consistency
        for(int i=0; i<V; i++){
            if(BFValue[i] == -1) BFValue[i] = MAX;
        }
    }

    // Temporary copy of value for relaxation
    vector<int> prevVal(V);
    for(int i=0; i<V; i++){
        prevVal[i] = BFValue[i];
    }

    // One step of Bellman-Ford
    for(size_t i=0; i<numEdges; i++){
        string uStr = to_string(sortedEdges[i][0]);
        string vStr = to_string(sortedEdges[i][1]);
        int weight = sortedEdges[i][2];

        int u = -1, v = -1;
        for(size_t j=0; j<V; j++){
            if(vertexMap[j] == uStr) u = j;
            if(vertexMap[j] == vStr) v = j;
        }

        // Relaxation
        if(startIdx != -1 && u != -1 && v != -1 && prevVal[u] != MAX){
            if(prevVal[u] + weight < BFValue[v]){
                BFValue[v] = prevVal[u] + weight;
                BFPrev[v] = u;
            }
        }
    }


    // Convert MAX back to -1 in value
    for(int j=0; j<V; j++){
        if(BFValue[j] == MAX) BFValue[j] = -1;
    }
}

string BF_Path(int edges[][3], int numEdges, char start, char goal){
    vector<string> vertexMap;
    mapVertices(edges, numEdges, vertexMap);
    int startIdx = -1, goalIdx = -1;

    string startStr = to_string(static_cast<int>(start)), goalStr=to_string(static_cast<int>(goal));
    for(size_t i = 0; i<vertexMap.size(); i++){
        if(vertexMap[i]==startStr) startIdx=i;
        if(vertexMap[i]==goalStr) goalIdx=i;
    }
    if(startIdx==-1 || goalIdx==-1) return "";

    vector<int> value(vertexMap.size(), MAX);
    vector<int> prev(vertexMap.size(), -1);
    value[startIdx]=0;

    // Running BF algo
    vector<int> tempVal;
    for(size_t j=0; j<vertexMap.size()-1; j++){
        tempVal=value;
        oneStepBF(edges, numEdges, start, value, prev);
        if(tempVal==value) break; // Nothing changes, meaning we've found the shortest path
    }

    // Parsing the path
    if(value[goalIdx]==MAX) return ""; // The goal is disconnected from the graph

    vector<int> pathIdx;
    int curr = goalIdx;
    while(curr!=-1){ // Trace back from goal
        pathIdx.push_back(curr);
        curr=prev[curr];
    }
    reverse(pathIdx.begin(),pathIdx.end());
    
    string path;
    for(size_t i=0; i<pathIdx.size(); i++){
        path += static_cast<char>(stoi(vertexMap[pathIdx[i]]));
        if(i<pathIdx.size()-1) path+=" ";
    }
    return path;
}