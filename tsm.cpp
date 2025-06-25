#include "tsm.h"

//Function to check symmetric graph
bool checkSymmetric(vector<vector<long long>> adj){
    bool check=true;
    for(int i=0; i<adj.size(); i++){
        for(int j=i+1; j<adj.size(); j++){
            if(adj[i][j]!=adj[j][i]){
                check=false;
                break;
            }
        }
        if(!check) break;
    }
    return check;
}
// Function to calculate tour cost
long long calculateTourCost(const vector<int>& tour, const vector<vector<long long>>& adj) {
    if (tour.empty()) return INF;
    long long cost = 0;
    int V = adj.size();
    for (int i = 0; i < V - 1; i++) {
        if (adj[tour[i]][tour[i + 1]] >= INF) return INF;
        cost += adj[tour[i]][tour[i + 1]];
    }
    if (adj[tour[V - 1]][tour[0]] >= INF) return INF;
    cost += adj[tour[V - 1]][tour[0]];
    return cost;
}
// Apply 2-opt to a tour
vector<int> apply2Opt(vector<int> tour, const vector<vector<long long>>& adj) {
    int V = adj.size();
    bool improved = true;
    while (improved) {
        improved = false;
        for (int i = 1; i < V - 1; i++) {
            for (int j = i + 1; j < V - 1; j++) {
                int a = tour[i - 1];
                int b = tour[i];
                int c = tour[j];
                int d = tour[j + 1];
                // Check if new connections are valid
                if (adj[a][c] >= INF || adj[b][d] >= INF) continue;
                // Calculate original segment cost
                long long originalSegmentCost = 0;
                for (int k = i; k < j; k++) {
                    if (adj[tour[k]][tour[k + 1]] >= INF) {
                        originalSegmentCost = INF;
                        break;
                    }
                    originalSegmentCost += adj[tour[k]][tour[k + 1]];
                }
                if (originalSegmentCost >= INF) continue;
                // Calculate reversed segment cost
                long long reversedSegmentCost = 0;
                for (int k = i; k < j; k++) {
                    if (adj[tour[k + 1]][tour[k]] >= INF) {
                        reversedSegmentCost = INF;
                        break;
                    }
                    reversedSegmentCost += adj[tour[k + 1]][tour[k]];
                }
                if (reversedSegmentCost >= INF) continue;
                // Calculate old and new connection costs
                long long oldConnectionCost = adj[a][b] + adj[c][d];
                long long newConnectionCost = adj[a][c] + adj[b][d];
                // Calculate change in cost
                long long deltaCost = -oldConnectionCost + newConnectionCost + 
                                     (reversedSegmentCost - originalSegmentCost);
                if (deltaCost < 0) {
                    // Apply the reverse
                    reverse(tour.begin() + i, tour.begin() + j + 1);
                    improved = true;
                }
            }
        }
    }
    return tour;
}
// ACO Implementation
vector<int> acoTSP(const vector<vector<long long>>& adj, int startIdx) {
    int V = adj.size();
    vector<vector<double>> pheromone(V, vector<double>(V, 1.0));
    vector<int> bestTour;
    double bestCost = INF;
    int noImprovementCount = 0;
    mt19937 gen(42);
    uniform_real_distribution<> dis(0.0, 1.0);
    for (int iter = 0; iter < MAX_ITERATIONS; ++iter) {
        vector<vector<int>> antTours(NUM_ANTS, vector<int>(V, -1));
        vector<double> antCosts(NUM_ANTS, 0.0);
        bool improvedThisIter = false;
        for (int ant = 0; ant < NUM_ANTS; ++ant) {
            vector<bool> visited(V, false);
            int current = startIdx;
            antTours[ant][0] = current;
            visited[current] = true;
            for (int step = 1; step < V; ++step) {
                vector<double> probabilities(V, 0.0);
                double totalProb = 0.0;
                for (int next = 0; next < V; ++next) {
                    if (!visited[next] && adj[current][next] < INF) {
                        double tau = pow(pheromone[current][next], ALPHA);
                        double eta = pow(1.0 / (adj[current][next] + 1e-6), BETA);
                        probabilities[next] = tau * eta;
                        totalProb += probabilities[next];
                    }
                }
                if (totalProb <= 0.0) break;
                double randChoice = dis(gen) * totalProb;
                double cumulativeProb = 0.0;
                int next = -1;
                for (int j = 0; j < V; ++j) {
                    if (!visited[j] && adj[current][j] < INF) {
                        cumulativeProb += probabilities[j];
                        if (cumulativeProb >= randChoice) {
                            next = j;
                            break;
                        }
                    }
                }
                if (next == -1) break;
                antTours[ant][step] = next;
                visited[next] = true;
                current = next;
            }
            if (antTours[ant][V - 1] != -1 && adj[antTours[ant][V - 1]][startIdx] < INF) {
                antCosts[ant] = 0.0;
                for (int i = 0; i < V - 1; ++i) {
                    antCosts[ant] += adj[antTours[ant][i]][antTours[ant][i + 1]];
                }
                antCosts[ant] += adj[antTours[ant][V - 1]][startIdx];
                if (antCosts[ant] < bestCost) {
                    bestCost = antCosts[ant];
                    bestTour = antTours[ant];
                    improvedThisIter = true;
                }
            }
        }

        if (improvedThisIter) {
            noImprovementCount = 0;
        } else {
            noImprovementCount++;
        }
        if (noImprovementCount >= NO_IMPROVEMENT_LIMIT) {
            break;
        }
        // Evaporate pheromone
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                pheromone[i][j] *= (1.0 - EVAPORATION);
            }
        }
        // Deposit pheromone
        for (int ant = 0; ant < NUM_ANTS; ++ant) {
            if (antCosts[ant] > 0.0) {
                double deposit = Q / antCosts[ant];
                for (int i = 0; i < V - 1; ++i) {
                    int u = antTours[ant][i];
                    int v = antTours[ant][i + 1];
                    pheromone[u][v] += deposit;
                }
                pheromone[antTours[ant][V - 1]][startIdx] += deposit;
            }
        }
    }
    // Final optimization with 2-opt
    if (!bestTour.empty()) {
        vector<int> improvedTour = apply2Opt(bestTour, adj);
        long long improvedCost = calculateTourCost(improvedTour, adj);
        if (improvedCost < bestCost) {
            bestTour = improvedTour;
        }
    }
    return (calculateTourCost(bestTour, adj) < THRESHOLD) ? bestTour : vector<int>();
}
string Traveling(const int edges[][3], int numEdges, char start) {
    // Build vertex list
    vector<string> vertices;
    for (int i = 0; i < numEdges; i++) {
        vertices.push_back(to_string(edges[i][0]));
        vertices.push_back(to_string(edges[i][1]));
    }
    sort(vertices.begin(), vertices.end(), [](const string& a, const string& b) {
        return stoi(a) < stoi(b);
    });
    vertices.erase(unique(vertices.begin(), vertices.end()), vertices.end());
    int V = vertices.size();
    // Get start index
    int startIdx = -1;
    string startStr = to_string(static_cast<int>(start));
    for (int i = 0; i < V; i++) {
        if (vertices[i] == startStr) {
            startIdx = i;
            break;
        }
    }
    if (startIdx == -1) return "";
    // Build adjacency matrix
    vector<vector<long long>> adj(V, vector<long long>(V, INF));
    for (int i = 0; i < numEdges; i++) {
        int u = -1, v = -1;
        string uStr = to_string(edges[i][0]), vStr = to_string(edges[i][1]);
        for (int j = 0; j < vertices.size(); j++) {
            if (vertices[j] == uStr) u = j;
            if (vertices[j] == vStr) v = j;
        }
        adj[u][v] = edges[i][2];
    }
    bool isSymmetric = checkSymmetric(adj);
    if (V <= 20) {
        // Held-Karp for small graphs
        vector<vector<long long>> dp((1 << V), vector<long long>(V, INF));
        vector<vector<int>> parent((1 << V), vector<int>(V, -1));
        dp[1 << startIdx][startIdx] = 0;
        for (int mask = 0; mask < (1 << V); mask++) {
            for (int k = 0; k < V; k++) {
                if (!(mask & (1 << k))) continue;
                for (int i = 0; i < V; i++) {
                    if (!(mask & (1 << i)) || i == k || adj[i][k] >= THRESHOLD) continue;
                    long long newCost = dp[mask ^ (1 << k)][i] + adj[i][k];
                    if (newCost < dp[mask][k]) {
                        dp[mask][k] = newCost;
                        parent[mask][k] = i;
                    }
                }
            }
        }
        int finalMask = (1 << V) - 1;
        long long minCost = INF;
        int lastVertex = -1;
        for (int k = 0; k < V; k++) {
            if (k == startIdx || adj[k][startIdx] >= THRESHOLD) continue;
            long long costToK = dp[finalMask][k];
            if (costToK < INF) {
                long long totalCost = costToK + adj[k][startIdx];
                if (totalCost < minCost) {
                    minCost = totalCost;
                    lastVertex = k;
                }
            }
        }
        if (minCost >= THRESHOLD) return "";
        vector<int> path;
        int mask = finalMask;
        int curr = lastVertex;
        while (mask != (1 << startIdx)) {
            path.push_back(curr);
            int prev = parent[mask][curr];
            if (prev == -1) return "";
            mask = mask ^ (1 << curr);
            curr = prev;
        }
        reverse(path.begin(), path.end());
        path.insert(path.begin(), startIdx);
        path.push_back(startIdx);
        string Path = "";
        for (size_t i = 0; i < path.size(); i++) {
            char c = static_cast<char>(stoi(vertices[path[i]]));
            Path += string(1, c);
            if (i < path.size() - 1) Path += " ";
        }
        if(isSymmetric){
            for(int i=0; i<static_cast<int>(Path.length()/2); i++){
                if(Path[i]<Path[Path.length()-i-1]) break;
                else if(Path[i]>Path[Path.length()-i-1]) reverse(Path.begin(),Path.end());
            }
        }
        return Path;
    } else {
        // Run ACO
        vector<int> acoTour = acoTSP(adj, startIdx);
        if (acoTour.empty()) return "";
        string Path = "";
        for (size_t i = 0; i < acoTour.size(); i++) {
            char c = static_cast<char>(stoi(vertices[acoTour[i]]));
            Path += string(1, c);
            if (i < acoTour.size() - 1) Path += " ";
        }
        Path += " " + string(1, start);
        if(isSymmetric){
            for(int i=0; i<static_cast<int>(Path.length()/2); i++){
                if(Path[i]<Path[Path.length()-i-1]) break;
                else if(Path[i]>Path[Path.length()-i-1]) reverse(Path.begin(),Path.end());
            }
        }
        return Path;
    }
}
