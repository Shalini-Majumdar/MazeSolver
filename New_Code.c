#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Node structure for adjacency list
typedef struct Node {
    int v;
    struct Node* next;
} Node;

// Graph structure with adjacency lists
typedef struct Graph {
    int vertices;
    Node** adjList;
} Graph;

// Queue structure for BFS
typedef struct Queue {
    int* data;
    int front, rear;
} Queue;

// Maze representation (0 = wall, 1 = path)
int** maze;
int rows, cols;

// Function prototypes
Graph* createGraph(int vertices);
void addEdge(Graph* graph, int src, int dest);
void bfsSolveMaze(Graph* graph, int startV, int endV);
void displayMaze(int rows, int cols);
Node* newNode(int v);
void connectMaze(Graph* graph);
int getVertex(int row, int cols);
void printPath(int parent[], int start, int end);
void initQueue(Queue* queue, int size);
bool isEmpty(Queue* queue);
void enqueue(Queue* queue, int val);
int dequeue(Queue* queue);
bool isValidMazeRow(char* rowInput, int row, int cols);

// Function to create a graph with given vertices
Graph* createGraph(int vertices) {
    Graph* graphPtr = (Graph*) malloc(sizeof(Graph));
    graphPtr->vertices = vertices;
    graphPtr->adjList = (Node**) malloc(vertices * sizeof(Node*));
    for (int i = 0; i < vertices; i++) graphPtr->adjList[i] = NULL;
    return graphPtr;
}

// Function to add an edge to the graph (undirected)
void addEdge(Graph* graphPtr, int src, int dest) {
    Node* adjListNode = newNode(dest);
    adjListNode->next = graphPtr->adjList[src];
    graphPtr->adjList[src] = adjListNode;

    adjListNode = newNode(src);
    adjListNode->next = graphPtr->adjList[dest];
    graphPtr->adjList[dest] = adjListNode;
}

// Create a node
Node* newNode(int v) {
    Node* adjListNode = (Node*) malloc(sizeof(Node));
    adjListNode->v = v;
    adjListNode->next = NULL;
    return adjListNode;
}

// Queue initialization
void initQueue(Queue* queue, int size) {
    queue->data = (int*) malloc(size * sizeof(int));
    queue->front = queue->rear = -1;
}

// Check if queue is empty
bool isEmpty(Queue* queue) {
    return queue->front == -1;
}

// Enqueue an item
void enqueue(Queue* queue, int val) {
    if (queue->rear == -1)
        queue->front = 0;
    queue->data[++queue->rear] = val;
}

// Dequeue an item
int dequeue(Queue* queue) {
    int val = queue->data[queue->front++];
    if (queue->front > queue->rear)
        queue->front = queue->rear = -1;
    return val;
}

// Connect maze cells based on the layout in the maze array
void connectMaze(Graph* graphPtr) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == 1 || maze[i][j] == 8 || maze[i][j] == 9) {
                int current = getVertex(i, j);
                if (j + 1 < cols && (maze[i][j + 1] == 1 || maze[i][j + 1] == 8 || maze[i][j + 1] == 9))
                    addEdge(graphPtr, current, getVertex(i, j + 1));
                if (i + 1 < rows && (maze[i + 1][j] == 1 || maze[i + 1][j] == 8 || maze[i + 1][j] == 9))
                    addEdge(graphPtr, current, getVertex(i + 1, j));
            }
        }
    }
}

// Convert (row, column) to vertex index
int getVertex(int row, int column) {
    return row * cols + column;
}

