#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct node {
    int data, color, cardinality; // 1-red, 0-black
    struct node *left, *right, *parent;
    struct node *cars;
};

struct node *root = NULL;

void addStation(char **args, int arg_count);

void removeStation(char **args);

void addCar(char **args);

void removeCar(char **args);

void transplant(struct node *u, struct node *v);

void deleteFixup(struct node *pt);

struct node *minimumNode(struct node *trav);

void pathPlan(char **args);

void directJourney_path(struct node *dep, struct node *dest);


// function to perform BST insertion of a node
struct node *insert(struct node *trav, struct node *temp) {
    // If the tree is empty,
    // return a new node
    if (trav == NULL)
        return temp;

    // Otherwise recur down the tree
    if (temp->data < trav->data) {
        trav->left = insert(trav->left, temp);
        trav->left->parent = trav;
    } else if (temp->data > trav->data) {
        trav->right = insert(trav->right, temp);
        trav->right->parent = trav;
    } else if (temp->data == trav->data) {
        trav->cardinality++;
    }

    // Return the (unchanged) node pointer
    return trav;
}

bool isLeftChild(struct node *node) {
    if (node == NULL || node->parent == NULL) {
        return false;
    }

    return node->parent->left == node;
}

bool isRightChild(struct node *node) {
    if (node == NULL || node->parent == NULL) {
        return false;
    }

    return node->parent->right == node;
}

void rightRotate(struct node *temp) {
    struct node *left = temp->left;
    temp->left = left->right;
    if (temp->left)
        temp->left->parent = temp;
    left->parent = temp->parent;
    if (!temp->parent)
        root = left;
    else if (temp == temp->parent->left)
        temp->parent->left = left;
    else
        temp->parent->right = left;
    left->right = temp;
    temp->parent = left;
}

void leftRotate(struct node *temp) {
    struct node *right = temp->right;
    temp->right = right->left;
    if (temp->right)
        temp->right->parent = temp;
    right->parent = temp->parent;
    if (!temp->parent)
        root = right;
    else if (temp == temp->parent->left)
        temp->parent->left = right;
    else
        temp->parent->right = right;
    right->left = temp;
    temp->parent = right;
}

