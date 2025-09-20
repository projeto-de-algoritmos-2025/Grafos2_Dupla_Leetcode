#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int row;
    int col;
} Cell;

int minCost(int** grid, int gridSize, int* gridColSize) {
    int rows = gridSize;
    int cols = gridColSize[0];
    int** dist = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        dist[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) dist[i][j] = INT_MAX;
    }

    int dr[4] = {0, 0, 1, -1};
    int dc[4] = {1, -1, 0, 0};

    int qSize = rows * cols * 2;
    Cell* deque = (Cell*)malloc(qSize * sizeof(Cell));
    int* dqDist = (int*)malloc(qSize * sizeof(int));
    int front = qSize / 2, back = qSize / 2;

    deque[back] = (Cell){0, 0};
    dqDist[back] = 0;
    back++;
    dist[0][0] = 0;

    while (front < back) {
        Cell current = deque[front];
        int currentDist = dqDist[front];
        front++;

        int r = current.row;
        int c = current.col;

        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;

            int cost = (grid[r][c] == d + 1) ? 0 : 1;
            if (currentDist + cost < dist[nr][nc]) {
                dist[nr][nc] = currentDist + cost;
                if (cost == 0) {
                    front--;
                    deque[front] = (Cell){nr, nc};
                    dqDist[front] = dist[nr][nc];
                } else {
                    deque[back] = (Cell){nr, nc};
                    dqDist[back] = dist[nr][nc];
                    back++;
                }
            }
        }
    }

    int result = dist[rows - 1][cols - 1];
    for (int i = 0; i < rows; i++) free(dist[i]);
    free(dist);
    free(deque);
    free(dqDist);
    return result;
}