#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Define the struct for each element in the array
struct element {
    int value;             // The integer value of the element
    struct MaxHeap *heap;  // Pointer to the max heap
};

struct array {
    struct element *data;
    int capacity;
    int size;
};

struct MaxHeap {
    int *array;
    int capacity;
    int size;
};


void addStation(char **args, int arg_count, struct array *stations);

void addCar(char **args, struct array *stations);

void removeStation(char **args, struct array *stations);

void removeCar(char **args, struct array *stations);

void resizeArray(struct array *arr) {
    struct element *newData = (struct element *) malloc(arr->capacity * 2 * sizeof(struct element));


    for (int i = 0; i < arr->size; i++) {
        newData[i] = arr->data[i];
    }

    free(arr->data);
    arr->data = newData;
    arr->capacity = arr->capacity * 2;
}

void freeDynamicArray(struct array *dynArray) {
    if (dynArray) {
        free(dynArray->data);
        free(dynArray);
    }
}


struct MaxHeap *createMaxHeap(int capacity);

struct array *createDynamicArray(int initialCapacity) {
    struct array *dynArray = (struct array *) malloc(sizeof(struct array));

    dynArray->capacity = initialCapacity;
    dynArray->size = 0;
    dynArray->data = (struct element *) malloc(initialCapacity * sizeof(struct element));
    for (int i = 0; i < initialCapacity; ++i) {
        dynArray->data[i].heap = createMaxHeap(512);
    }

    return dynArray;
}

struct MaxHeap *createMaxHeap(int capacity) {
    struct MaxHeap *heap = (struct MaxHeap *) malloc(sizeof(struct MaxHeap));
    if (heap == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    heap->capacity = capacity;
    heap->size = 0;
    heap->array = (int *) malloc(capacity * sizeof(int));
    if (heap->array == NULL) {
        printf("Memory allocation failed.\n");
        free(heap);
        return NULL;
    }

    return heap;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(struct MaxHeap *heap, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->array[left] > heap->array[largest])
        largest = left;

    if (right < heap->size && heap->array[right] > heap->array[largest])
        largest = right;

    if (largest != index) {
        swap(&heap->array[index], &heap->array[largest]);
        heapify(heap, largest);
    }
}

void insertMaxHeap(struct MaxHeap *heap, int value) {
    if (heap->size == heap->capacity) {
        printf("Heap is full. Resizing...\n");
        heap->capacity *= 2;
        heap->array = (int *) realloc(heap->array, heap->capacity * sizeof(int));
    }

    int index = heap->size;
    heap->array[index] = value;
    heap->size++;

    int parent = (index - 1) / 2;
    while (index > 0 && heap->array[index] > heap->array[parent]) {
        swap(&heap->array[index], &heap->array[parent]);
        index = parent;
        parent = (index - 1) / 2;
    }
}

int getMax(struct MaxHeap *heap) {
    if (heap->size == 0) {
        return -1; // Return -1 to indicate an empty heap or use an appropriate error code.
    }

    int max = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    heapify(heap, 0);

    return max;
}

void freeMaxHeap(struct MaxHeap *heap) {
    if (heap) {
        free(heap->array);
        free(heap);
    }
}

int binarySearch(struct array *arr, int start, int end, int target) {
    while (start <= end) {
        int mid = start + (end - start) / 2;

        if (arr->data[mid].value == target) {
            return mid; // Element found, return its index
        } else if (arr->data[mid].value < target) {
            start = mid + 1; // Target is in the end half
        } else {
            end = mid - 1; // Target is in the start half
        }
    }

    return -1; // Element not found
}

int main() {
    char line[1000];
    char *word;
    char *args[515];
    int arg_count;

    struct array *stations = createDynamicArray(50);


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
            addStation(args, arg_count, stations);
        } else if (strcmp(args[0], "pianifica-percorso") == 0) {
        } else if (strcmp(args[0], "demolisci-stazione") == 0) {
            removeStation(args,stations);
        } else if (strcmp(args[0], "aggiungi-auto") == 0) {
            addCar(args, stations);
        } else if (strcmp(args[0], "rottama-auto") == 0) {
            removeCar(args, stations);
        }
        for (int i = 0; i < arg_count; i++) {
            args[i] = NULL;
        }
    }
    for (int i = 0; i < stations->size; ++i) {
        printf("%d ", stations->data[i].value);
    }
    return 0;
}

int insertSorted(struct array *arr, int key) {
    // Cannot insert more elements if n is already
    // more than or equal to capacity
    if (arr->size >= arr->capacity)
        resizeArray(arr);
    int i;
    if (arr->size == 0) {
        arr->data[0].value = key;
        arr->size++;
        return 0;
    } else {
        for (i = arr->size - 1; (i >= 0 && arr->data[i].value > key); i--) {
            arr->data[i + 1].value = arr->data[i].value;
        }
    }

    arr->data[i + 1].value = key;
    arr->size++;
    return i + 1;
}

void removeElement(struct array *arr, int carIndex) {
    int j;

    // Shift the elements after the target element one position to the left
    for (j = carIndex; j < arr->size-1; j++) {
        arr->data[j].value = arr->data[j + 1].value;
    }

    freeMaxHeap(arr->data->heap);
    // Decrement the size of the array
    arr->size--;
}

int max_heap_search(struct MaxHeap *heap, int target) {
    for (int i = 0; i < heap->size; i++) {
        if (heap->array[i] == target) {
            return i; // Found the element at index i
        }
    }

    return -1; // Element not found in the max heap
}



void addStation(char **args, int arg_count, struct array *stations) {
    int carIndex = insertSorted(stations, atoi(args[1]));

    for (int i = 3; i < arg_count; i++) {
        insertMaxHeap(stations->data[carIndex].heap, atoi(args[i]));
    }

    printf("aggiunta\n");
}

void addCar(char **args, struct array *stations) {
    int carIndex = binarySearch(stations, 0, stations->size, atoi(args[1]));

    if (carIndex == -1) {
        printf("non aggiunta\n");
    } else {
        insertMaxHeap(stations->data[carIndex].heap, atoi(args[2]));
    }
}

void removeStation(char **args, struct array *stations){
    int stationIndex = binarySearch(stations, 0, stations->size, atoi(args[1]));

    if (stationIndex == -1){
        printf("non demolita\n");
    } else{
        removeElement(stations, stationIndex);
        printf("demolita\n");

    }
}

void removeCar(char **args, struct array *stations){
    int stationIndex = binarySearch(stations, 0, stations->size, atoi(args[1]));

    if (stationIndex == -1){
        printf("non demolita\n");
    } else{
        int carIndex = max_heap_search(stations->data[stationIndex].heap, atoi(args[1]));
        if(carIndex == -1){
            printf("non demolita\n");
        }else{

            printf("demolita\n");
        }


    }
}