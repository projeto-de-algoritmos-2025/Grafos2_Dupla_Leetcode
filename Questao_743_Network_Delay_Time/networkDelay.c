#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int node;
    int time;
} Edge;

typedef struct {
    int node;
    int dist;
} HeapNode;

typedef struct {
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;

MinHeap* createHeap(int capacity) {
    MinHeap* heap = malloc(sizeof(MinHeap));
    heap->data = malloc(sizeof(HeapNode) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void push(MinHeap* heap, int node, int dist) {
    int i = heap->size++;
    heap->data[i].node = node;
    heap->data[i].dist = dist;
    while (i > 0 && heap->data[(i - 1) / 2].dist > heap->data[i].dist) {
        swap(&heap->data[i], &heap->data[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

HeapNode pop(MinHeap* heap) {
    HeapNode root = heap->data[0];
    heap->data[0] = heap->data[--heap->size];
    int i = 0;
    while (1) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < heap->size && heap->data[left].dist < heap->data[smallest].dist) smallest = left;
        if (right < heap->size && heap->data[right].dist < heap->data[smallest].dist) smallest = right;
        if (smallest == i) break;
        swap(&heap->data[i], &heap->data[smallest]);
        i = smallest;
    }
    return root;
}

int networkDelayTime(int** times, int timesSize, int* timesColSize, int n, int k) {
    Edge** graph = malloc((n + 1) * sizeof(Edge*));
    int* edgeCount = calloc(n + 1, sizeof(int));
    for (int i = 0; i < timesSize; i++) edgeCount[times[i][0]]++;
    for (int i = 1; i <= n; i++) graph[i] = malloc(edgeCount[i] * sizeof(Edge));
    int* idx = calloc(n + 1, sizeof(int));
    for (int i = 0; i < timesSize; i++) {
        int u = times[i][0], v = times[i][1], w = times[i][2];
        graph[u][idx[u]].node = v;
        graph[u][idx[u]].time = w;
        idx[u]++;
    }

    int* dist = malloc((n + 1) * sizeof(int));
    for (int i = 1; i <= n; i++) dist[i] = INT_MAX;
    dist[k] = 0;

    MinHeap* heap = createHeap(n * n);
    push(heap, k, 0);

    while (heap->size > 0) {
        HeapNode current = pop(heap);
        int node = current.node;
        int d = current.dist;
        if (d > dist[node]) continue;
        for (int i = 0; i < edgeCount[node]; i++) {
            int neighbor = graph[node][i].node;
            int time = graph[node][i].time;
            if (dist[node] + time < dist[neighbor]) {
                dist[neighbor] = dist[node] + time;
                push(heap, neighbor, dist[neighbor]);
            }
        }
    }

    int result = 0;
    for (int i = 1; i <= n; i++) {
        if (dist[i] == INT_MAX) return -1;
        if (dist[i] > result) result = dist[i];
    }
    return result;
}