// This function fixes violations
// caused by insertion
void fixup(struct node *treeRoot, struct node *pt) {
    struct node *parent_pt = NULL;
    struct node *grand_parent_pt = NULL;

    while ((pt != treeRoot) && (pt->color != 0)
           && (pt->parent->color == 1)) {
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        /*  Case : A
             Parent of pt is left child
             of Grand-parent of
           pt */
        if (parent_pt == grand_parent_pt->left) {

            struct node *uncle_pt = grand_parent_pt->right;

            /* Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if (uncle_pt != NULL && uncle_pt->color == 1) {
                grand_parent_pt->color = 1;
                parent_pt->color = 0;
                uncle_pt->color = 0;
                pt = grand_parent_pt;
            } else {

                /* Case : 2
                     pt is right child of its parent
                     Left-rotation required */
                if (pt == parent_pt->right) {
                    leftRotate(parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                     pt is left child of its parent
                     Right-rotation required */
                rightRotate(grand_parent_pt);
                int t = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = t;
                pt = parent_pt;
            }
        }

            /* Case : B
                 Parent of pt is right
                 child of Grand-parent of
               pt */
        else {
            struct node *uncle_pt = grand_parent_pt->left;

            /*  Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if ((uncle_pt != NULL) && (uncle_pt->color == 1)) {
                grand_parent_pt->color = 1;
                parent_pt->color = 0;
                uncle_pt->color = 0;
                pt = grand_parent_pt;
            } else {
                /* Case : 2
                   pt is left child of its parent
                   Right-rotation required */
                if (pt == parent_pt->left) {
                    rightRotate(parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                     pt is right child of its parent
                     Left-rotation required */
                leftRotate(grand_parent_pt);
                int t = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = t;
                pt = parent_pt;
            }
        }
    }
}

// Function to print inorder traversal
// of the fixated tree
void inorder(struct node *trav) {
    if (trav == NULL)
        return;
    inorder(trav->left);
    printf("%d ", trav->data);
    printf("%d ", trav->cardinality);
    inorder(trav->right);
}


// Function to delete a node from the red-black tree
void deleteNode(struct node *treeRoot, struct node *delNode) {
    struct node *temp, *child;
    int originalColor;

    // Find the node to be deleted
    temp = delNode;
    originalColor = temp->color;

    // Case 1: Node to be deleted has no children or only one child
    if (delNode->left == NULL) {
        child = delNode->right;
        transplant(delNode, delNode->right);
    } else if (delNode->right == NULL) {
        child = delNode->left;
        transplant(delNode, delNode->left);
    } else {
        // Case 2: Node to be deleted has two children
        temp = minimumNode(delNode->right);
        originalColor = temp->color;
        child = temp->right;

        if (temp->parent == delNode)
            child->parent = temp;
        else {
            transplant(temp, temp->right);
            temp->right = delNode->right;
            temp->right->parent = temp;
        }

        transplant(delNode, temp);
        temp->left = delNode->left;
        temp->left->parent = temp;
        temp->color = delNode->color;
    }

    if (originalColor == 0)
        deleteFixup(child);
}

// Helper function to transplant a node in the red-black tree
void transplant(struct node *u, struct node *v) {
    if (u->parent == NULL)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v != NULL)
        v->parent = u->parent;
}

// Helper function to find the minimum node in a subtree
struct node *minimumNode(struct node *trav) {
    struct node *temp = trav;
    while (temp->left != NULL)
        temp = temp->left;
    return temp;
}

struct node *maximumNode(struct node *trav) {
    struct node *temp = trav;
    while (temp->right != NULL)
        temp = temp->right;
    return temp;
}

// Function to fix violations caused by deletion
void deleteFixup(struct node *pt) {
    struct node *sibling;

    while (pt != root && (pt == NULL || pt->color == 0)) {
        if (pt == pt->parent->left) {
            sibling = pt->parent->right;

            if (sibling->color == 1) {
                sibling->color = 0;
                pt->parent->color = 1;
                leftRotate(pt->parent);
                sibling = pt->parent->right;
            }

            if ((sibling->left == NULL || sibling->left->color == 0) &&
                (sibling->right == NULL || sibling->right->color == 0)) {
                sibling->color = 1;
                pt = pt->parent;
            } else {
                if (sibling->right == NULL || sibling->right->color == 0) {
                    sibling->left->color = 0;
                    sibling->color = 1;
                    rightRotate(sibling);
                    sibling = pt->parent->right;
                }

                sibling->color = pt->parent->color;
                pt->parent->color = 0;
                sibling->right->color = 0;
                leftRotate(pt->parent);
                pt = root;
            }
        } else {
            sibling = pt->parent->left;

            if (sibling->color == 1) {
                sibling->color = 0;
                pt->parent->color = 1;
                rightRotate(pt->parent);
                sibling = pt->parent->left;
            }

            if ((sibling->left == NULL || sibling->left->color == 0) &&
                (sibling->right == NULL || sibling->right->color == 0)) {
                sibling->color = 1;
                pt = pt->parent;
            } else {
                if (sibling->left == NULL || sibling->left->color == 0) {
                    sibling->right->color = 0;
                    sibling->color = 1;
                    leftRotate(sibling);
                    sibling = pt->parent->left;
                }

                sibling->color = pt->parent->color;
                pt->parent->color = 0;
                sibling->left->color = 0;
                rightRotate(pt->parent);
                pt = root;
            }
        }
    }

    if (pt != NULL)
        pt->color = 0;
}

struct node *search(struct node *trav, int value) {
    if (trav == NULL || trav->data == value)
        return trav; // Return the current node if it matches the value or if the tree is empty

    if (value < trav->data)
        return search(trav->left, value); // Recursively search in the left subtree

    return search(trav->right, value); // Recursively search in the right subtree
}


int main() {
    char line[1000];
    char *word;
    char *args[515];
    int arg_count;

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
            addStation(args, arg_count);
        } else if (strcmp(args[0], "pianifica-percorso") == 0) {
            pathPlan(args);
        } else if (strcmp(args[0], "demolisci-stazione") == 0) {
            removeStation(args);
        } else if (strcmp(args[0], "aggiungi-auto") == 0) {
            addCar(args);
        } else if (strcmp(args[0], "rottama-auto") == 0) {
            removeCar(args);
        }
    }
    return 0;
}

