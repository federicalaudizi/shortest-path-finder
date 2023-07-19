#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


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

void directJourney(int departure, int arrival, struct array *stations);

void indirectJourney(int departure, int arrival, struct array *stations);

void resizeArray(struct array *arr) {
    struct element *newData = (struct element *) malloc(arr->capacity * 2 * sizeof(struct element));


    for (int i = 0; i < arr->size; i++) {
        newData[i] = arr->data[i];
    }

    free(arr->data);
    arr->data = newData;
    arr->capacity = arr->capacity * 2;
}

// Function to resize the MaxHeap
void resizeMaxHeap(struct MaxHeap *heap) {
    int newCapacity = heap->capacity * 2;

    // Allocate memory for the new array
    int *newArray = (int *)malloc(newCapacity * sizeof(int));

    // Copy elements from the old array to the new array
    for (int i = 0; i < heap->size; i++) {
        newArray[i] = heap->array[i];
    }

    // Deallocate the old array
    free(heap->array);

    // Update the heap struct with the new array and capacity
    heap->array = newArray;
    heap->capacity = newCapacity;
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
        dynArray->data[i].heap = createMaxHeap(32);
    }

    return dynArray;
}

struct MaxHeap *createMaxHeap(int capacity) {
    struct MaxHeap *heap = (struct MaxHeap *) malloc(sizeof(struct MaxHeap));
    if (heap == NULL) {
        return NULL;
    }

    heap->capacity = capacity;
    heap->size = 0;
    heap->array = (int *) malloc(capacity * sizeof(int));
    if (heap->array == NULL) {
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

    return heap->array[0];
}

void freeMaxHeap(struct MaxHeap *heap) {
    if (heap) {
        free(heap->array);
        free(heap);
    }
}

int binarySearchStruct(struct array *arr, int start, int end, int target) {
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

int binarySearchAsc(const int *arr, int start, int end, int target) {
    while (start <= end) {
        int mid = start + (end - start) / 2;

        if (arr[mid] == target) {
            return mid; // Element found, return its index
        } else if (arr[mid] < target) {
            start = mid + 1; // Target is in the end half
        } else {
            end = mid - 1; // Target is in the start half
        }
    }

    return -1; // Element not found
}

int binarySearchDesc(const int *arr, int start, int end, int target) {
    while (start <= end) {
        int mid = start + (end - start) / 2;

        if (arr[mid] == target) {
            return mid; // Element found, return its index
        } else if (arr[mid] < target) {
            end = mid - 1;// Target is in the end half
        } else {
            start = mid + 1; // Target is in the start half
        }
    }

    return -1; // Element not found
}

int main() {
    char line[10000];
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
            if (atoi(args[1]) < atoi(args[2])) {
                //andata
                directJourney(atoi(args[1]), atoi(args[2]), stations);
            } else if (atoi(args[1]) > atoi(args[2])) {
                //ritorno
                indirectJourney(atoi(args[1]), atoi(args[2]), stations);
            } else {
                printf("%d\n", atoi(args[1]));
            }
        } else if (strcmp(args[0], "demolisci-stazione") == 0) {
            removeStation(args, stations);
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
        freeMaxHeap(stations->data[i].heap);
    }

    freeDynamicArray(stations);
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
        arr->data[0].heap = createMaxHeap(32);
        arr->size++;
        return 0;
    } else {
        for (i = arr->size - 1; (i >= 0 && arr->data[i].value > key); i--) {
            arr->data[i + 1] = arr->data[i];
        }
        arr->data[i + 1].value = key;
        arr->data[i + 1].heap = createMaxHeap(32);
    }

    arr->size++;

    return i + 1;
}

