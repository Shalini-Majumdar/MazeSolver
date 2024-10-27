#include <stdio.h>
#include <stdlib.h>

#define MAX 100
int visited[MAX];

struct Node {
    int dest;
    struct Node* next;
};

typedef struct Graph {
    struct Node* head[MAX];
}graph;

struct Node* createNode(int dest) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

void addEdge(struct Graph* graph, int src, int dest) {
    struct Node* newNode = createNode(dest);
    newNode->next = graph->head[src];
    graph->head[src] = newNode;

    newNode = createNode(src);
    newNode->next = graph->head[dest];
    graph->head[dest] = newNode;
}

int dfs(struct Graph* graph, int v, int end) {
    visited[v] = 1;

    if (v == end)
        return 1;

    struct Node* temp = graph->head[v];
    while (temp != NULL) {
        if (!visited[temp->dest]) {
            if (dfs(graph, temp->dest, end))
                return 1;
        }
        temp = temp->next;
    }

    return 0;
}

int main() {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));

    for (int i = 0; i < MAX; i++)
        graph->head[i] = NULL;
    
    addEdge(graph, 0, 1);
    addEdge(graph, 1, 2);
    addEdge(graph, 2, 3);

    int start = 0, end = 3;
    for (int i = 0; i < MAX; i++)
        visited[i] = 0;

    if (dfs(graph, start, end))
        printf("Path exists from %d to %d\n", start, end);
    else
        printf("No path exists from %d to %d\n", start, end);

    return 0;
}
