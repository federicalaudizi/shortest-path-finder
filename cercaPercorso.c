#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10
#define RESIZE_FACTOR 2
struct node
{
    int data, color; // 1-red, 0-black
    struct node *left, *right, *parent;
};

struct node* root = NULL;

void  addStation(char** args, int arg_count);

// function to perform BST insertion of a node
struct node* insert(struct node* trav,struct node* temp){
    // If the tree is empty,
    // return a new node
    if (trav == NULL)
        return temp;

    // Otherwise recur down the tree
    if (temp->data < trav->data)
    {
        trav->left = insert(trav->left, temp);
        trav->left->parent = trav;
    }
    else if (temp->data > trav->data)
    {
        trav->right = insert(trav->right, temp);
        trav->right->parent = trav;
    }

    // Return the (unchanged) node pointer
    return trav;
}

void rightRotate(struct node* temp)
{
    struct node* left = temp->left;
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

void leftrotate(struct node* temp)
{
    struct node* right = temp->right;
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
void fixup(struct node* root, struct node* pt)
{
    struct node* parent_pt = NULL;
    struct node* grand_parent_pt = NULL;

    while ((pt != root) && (pt->color != 0)
           && (pt->parent->color == 1))
    {
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        /*  Case : A
             Parent of pt is left child
             of Grand-parent of
           pt */
        if (parent_pt == grand_parent_pt->left)
        {

            struct node* uncle_pt = grand_parent_pt->right;

            /* Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if (uncle_pt != NULL && uncle_pt->color == 1)
            {
                grand_parent_pt->color = 1;
                parent_pt->color = 0;
                uncle_pt->color = 0;
                pt = grand_parent_pt;
            }

            else {

                /* Case : 2
                     pt is right child of its parent
                     Left-rotation required */
                if (pt == parent_pt->right) {
                    leftrotate(parent_pt);
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
            struct node* uncle_pt = grand_parent_pt->left;

            /*  Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if ((uncle_pt != NULL) && (uncle_pt->color == 1))
            {
                grand_parent_pt->color = 1;
                parent_pt->color = 0;
                uncle_pt->color = 0;
                pt = grand_parent_pt;
            }
            else {
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
                leftrotate(grand_parent_pt);
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
void inorder(struct node* trav)
{
    if (trav == NULL)
        return;
    inorder(trav->left);
    printf("%d ", trav->data);
    inorder(trav->right);
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
            printf("aggiunta\n");
        } else if (strcmp(args[0], "pianifica-percorso") == 0) {
            //TODO pathPlan
        } else if (strcmp(args[0], "demolisci-stazione") == 0) {
            //TODO removeStation
        } else if (strcmp(args[0], "aggiungi-auto") == 0) {
            //TODO addCar
        } else if (strcmp(args[0], "rottama-auto") == 0) {
            //TODO removeCar
        }
    }
    return 0;
}

void addStation(char**args, int arg_count){
    struct node* temp= (struct node*)malloc(sizeof(struct node));
    temp->right = NULL;
    temp->left = NULL;
    temp->parent = NULL;
    temp->data = atoi(args[1]);
    temp->color = 1;

    // calling function that performs bst insertion on this newly created node
    root = insert(root, temp);
    // calling function to preserve properties of rb tree
    fixup(root, temp);
    root->color = 0;
}
