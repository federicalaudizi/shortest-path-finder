#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000
#define MAX_WORD_LENGTH 100

// Max Heap structure
typedef struct MaxHeap {
    int* elements;
    int capacity;
    int size;
} MaxHeap;

// Node structure for the linked list
typedef struct Node {
    int vertex;
    struct Node* next;
    MaxHeap* maxHeap;  // Pointer to Max Heap
} Node;

// Graph structure
typedef struct Graph {
    int numVertices;
    Node** adjacencyList;
    int capacity;
} Graph;

void aggiungi_stazione(Graph* graph, char *args[100], int arg_count);

void pianifica_percorso(Graph* graph, char *args[100], int arg_count);

void demolisci_stazione(Graph* graph, char *args[100], int arg_count);

void aggiungi_auto(Graph* graph, char *args[100], int arg_count);

void rottama_auto(Graph *graph, char *args[100], int arg_count);

// Create a new Max Heap
MaxHeap* createMaxHeap(int capacity) {
    MaxHeap* maxHeap = (MaxHeap*)malloc(sizeof(MaxHeap));
    maxHeap->elements = (int*)malloc(capacity * sizeof(int));
    maxHeap->capacity = capacity;
    maxHeap->size = 0;
    return maxHeap;
}

// Resize the graph by increasing the capacity
void resizeGraph(Graph* graph) {
    int newCapacity = graph->numVertices * 2;

    // Create a new adjacency list with increased capacity
    Node** newAdjacencyList = (Node**)malloc(newCapacity * sizeof(Node*));
    for (int i = 0; i < newCapacity; i++) {
        newAdjacencyList[i] = NULL;
    }

    // Copy the existing adjacency list to the new list
    for (int i = 0; i < graph->numVertices; i++) {
        newAdjacencyList[i] = graph->adjacencyList[i];
    }

    // Free the memory occupied by the old adjacency list
    free(graph->adjacencyList);

    // Update the graph with the new adjacency list and capacity
    graph->adjacencyList = newAdjacencyList;
    graph->capacity = newCapacity;
}

// Get the parent index of a node
int getParentIndex(int index) {
    return (index - 1) / 2;
}

// Get the left child index of a node
int getLeftChildIndex(int index) {
    return (2 * index) + 1;
}

// Get the right child index of a node
int getRightChildIndex(int index) {
    return (2 * index) + 2;
}

// Swap two elements in the Max Heap
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Heapify the Max Heap from a given index
void maxHeapify(MaxHeap* maxHeap, int index) {
    int largest = index;
    int leftChildIndex = getLeftChildIndex(index);
    int rightChildIndex = getRightChildIndex(index);

    if (leftChildIndex < maxHeap->size && maxHeap->elements[leftChildIndex] > maxHeap->elements[largest])
        largest = leftChildIndex;

    if (rightChildIndex < maxHeap->size && maxHeap->elements[rightChildIndex] > maxHeap->elements[largest])
        largest = rightChildIndex;

    if (largest != index) {
        swap(&maxHeap->elements[index], &maxHeap->elements[largest]);
        maxHeapify(maxHeap, largest);
    }
}

// Insert an element into the Max Heap
void insert(MaxHeap* maxHeap, int element) {
    maxHeap->size++;
    int index = maxHeap->size - 1;
    maxHeap->elements[index] = element;

    // Fix the Max Heap property
    while (index != 0 && maxHeap->elements[getParentIndex(index)] < maxHeap->elements[index]) {
        swap(&maxHeap->elements[index], &maxHeap->elements[getParentIndex(index)]);
        index = getParentIndex(index);
    }
}

// Extract the maximum element from the Max Heap
int extractMax(MaxHeap* maxHeap) {
    if (maxHeap->size == 0) {
        printf("Max Heap is empty. Unable to extract maximum element.\n");
        return -1;
    }

    if (maxHeap->size == 1) {
        maxHeap->size--;
        return maxHeap->elements[0];
    }

    int root = maxHeap->elements[0];
    maxHeap->elements[0] = maxHeap->elements[maxHeap->size - 1];
    maxHeap->size--;
    maxHeapify(maxHeap, 0);

    return root;
}