//aggiungi-stazione distanza numero-auto autonomia-auto-1 ... autonomia-auto-n
void addStation(char **args, int arg_count) {
    struct node *temp = (struct node *) malloc(sizeof(struct node));
    temp->right = NULL;
    temp->left = NULL;
    temp->parent = NULL;
    temp->cardinality = 1;
    temp->data = atoi(args[1]);
    temp->color = 1;
    temp->cars = NULL;

    // calling function that performs bst insertion on this newly created node
    root = insert(root, temp);

    // calling function to preserve properties of rb tree
    fixup(root, temp);

    //searching for the newly added station so to update its cars
    struct node *result = search(root, temp->data);

    for (int i = 3; i < arg_count; i++) {
        struct node *carTemp = (struct node *) malloc(sizeof(struct node));
        carTemp->right = NULL;
        carTemp->cardinality = 1;
        carTemp->left = NULL;
        carTemp->parent = NULL;
        carTemp->data = atoi(args[i]);
        carTemp->color = 1;
        carTemp->cars = NULL;

        result->cars = insert(result->cars, carTemp);
    }
    printf("aggiunta\n");

    root->color = 0;
}

//aggiungi-auto distanza-stazione autonomia-auto-da-aggiungere
void addCar(char **args) {
    struct node *temp = (struct node *) malloc(sizeof(struct node));
    temp->right = NULL;
    temp->left = NULL;
    temp->parent = NULL;
    temp->data = atoi(args[1]);
    temp->color = 1;
    temp->cars = NULL;

    struct node *station = search(root, temp->data);

    if (station == NULL) {
        printf("non aggiunta\n");
    } else {
        struct node *carTemp = (struct node *) malloc(sizeof(struct node));
        carTemp->right = NULL;
        carTemp->cardinality = 1;
        carTemp->left = NULL;
        carTemp->parent = NULL;
        carTemp->data = atoi(args[2]);
        carTemp->color = 1;
        carTemp->cars = NULL;

        struct node *car = search(station->cars, carTemp->data);

        if (car == NULL) {
            station->cars = insert(station->cars, carTemp);
        } else {
            car->cardinality++;
        }
        printf("aggiunta\n");
    }
}

//demolisci-stazione distanza
void removeStation(char **args) {
    struct node *station = search(root, atoi(args[1]));
    if (station == NULL) {
        printf("non demolita\n");
    } else {
        free(station->cars);
        deleteNode(root, station);
        printf("demolita\n");
    }
}

//rottama-auto distanza-stazione autonomia-auto-da-rottamare
void removeCar(char **args) {
    struct node *station = search(root, atoi(args[1]));
    if (station == NULL) {
        printf("non rottamata\n");
    } else {
        struct node *car = search(station->cars, atoi(args[2]));

        if (car == NULL) {
            printf("non rottamata\n");
        } else if (car->cardinality > 1) {
            car->cardinality--;
            printf("rottamata\n");
        } else {
            deleteNode(station->cars, car);
            printf("rottamata\n");
        }
    }
}

