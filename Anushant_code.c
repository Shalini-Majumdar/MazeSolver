#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROWS 5
#define COLS 5

// Node structure for adjacency list
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

// Graph structure with adjacency lists
typedef struct Graph {
    int numVertices;
    Node** adjLists;
} Graph;

// Queue structure for BFS
typedef struct Queue {
    int items[ROWS * COLS];
    int front, rear;
} Queue;

// Maze representation (0 = wall, 1 = path, 9 = exit)
int maze[ROWS][COLS] = {
    {1, 0, 1, 1, 1},
    {1, 1, 1, 0, 1},
    {0, 0, 1, 0, 1},
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 9}
};

// Function prototypes
Graph* createGraph(int vertices);
void addEdge(Graph* graph, int src, int dest);
void printGraph(Graph* graph);
void bfsSolveMaze(Graph* graph, int startVertex, int endVertex);
void displayMaze(int startRow, int startCol, int exitRow, int exitCol);

// Helper functions for maze and graph
Node* createNode(int vertex);
void connectMaze(Graph* graph);
int getVertex(int row, int col);
void printPath(int parent[], int start, int end);

// Queue operations
void initQueue(Queue* q);
bool isEmpty(Queue* q);
void enqueue(Queue* q, int value);
int dequeue(Queue* q);

int main() {
    int vertices = ROWS * COLS;
    Graph* graph = createGraph(vertices);

    // Connect nodes according to maze layout
    connectMaze(graph);

    // Define start (0, 0) and end (4, 4) in the maze
    int startRow = 0, startCol = 0;
    int exitRow = 4, exitCol = 4;
    int startVertex = getVertex(startRow, startCol);
    int endVertex = getVertex(exitRow, exitCol);

    // Display the maze
    printf("Maze Layout:\n");
    displayMaze(startRow, startCol, exitRow, exitCol);

    // Solve the maze using BFS
    printf("Solving the maze:\n");
    bfsSolveMaze(graph, startVertex, endVertex);

    return 0;
}

// Function to create a graph with given vertices
Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = vertices;
    graph->adjLists = (Node*)malloc(vertices * sizeof(Node));

    for (int i = 0; i < vertices; i++)
        graph->adjLists[i] = NULL;

    return graph;
}

// Function to add an edge to the graph (undirected)
void addEdge(Graph* graph, int src, int dest) {
    Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

// Create a node
Node* createNode(int vertex) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

// Connect maze cells based on the layout in the maze array
void connectMaze(Graph* graph) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (maze[i][j] == 1 || maze[i][j] == 9) {
                int currentVertex = getVertex(i, j);

                // Connect with right cell
                if (j + 1 < COLS && (maze[i][j + 1] == 1 || maze[i][j + 1] == 9))
                    addEdge(graph, currentVertex, getVertex(i, j + 1));

                // Connect with down cell
                if (i + 1 < ROWS && (maze[i + 1][j] == 1 || maze[i + 1][j] == 9))
                    addEdge(graph, currentVertex, getVertex(i + 1, j));
            }
        }
    }
}

// Convert (row, col) to vertex index
int getVertex(int row, int col) {
    return row * COLS + col;
}

// BFS to solve the maze
void bfsSolveMaze(Graph* graph, int startVertex, int endVertex) {
    bool visited[ROWS * COLS] = {false};
    int parent[ROWS * COLS];
    for (int i = 0; i < ROWS * COLS; i++) parent[i] = -1;

    Queue q;
    initQueue(&q);

    visited[startVertex] = true;
    enqueue(&q, startVertex);

    while (!isEmpty(&q)) {
        int current = dequeue(&q);

        // If we reached the end, print the path
        if (current == endVertex) {
            printf("Path from start to exit:\n");
            printPath(parent, startVertex, endVertex);
            printf("\n");
            return;
        }

        Node* temp = graph->adjLists[current];
        while (temp) {
            int adjVertex = temp->vertex;

            if (!visited[adjVertex]) {
                visited[adjVertex] = true;
                parent[adjVertex] = current;
                enqueue(&q, adjVertex);
            }
            temp = temp->next;
        }
    }
    printf("No path to the exit found.\n");
}

// Helper function to print path from start to end
void printPath(int parent[], int start, int end) {
    if (end == start) {
        printf("(%d, %d) ", start / COLS, start % COLS);
        return;
    }
    printPath(parent, start, parent[end]);
    printf("-> (%d, %d) ", end / COLS, end % COLS);
}

// Function to display the maze with start and exit positions
void displayMaze(int startRow, int startCol, int exitRow, int exitCol) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (i == startRow && j == startCol) {
                printf("S ");  // Start position
            } else if (i == exitRow && j == exitCol) {
                printf("E ");  // Exit position
            } else if (maze[i][j] == 0) {
                printf("# ");  // Wall
            } else {
                printf(". ");  // Open path
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Queue functions
void initQueue(Queue* q) {
    q->front = q->rear = 0;
}

bool isEmpty(Queue* q) {
    return q->front == q->rear;
}

void enqueue(Queue* q, int value) {
    q->items[q->rear++] = value;
}

int dequeue(Queue* q) {
    return q->items[q->front++];
}