// BFS to solve the maze
void bfsSolveMaze(Graph* graphPtr, int startV, int endV) {
    bool visited[rows * cols];
    int parent[rows * cols];
    for (int i = 0; i < rows * cols; i++) {
        visited[i] = false;
        parent[i] = -1;
    }

    Queue queue;
    initQueue(&queue, rows * cols);
    visited[startV] = true;
    enqueue(&queue, startV);

    while (!isEmpty(&queue)) {
        int curr = dequeue(&queue);

        if (curr == endV) {
            printf("Path from start to exit:\n");
            printPath(parent, startV, endV);
            printf("\n");
            return;
        }

        Node* temp = graphPtr->adjList[curr];
        while (temp) {
            int adjV = temp->v;
            if (!visited[adjV]) {
                visited[adjV] = true;
                parent[adjV] = curr;
                enqueue(&queue, adjV);
            }
            temp = temp->next;
        }
    }
    printf("No path to the exit found...\n");
}

// Function to print path from start to end
void printPath(int parent[], int start, int end) {
    if (end == start) {
        printf("(%d, %d) ", start / cols, start % cols);
        return;
    }
    printPath(parent, start, parent[end]);
    printf("-> (%d, %d) ", end / cols, end % cols);
}

// Function to display the maze with walls and paths
// open path    -> .
// walls        -> #
void displayMaze(int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == 0 && j == 0)
                printf("S  ");
            else if (i == rows - 1 && j == cols - 1)
                printf("E  ");
            else if (maze[i][j] == 0)
                printf("#  ");
            else
                printf(".  ");
        }
        printf("\n");
    }
    printf("\n");
}

// Function to validate maze rows
bool isValidMazeRow(char* rowInput, int row, int cols) {
    if (row == 0 && rowInput[0] != '1')
    {
        printf("The first cell of the first row must be 1!\n");
        return false;
    }
    else if (row == rows - 1 && rowInput[cols - 1] != '1')
    {
        printf("The last cell of the last row must be 1!\n");
        return false;
    }
    for (int j = 0; j < cols; j++)
    {
        if (rowInput[j] != '0' && rowInput[j] != '1')
            return false;
    }
    return strlen(rowInput) == cols;
}

int main() {
    // Taking number of rows as input to form a maze
    do {
        printf("Enter the number of rows (2-10): ");
        scanf("%d", &rows);
        if (rows <= 1 || rows > 10) {
            printf("Invalid input! Please enter a value between 2 and 10.\n");
        }
    } while (rows <= 1 || rows > 10);
    
    // Taking number of columns as input to form a maze
    do {
        printf("Enter the number of columns (2-10): ");
        scanf("%d", &cols);
        if (cols <= 1 || cols > 10) {
            printf("Invalid input! Please enter a value between 2 and 10.\n");
        }
    } while (cols <= 1 || cols > 10);

    // Forming the maze
    maze = (int**) malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++)
        maze[i] = (int*) malloc(cols * sizeof(int));

    // Input for the maze
    bool startSet = false;
    for (int i = 0; i < rows; i++) {
        char rowInput[cols + 1];
        bool validInput = false;
        do {
            printf("Row %d: ", i + 1);
            scanf("%s", rowInput);
            validInput = isValidMazeRow(rowInput, i, cols);

            if (!validInput)
                printf("Invalid input, try again...\n");
            else {
                for (int j = 0; j < cols; j++) {
                    maze[i][j] = rowInput[j] - '0';
                }
                if (i == 0) startSet = strchr(rowInput, '1') != NULL;
            }
        } while (!validInput || (i == 0 && !startSet));
    }

    int vertices = rows * cols;
    Graph* graphPtr = createGraph(vertices);
    connectMaze(graphPtr);

    int startV = getVertex(0, 0);
    int endV = getVertex(rows - 1, cols - 1);

    // Display the maze
    printf("\nMaze Layout:-\n. -> path\n# -> wall\nS -> start\nE -> exit\n\n");
    displayMaze(rows, cols);
    
    // Using BFS to find the shortest path in the graph
    bfsSolveMaze(graphPtr, startV, endV);

    // Freeing allocated memory
    for (int i = 0; i < rows; i++)
        free(maze[i]);
    free(maze);

    return 0;
}