// Function to perform DFS and find the path from node A to node B in a BST
bool dfs(struct node *trav, int A, int B) {
    if (trav == NULL)
        return false;

    // If the current node matches either A or B, we have found a path
    if (trav->data == A || trav->data == B)
        return true;

    // Check if A and B are in different subtrees
    bool left = dfs(trav->left, A, B);
    bool right = dfs(trav->right, A, B);

    // If A and B are in different subtrees, the current node is the lowest common ancestor
    if (left && right) {
        printf("%d ", trav->data);
        return true;
    }

    // If A and B are in the same subtree, continue searching for the lowest common ancestor
    return left || right;
}

//pianifica-percorso distanza-stazione-partenza distanza-stazione-arrivo
void pathPlan(char **args) {
    struct node *departure_station = search(root, atoi(args[1]));
    struct node *arrival_station = search(root, atoi(args[2]));

    if (departure_station->data == arrival_station->data) {
        printf("%s\n", args[1]);
    } else if (departure_station->data < arrival_station->data) { //andata
        directJourney_path(departure_station, arrival_station);
    } else {         //ritorno
    }
}

void directJourney_path(struct node *dep, struct node *dest) {
    int max_reachable = dep->data + maximumNode(dep->cars)->data;
    struct node *temp = root;
    int* path;  // Declare a pointer to an int (dynamic array)
    int size = 10;
    int capacity = 1;// Variable to store the index of the first free element in the array

    path = (int*)malloc(size * sizeof(int));
    path[0] = dep->data;

    while (true) {
        if (max_reachable > temp->data) {
            if (temp->data < dest->data) {
                //moving to the right subtree
                if (temp->right != NULL){
                    temp = temp->right;
                }else{
                    max_reachable = temp->data + maximumNode(temp->cars)->data;
                    temp = root;
                }
            } else if (temp->data > dest->data) {
                //moving to the left subtree
                temp = temp->left;
            } else{
                if (capacity < size) {
                    path[capacity] = temp->data;
                    capacity++;
                    break;
                }else{
                    int* tempArray = (int*)realloc(path, size * sizeof(int) * 2);
                    path = tempArray;
                }
            }
        }else if(max_reachable == temp->data){
            if (capacity < size) {
                path[capacity] = temp->data;
                capacity++;
            }else{
                int* tempArray = (int*)realloc(path, size * sizeof(int) * 2);
                path = tempArray;
            }
            max_reachable = temp->data + maximumNode(temp->cars)->data;
        } else {
            if (temp->left != NULL) {
                if (max_reachable < temp->left->data) {
                    //moving to the right subtree
                    if (capacity < size) {
                        path[capacity] = temp->parent->data;
                        capacity++;
                    }else{
                        int* tempArray = (int*)realloc(path, size * sizeof(int) * 2);
                        path = tempArray;
                    }
                    temp = temp->parent;
                    max_reachable = temp->parent->data + maximumNode(temp->parent->cars)->data;
                } else {
                    if (temp->left->data > dest->data) {
                        //moving to the left subtree
                        temp = temp->left;
                    } else if (temp->left->data < dest->data) {
                        if (temp->left->right != NULL) {
                            if (temp->left->right->data < dest->data) {
                                temp = temp->left->right;
                            } else {
                                if (temp->right != NULL){
                                    temp = temp->right;
                                }
                            }
                        }else{
                            printf("nessun percorso\n");
                            free(path);
                            path = NULL;
                            break;
                        }
                    } else if (temp->left->data == dest->data) {
                        if (capacity < size) {
                            path[capacity] = temp->parent->data;
                            capacity++;
                        }else{
                            int* tempArray = (int*)realloc(path, size * sizeof(int) * 2);
                            path = tempArray;
                        }
                        break;
                    } else {
                        printf("nessun percorso\n");
                        free(path);
                        path = NULL;
                        break;
                    }
                }
            } else {
                printf("nessun percorso\n");
                free(path);
                path = NULL;
                break;
            }
        }
    }
    if (path!=NULL){
        for (int i = 0; i < capacity; i++) {
            printf("%d ", path[i]);
        }
    }
}