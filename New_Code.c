
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
    int* items;
    int front, rear;
} Queue;

// Maze representation (0 = wall, 1 = path, 8 = start, 9 = exit)
int** maze;
int rows, cols;

// Function prototypes
Graph* createGraph(int vertices);
void addEdge(Graph* graph, int source, int destination);
void bfsSolveMaze(Graph* graph, int startVertex, int endVertex);
void displayMaze(int rows, int columns, int startRow, int startCol, int exitRow, int exitCol);
Node* createNode(int vertex);
void connectMaze(Graph* graph);
int getVertex(int row, int columns);
void printPath(int parent[], int start, int end);
void initQueue(Queue* queue, int size);
bool isEmpty(Queue* queue);
void enqueue(Queue* queue, int value);
int dequeue(Queue* queue);
bool isValidMazeRow(char* rowInput, int row, int columns);

// Function to create a graph with given vertices
Graph* createGraph(int vertices) {
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    graph->numVertices = vertices;
    graph->adjLists = (Node**) malloc(vertices * sizeof(Node*));

    for (int i = 0; i < vertices; i++)
        graph->adjLists[i] = NULL;

    return graph;
}

// Function to add an edge to the graph (undirected)
void addEdge(Graph* graph, int source, int destination) {
    Node* newNode = createNode(destination);
    newNode->next = graph->adjLists[source];
    graph->adjLists[source] = newNode;

    newNode = createNode(source);
    newNode->next = graph->adjLists[destination];
    graph->adjLists[destination] = newNode;
}

// Create a node
Node* createNode(int vertex) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

// Queue initialization
void initQueue(Queue* queue, int size) {
    queue->items = (int*)malloc(size * sizeof(int));
    queue->front = -1;
    queue->rear = -1;
}

// Check if queue is empty
bool isEmpty(Queue* queue) {
    return queue->front == -1;
}

// Enqueue an item
void enqueue(Queue* queue, int value) {
    if (queue->rear == -1) {
        queue->front = 0;
    }
    queue->rear++;
    queue->items[queue->rear] = value;
}

// Dequeue an item
int dequeue(Queue* queue) {
    int item = queue->items[queue->front];
    queue->front++;
    if (queue->front > queue->rear) {
        queue->front = queue->rear = -1; // Reset queue
    }
    return item;
}

// Connect maze cells based on the layout in the maze array
void connectMaze(Graph* graph) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == 1 || maze[i][j] == 8 || maze[i][j] == 9) {
                int currentVertex = getVertex(i, j);

                // Connect with right cell
                if (j + 1 < cols && (maze[i][j + 1] == 1 || maze[i][j + 1] == 8 || maze[i][j + 1] == 9))
                    addEdge(graph, currentVertex, getVertex(i, j + 1));

                // Connect with down cell
                if (i + 1 < rows && (maze[i + 1][j] == 1 || maze[i + 1][j] == 8 || maze[i + 1][j] == 9))
                    addEdge(graph, currentVertex, getVertex(i + 1, j));
            }
        }
    }
}

// Convert (row, column) to vertex index
int getVertex(int row, int column) {
    return row * cols + column;
}

// BFS to solve the maze
void bfsSolveMaze(Graph* graph, int startVertex, int endVertex) {
    bool visited[rows * cols];
    int parent[rows * cols];
    for (int i = 0; i < rows * cols; i++) {
        visited[i] = false;
        parent[i] = -1;
    }

    Queue queue;
    initQueue(&queue, rows * cols);

    visited[startVertex] = true;
    enqueue(&queue, startVertex);

    while (!isEmpty(&queue)) {
        int current = dequeue(&queue);

        if (current == endVertex) {
            printf("Shortest path from start to exit:\n");
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
                enqueue(&queue, adjVertex);
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
void displayMaze(int rows, int columns, int startRow, int startCol, int exitRow, int exitCol) {
    // Traversing the maze
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            // Storing the total number of connections, horizontal and vertical that a wall has to its adjacent cells
            int connections = 0;
            int horizontal = 0;
            int vertical = 0;
            if (i == startRow && j == startCol)
            {
                printf("S  ");  // Start position
            }
            else if (i == exitRow && j == exitCol)
            {
                printf("E  ");  // Exit position
            }
            else if (maze[i][j] == 0)   // If the currrent cell is a wall
            {
                printf("#  ");
            }
            else
            {
                printf(".  ");  // Open path
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Function to validate maze rows
bool isValidMazeRow(char* rowInput, int row, int columns) {
    if (row == 0) {
        // First row must contain at least one '8'
        return (strchr(rowInput, '8') != NULL && strlen(rowInput) == columns);
    } else if (row == rows - 1) {
        // Last row must end with '9'
        return (rowInput[columns - 1] == '9' && strlen(rowInput) == columns);
    } else {
        // Intermediate rows can only contain '0' and '1'
        for (int j = 0; j < columns; j++) {
            if (rowInput[j] != '0' && rowInput[j] != '1') {
                return false;
            }
        }
        return strlen(rowInput) == columns; // Check length
    }
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
    maze = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        maze[i] = (int*)malloc(cols * sizeof(int));
    }

    // Input for the maze
    bool startFound = false;
    for (int i = 0; i < rows; i++) {
        char rowInput[cols + 1];
        bool validInput = false;

        do {
            printf("Row %d: ", i + 1);
            scanf("%s", rowInput);

            // Validate the input for the maze row
            validInput = isValidMazeRow(rowInput, i, cols);

            if (!validInput) {
                if (i == 0) {
                    printf("Error: The first row must contain '8' and have exactly %d elements.\n", cols);
                } else if (i == rows - 1) {
                    printf("Error: The last row must end with '9' and have exactly %d elements.\n", cols);
                } else {
                    printf("Error: Intermediate rows can only contain '0' and '1' and must have exactly %d elements.\n", cols);
                }
            }

            // Build the maze if the input is valid
            if (validInput) {
                for (int j = 0; j < cols; j++) {
                    maze[i][j] = rowInput[j] - '0';
                }
                if (i == 0) {
                    startFound = (strchr(rowInput, '8') != NULL); // Check if '8' is in the first row
                }
            }
        } while (!validInput || (i == 0 && !startFound));
    }


    int vertices = rows * cols;
    Graph* graph = createGraph(vertices);

    connectMaze(graph);

    // Define start (8's position) and end (9's position) in the maze
    int startRow, startCol, exitRow, exitCol;

    // Find the start position (8) and exit position (9)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == 8) {
                startRow = i;
                startCol = j;
            }
            if (maze[i][j] == 9) {
                exitRow = i;
                exitCol = j;
            }
        }
    }

    int startVertex = getVertex(startRow, startCol);
    int endVertex = getVertex(exitRow, exitCol);

    // Display the maze
    printf("\nMaze Layout:-\n. -> path\n# -> wall\nS -> start\nE -> exit\n\n");
    displayMaze(rows, cols, startRow, startCol, exitRow, exitCol);

    // Using BFS to find the shortest path in the graph
    printf("Solving the maze:\n");
    bfsSolveMaze(graph, startVertex, endVertex);

    // Free allocated memory
    for (int i = 0; i < rows; i++) {
        free(maze[i]);
    }
    free(maze);

    return 0;
}
