#include <limits.h>

#define MAX_N 100

int findTheCity(int n, int** edges, int edgesSize, int* edgesColSize, int distanceThreshold) {
    int dist[MAX_N][MAX_N];
    
    // Initialize distance matrix with infinity
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                dist[i][j] = 0;
            } else {
                dist[i][j] = INT_MAX;
            }
        }
    }
    
    // Fill in the edges (bidirectional)
    for (int i = 0; i < edgesSize; i++) {
        int from = edges[i][0];
        int to = edges[i][1];
        int weight = edges[i][2];
        dist[from][to] = weight;
        dist[to][from] = weight;
    }
    
    // Floyd-Warshall algorithm to find all shortest paths
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
    
    // Count reachable cities for each city
    int minCount = INT_MAX;
    int result = 0;
    
    for (int i = 0; i < n; i++) {
        int count = 0;
        for (int j = 0; j < n; j++) {
            if (i != j && dist[i][j] <= distanceThreshold) {
                count++;
            }
        }
        
        // Update result if we found a city with fewer neighbors
        // or same number but with greater index
        if (count < minCount || (count == minCount && i > result)) {
            minCount = count;
            result = i;
        }
    }
    
    return result;
}