// Print the Max Heap
void printMaxHeap(MaxHeap* maxHeap) {
    printf("Max Heap: ");
    for (int i = 0; i < maxHeap->size; i++) {
        printf("%d ", maxHeap->elements[i]);
    }
    printf("\n");
}

// Create a new node with the given vertex and initialize the Max Heap
Node* createNode(int vertex) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;

    // Initialize Max Heap
    //newNode->maxHeap = (MaxHeap*)malloc(sizeof(MaxHeap));
    // Perform additional initialization for the Max Heap if required
    newNode->maxHeap = createMaxHeap( 512);

    return newNode;
}

// Create a graph with the given number of vertices
Graph* createGraph(int numVertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->capacity = 50;

    graph->adjacencyList = (Node**)malloc(numVertices * sizeof(Node*));
    for (int i = 0; i < numVertices; i++) {
        graph->adjacencyList[i] = NULL;
    }

    return graph;
}

// Add an edge to the graph
void addEdge(Graph* graph, int src, int dest) {
    // Add an edge from src to dest
    Node* newNode = createNode(dest);
    newNode->next = graph->adjacencyList[src];
    graph->adjacencyList[src] = newNode;

    // Add an edge from dest to src (for undirected graph)
    newNode = createNode(src);
    newNode->next = graph->adjacencyList[dest];
    graph->adjacencyList[dest] = newNode;
}

// Print the graph
void printGraph(Graph* graph) {
    for (int i = 0; i < graph->numVertices; i++) {
        Node* currentNode = graph->adjacencyList[i];
        printf("Adjacency list of vertex %d: ", i);
        while (currentNode) {
            printf("%d ", currentNode->vertex);
            currentNode = currentNode->next;
        }
        printf("\n");
    }
}

// Main function
int main() {
    int numVertices = 0;
    Graph* graph = createGraph(numVertices);
    char line[MAX_LINE_LENGTH];
    char *word;
    char *args[MAX_WORD_LENGTH];
    int arg_count;

    // Read each line from stdin
    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Extract individual words from the line
        word = strtok(line, " \t\n");  // Split the line by spaces, tabs, and newlines
        arg_count = 0;

        while (word != NULL) {
            // Store the word in the arguments array
            args[arg_count++] = word;

            // Get the next word from the line
            word = strtok(NULL, " \t\n");
        }

        // Check the command and perform the corresponding action
        if (strcmp(args[0], "aggiungi-stazione") == 0) {
            aggiungi_stazione(graph, args, arg_count);
        } else if (strcmp(args[0], "pianifica-percorso") == 0) {
            pianifica_percorso(graph, args, arg_count);
        } else if (strcmp(args[0], "demolisci-stazione") == 0) {
            demolisci_stazione(graph, args, arg_count);
        } else if (strcmp(args[0], "aggiungi-auto") == 0) {
            aggiungi_auto(graph, args, arg_count);
        } else if(strcmp(args[0], "rottama-auto") == 0) {
            rottama_auto(graph, args, arg_count);
        }
    }
    return 0;
}

void rottama_auto(Graph *graph, char *args[100], int arg_count) {

}

void aggiungi_auto(Graph* graph, char *args[100], int arg_count) {

}

void demolisci_stazione(Graph* graph,char *args[100], int arg_count) {

}

void pianifica_percorso(Graph* graph, char *args[100], int arg_count) {

}

void aggiungi_stazione(Graph* graph, char *args[100], int arg_count) {
    if (graph->numVertices == graph->capacity) {
        resizeGraph(graph);
    }

    // Create a new node for the station
    Node* newNode = createNode(atoi(args[1]));
    // Add the new node to the adjacency list
    graph->adjacencyList[graph->numVertices] = newNode;
    // Increment the number of vertices in the graph
    graph->numVertices++;

    for (int i = 3; i <= atoi(args[2]) + 2 ; ++i) {
        insert(newNode->maxHeap, atoi(args[i]));
    }

    printGraph(graph);
    printMaxHeap(newNode->maxHeap);

    printf("\n");
}