void removeElement(struct array *arr, int carIndex) {
    int j;

    //todo caso limite solo due stazioni

    // Shift the elements after the target element one position to the left
    for (j = carIndex; j < arr->size - 1; j++) {
        arr->data[j] = arr->data[j + 1];
    }

    arr->data[arr->size-1].heap = NULL;
    arr->data[arr->size-1].value = 0;

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

// Function to delete a given element from the max heap
void deleteElement(struct MaxHeap *maxHeap, int index) {

    // Swap the element with the last element in the heap
    swap(&maxHeap->array[index], &maxHeap->array[maxHeap->size - 1]);

    // Decrement the heap size to remove the last element (element to be deleted)
    maxHeap->size--;

    // Re-heapify the heap
    heapify(maxHeap, index);
}

void addStation(char **args, int arg_count, struct array *stations) {
    if (binarySearchStruct(stations, 0 , stations->size, atoi(args[1])) == -1 ){
        int carIndex = insertSorted(stations, atoi(args[1]));
        for (int i = 3; i < arg_count; i++) {
            if (stations->data[carIndex].heap->size == stations->data[carIndex].heap->capacity || arg_count-3 > stations->data[carIndex].heap->capacity){
                resizeMaxHeap(stations->data[carIndex].heap);
            }
            insertMaxHeap(stations->data[carIndex].heap, atoi(args[i]));
        }
        printf("aggiunta\n");
    } else{
        printf("non aggiunta\n");
    }

}

void addCar(char **args, struct array *stations) {
    int carIndex = binarySearchStruct(stations, 0, stations->size, atoi(args[1]));

    if (carIndex == -1) {
        printf("non aggiunta\n");
    } else {
        insertMaxHeap(stations->data[carIndex].heap, atoi(args[2]));
        printf("aggiunta\n");
    }
}

void removeStation(char **args, struct array *stations) {
    int stationIndex = binarySearchStruct(stations, 0, stations->size, atoi(args[1]));

    if (stationIndex == -1) {
        printf("non demolita\n");
    } else {
        removeElement(stations, stationIndex);
        printf("demolita\n");

    }
}

void removeCar(char **args, struct array *stations) {
    int stationIndex = binarySearchStruct(stations, 0, stations->size, atoi(args[1]));

    if (stationIndex == -1) {
        printf("non rottamata\n");
    } else {
        int carIndex = max_heap_search(stations->data[stationIndex].heap, atoi(args[2]));
        if (carIndex == -1) {
            printf("non rottamata\n");
        } else {
            deleteElement(stations->data[stationIndex].heap, carIndex);
            printf("rottamata\n");
        }
    }
}

void directJourney(int dep, int arr, struct array *stations) {
    int depIndex = binarySearchStruct(stations, 0, stations->size, dep);
    int arrIndex = binarySearchStruct(stations, depIndex, stations->size, arr);
    int arraySize = arrIndex - depIndex + 1;
    int prec[arraySize], costs[arraySize], mapping[arraySize];
    int maxReachable, j;

    costs[0] = 0;
    prec[0] = 0;
    mapping[0] = stations->data[depIndex].value;

    for (int i = 1; i < arraySize; i++) {
        costs[i] = 2147483647;
        prec[i] = 2147483647;
        mapping[i] = stations->data[depIndex + i].value;
    }

    for (int i = 0; i < arraySize; i++) {
        j = i;
        maxReachable = mapping[i] + getMax(stations->data[depIndex + i].heap);
        while (maxReachable >= mapping[j + 1] && j < arraySize) {
            if (costs[i] == 2147483647){
                printf("nessun percorso\n");
                return;
            }else{
                if (costs[j + 1] > costs[i] + 1) {
                    costs[j + 1] = costs[i] + 1;
                    prec[j + 1] = mapping[i];
                }
                j++;
            }
        }
    }

    if (prec[arraySize - 1] == 2147483647) {
        printf("nessun percorso\n");
    } else {
        int pathSize = costs[arraySize - 1] + 1;
        int path[pathSize];
        int mapIndex;

        path[0] = stations->data[arrIndex].value;
        path[pathSize - 1] = stations->data[depIndex].value;

        int target = prec[arraySize - 1];

        for (int i = 1; i < pathSize - 1; ++i) {
            mapIndex = binarySearchAsc(mapping, 0, arraySize, target);
            path[i] = target;
            target = prec[mapIndex];
        }

        for (int i = pathSize - 1; i > 0; i--) {
            printf("%d ", path[i]);
        }
        printf("%d\n", path[0]);
    }
}

void indirectJourney(int dep, int arr, struct array *stations){
    int arrIndex = binarySearchStruct(stations, 0, stations->size, arr);
    int depIndex = binarySearchStruct(stations, arrIndex, stations->size, dep);
    int arraySize = depIndex - arrIndex + 1;
    int prec[arraySize], costs[arraySize], mapping[arraySize];
    int maxReachable, j;

    costs[0] = 0;
    prec[0] = 0;
    mapping[0] = stations->data[depIndex].value;

    for (int i = arraySize - 1; i >= 1; i--) {
        costs[i] = 2147483647;
        prec[i] = 2147483647;
        mapping[i] = stations->data[depIndex - i].value;
    }

    for (int i = 0; i < arraySize; i++) {
        j = i;
        maxReachable = mapping[i] - getMax(stations->data[depIndex - i].heap);
        while (maxReachable <= mapping[j + 1] && j < arraySize) {
            if (costs[i] == 2147483647){
                printf("nessun percorso\n");
                return;
            }else{
                if (costs[j + 1] >= costs[i] + 1) {
                    costs[j + 1] = costs[i] + 1;
                    prec[j + 1] = mapping[i];
                }
                j++;
            }
        }
    }

    if (prec[arraySize - 1] == 2147483647) {
        printf("nessun percorso\n");
    } else {
        int pathSize = costs[arraySize - 1] + 1;
        int path[pathSize];
        int mapIndex;

        path[0] = stations->data[arrIndex].value;
        path[pathSize - 1] = stations->data[depIndex].value;

        int target = prec[arraySize - 1];

        for (int i = 1; i < pathSize - 1; ++i) {
            mapIndex = binarySearchDesc(mapping, 0, arraySize, target);
            path[i] = target;
            target = prec[mapIndex];
        }

        for (int i = pathSize - 1; i > 0; i--) {
            printf("%d ", path[i]);
        }
        printf("%d\n", path[0]);
    }
}