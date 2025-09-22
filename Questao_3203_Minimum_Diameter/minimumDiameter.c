#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int val;
    struct Node* next;
} Node;

void addEdge(Node** adj, int u, int v) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->val = v;
    node->next = adj[u];
    adj[u] = node;
    
    node = (Node*)malloc(sizeof(Node));
    node->val = u;
    node->next = adj[v];
    adj[v] = node;
}

int bfs(Node** adj, int start, int n, int* farthest) {
    int* dist = (int*)calloc(n, sizeof(int));
    int* visited = (int*)calloc(n, sizeof(int));
    int* queue = (int*)malloc(n * sizeof(int));
    int front = 0, rear = 0;
    
    queue[rear++] = start;
    visited[start] = 1;
    dist[start] = 0;
    
    int maxDist = 0;
    *farthest = start;
    
    while (front < rear) {
        int u = queue[front++];
        Node* curr = adj[u];
        
        while (curr) {
            int v = curr->val;
            if (!visited[v]) {
                visited[v] = 1;
                dist[v] = dist[u] + 1;
                queue[rear++] = v;
                
                if (dist[v] > maxDist) {
                    maxDist = dist[v];
                    *farthest = v;
                }
            }
            curr = curr->next;
        }
    }
    
    free(dist);
    free(visited);
    free(queue);
    
    return maxDist;
}

int findDiameter(Node** adj, int n) {
    if (n == 1) return 0;
    
    int farthest;
    // First BFS from any node to find one end of diameter
    bfs(adj, 0, n, &farthest);
    // Second BFS from that end to find diameter
    int diameter = bfs(adj, farthest, n, &farthest);
    
    return diameter;
}

int minimumDiameterAfterMerge(int** edges1, int edges1Size, int* edges1ColSize, 
                              int** edges2, int edges2Size, int* edges2ColSize) {
    int n = edges1Size + 1;
    int m = edges2Size + 1;
    
    // Build adjacency list for tree1
    Node** adj1 = (Node**)calloc(n, sizeof(Node*));
    for (int i = 0; i < edges1Size; i++) {
        addEdge(adj1, edges1[i][0], edges1[i][1]);
    }
    
    // Build adjacency list for tree2
    Node** adj2 = (Node**)calloc(m, sizeof(Node*));
    for (int i = 0; i < edges2Size; i++) {
        addEdge(adj2, edges2[i][0], edges2[i][1]);
    }
    
    // Find diameters of both trees
    int d1 = findDiameter(adj1, n);
    int d2 = findDiameter(adj2, m);
    
    // Calculate radii (ceiling of diameter/2)
    int r1 = (d1 + 1) / 2;  // This computes ceil(d1/2)
    int r2 = (d2 + 1) / 2;  // This computes ceil(d2/2)
    
    // Minimum possible diameter after merging
    int result = d1;
    if (d2 > result) result = d2;
    if (r1 + r2 + 1 > result) result = r1 + r2 + 1;
    
    // Free memory
    for (int i = 0; i < n; i++) {
        Node* curr = adj1[i];
        while (curr) {
            Node* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(adj1);
    
    for (int i = 0; i < m; i++) {
        Node* curr = adj2[i];
        while (curr) {
            Node* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(adj2);
    
    return result;
}